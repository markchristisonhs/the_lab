/*
 * SMC_Bus.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: mchristiso
 */

#include "stdafx.h"

#include "SMC_Bus.h"

#ifdef _UNIX
#include <unistd.h>
#include <thread>
#elif WIN32
#include <Windows.h>
#endif

#include <sstream>

using namespace std;

//cook booked from stack overflow:

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}

HNS_LogALine2 *g_logaline = nullptr;

void LogALine_SMC(const std::string &line, const std::string &caller)
{
    if(g_logaline != nullptr)
    {
        g_logaline->fLogALine(line,caller);
    }
}

int64_t Clock_to_Int()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    long result = epoch.count();

    return result;
}

void LogCommand(const HNS_SMCBus_Command &cmd)
{
    stringstream ss;
    string caller = "LogCommand";

    ss.str("");
    ss << "This command is " << hex(cmd.fGetCommand()) << " for address " << hex(cmd.fGetAddress());
    LogALine_SMC(ss.str(),caller);
}

enum
{
	HNS_SMC_CMD_DISPLAYDATA = 0x1,
	HNS_SMC_CMD_SETDATA = 0x5,
	HNS_SMC_CMD_SETPWM = 0x76,
	HNS_SMC_CMD_BLANKDISPLAY = 0x77,
	HNS_SMC_CMD_RETURNDATA = 0x81,
	HNS_SMC_CMD_RETURNDIAG = 0x82,
	HNS_SMC_CMD_RETURNVERSION = 0x83,
	HNS_SMC_CMD_INITPIXELOUT = 0x84,
	HNS_SMC_CMD_POWERDOWN = 0x85,
	HNS_SMC_CMD_RETURNHIGHTEMP = 0xA0,
	HNS_SMC_CMD_RETURNLOWTEMP = 0xA1,
	HNS_SMC_CMD_CALTEMP = 0xA2,
	HNS_SMC_CMD_CALPWM = 0xA3,
	HNS_SMC_CMD_SETPWMREF = 0xA4,
	HNS_SMC_CMD_RETURNLOWVOLT = 0xA5,
	HNS_SMC_CMD_RETURNHIGHVOLT = 0xA6,
	HNS_SMC_CMD_CALVOLTHIGH = 0xA7,
	HNS_SMC_CMD_SETCHARGETIME = 0xA8,
	HNS_SMC_CMD_SETCHARGEONTIME = 0xA9,
	HNS_SMC_CMD_SETCHARGEOFFTIME = 0xAA,
	HNS_SMC_CMD_CALVOLTLOW = 0xAB,
	HNS_SMC_CMD_RETURNVERANDSWITCH = 0xAC,
	HNS_SMC_CMD_SETRAWPWM = 0xAD,
	HNS_SMC_CMD_SETCALPTDATA = 0xAE,
	HNS_SMC_CMD_SETSLOPE = 0xAF,
	HNS_SMC_CMD_GETRAWPWM = 0xB0,
	HNS_SMC_CMD_GETCALPTDATA = 0xB1,
	HNS_SMC_CMD_GETSLOPE = 0xB2,
	HNS_SMC_CMD_GETCHARBDTYPE = 0xB3,
	HNS_SMC_CMD_SETBRIGHTFACTOR = 0xB4,
	HNS_SMC_CMD_GETBRIGHTFACTOR = 0xB5,
	HNS_SMC_CMD_GETFLASHBLOCK = 0xB6,
	HNS_SMC_CMD_CALTEMPCENTIGRADE = 0xB7,
	HNS_SMC_CMD_GETTEMPCENTIGRADE = 0xB8,
	HNS_SMC_CMD_INCCALCOUNT = 0xB9,
	HNS_SMC_CMD_GETCALCOUNT = 0xBA,
	HNS_SMC_CMD_SETPWMPERIOD = 0xBB,
	HNS_SMC_CMD_STOREPWMFLASH = 0xBC,
	HNS_SMC_CMD_GETPWMFLASH = 0x7D
};

typedef enum
{
    HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH = 0x0,
    HNS_SMC_BUS_RECV_STATE_DATA_COLLECTION,
    HNS_SMC_BUS_RECV_STATE_DATA_PIX_OUT_GET_SIZE
} type_hns_smc_state;

bool HNS_SMCBus_Command::f_times_out = true;
//MSC20211115 set to 300ms to match CPU-6M.
std::chrono::milliseconds HNS_SMCBus_Command::f_timeout = std::chrono::milliseconds(300);
std::chrono::milliseconds HNS_SMCBus_Command::f_send_delay = std::chrono::milliseconds(50);

HNS_SMCBus_Command::HNS_SMCBus_Command()
    : f_command(0)
    , f_address(0)
    , f_sent(false)
    , f_is_pix_out(false)
    , f_bytes_expected(0)
    , f_timed_out(false)
    , f_command_created(std::chrono::steady_clock::now())
{

}

HNS_SMCBus_Command::HNS_SMCBus_Command(const unsigned char &command, const unsigned char &address, const std::vector<unsigned char> &send_data)
    : f_command(command)
    , f_address(address)
    , f_send_data(send_data)
    , f_sent(false)
    , f_is_pix_out(false)
    , f_timed_out(false)
    , f_command_created(std::chrono::steady_clock::now())
{
    if(command == HNS_SMC_CMD_RETURNDATA)
    {
        if(address == HNS_SMC_ADDR_CHARGER)
        {
            f_bytes_expected = 7;
        }
        else
        {
            f_bytes_expected = 1;
        }
    }
    //MSC20211113 Remember, display data does not expect data back.  Other commands I THINK don't expect data back, but not sure.  Research required.
    else if((command == HNS_SMC_CMD_DISPLAYDATA)
         || (command == HNS_SMC_CMD_SETPWM))
    {
        f_bytes_expected = 0;
    }
    else
    {
        f_bytes_expected = 1;
    }
}

HNS_SMCBus_Command::HNS_SMCBus_Command(const unsigned char &address)
    : f_command(0)
    , f_address(address)
    , f_sent(false)
    , f_is_pix_out(true)
    , f_bytes_expected(0)
    , f_timed_out(false)
    , f_command_created(std::chrono::steady_clock::now())
{

}

bool HNS_SMCBus_Command::fCheckForTimeout()
{
    if(f_sent)
    {
        std::chrono::time_point<std::chrono::steady_clock> current_clock = std::chrono::steady_clock::now();
        std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - f_command_sent);
        if(diff > f_timeout)
        {
            f_timed_out = true;
        }
    }

    return f_timed_out;
}

vector<unsigned char> HNS_SMCBus_Command::fGetSendData() const
{
    return fBuildPacket();
}

vector<unsigned char> HNS_SMCBus_Command::fGetReceivedData() const
{
    return f_returned_data;
}

size_t HNS_SMCBus_Command::fGetReceivedDataSize() const
{
    return f_returned_data.size();
}

void HNS_SMCBus_Command::fDataWasSent()
{
    f_command_sent = std::chrono::steady_clock::now();
    f_sent = true;
}

bool HNS_SMCBus_Command::fWasDataSent() const
{
    return f_sent;
}

bool HNS_SMCBus_Command::fIsCommandDone() const
{
    bool result = false;

    if(fIsReturnDataExpected())
    {
        result = f_sent && fReturnDataIsAvailable();
    }
    else
    {
        result = f_sent && fIsSendDelayOver();
    }

    if(fIsTimedOut())
    {
        result = true;
    }

    return result;
}

bool HNS_SMCBus_Command::fIsSendDelayOver() const
{
//    stringstream ss;
    std::chrono::time_point<std::chrono::steady_clock> current_clock = std::chrono::steady_clock::now();
    std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - f_command_created);
    if(diff > f_send_delay)
    {
//        ss.str("");
//        ss << "Command " << static_cast<unsigned int>(fGetCommand()) << " to address " << static_cast<unsigned int>(fGetAddress()) << " has reached the send delay";
//        LogALine_SMC(ss.str(),"HNS_SMCBus_Command::fIsSendDelayOver()");
        return true;
    }
    else
    {
        return false;
    }
}

bool HNS_SMCBus_Command::fIsReturnDataExpected() const
{
    return f_bytes_expected > 0;
}

bool HNS_SMCBus_Command::fReturnDataIsAvailable() const
{
    return f_returned_data.size() >= f_bytes_expected;
}

unsigned char HNS_SMCBus_Command::fGetAddress() const
{
    return f_address;
}

unsigned char HNS_SMCBus_Command::fGetCommand() const
{
    return f_command;
}

bool HNS_SMCBus_Command::fIsPixOutCommand() const
{
    return f_is_pix_out;
}

void HNS_SMCBus_Command::fAddReturnData(const unsigned char &data)
{
    f_returned_data.push_back(data);
}

void HNS_SMCBus_Command::fSetReturnData(const std::vector<unsigned char> &data)
{
    f_returned_data = data;
}

void HNS_SMCBus_Command::fSetBytesExpected(const size_t &bytes_expected)
{
    f_bytes_expected = bytes_expected;
}

size_t HNS_SMCBus_Command::fGetBytesExpected() const
{
    return f_bytes_expected;
}

void HNS_SMCBus_Command::fSetTimeOut(const bool &times_out, const chrono::milliseconds &timeout)
{
    f_times_out = times_out;
    f_timeout = timeout;
}

void HNS_SMCBus_Command::fSetTimeOut(const bool &times_out, const int &timeout)
{
    f_times_out = times_out;
    f_timeout = std::chrono::milliseconds(timeout);
}

bool HNS_SMCBus_Command::fIsTimedOut() const
{
    return f_timed_out;
}

vector<unsigned char> HNS_SMCBus_Command::fBuildPacket() const
{
    vector<unsigned char> to_send;
    size_t i;

    to_send.push_back(0xFF);
    to_send.push_back(0xFF);
    to_send.push_back(0xFF);
    to_send.push_back(0x55);
    to_send.push_back(0xAA);
    to_send.push_back((unsigned char)(f_send_data.size() >> 8) & 0xFF);
    to_send.push_back((unsigned char)(f_send_data.size() & 0xFF));
    to_send.push_back(f_command);
    to_send.push_back(f_address);
    for(i=0;i<f_send_data.size();i++)
    {
        to_send.push_back(f_send_data[i]);
    }

    return to_send;
}

HNS_SMCBus2::HNS_SMCBus2()
    : f_pending_command(nullptr)
{
}

HNS_SMCBus2::HNS_SMCBus2(const HNS_SMCBus2 &copy)
{
    *this = copy;
}

HNS_SMCBus2::~HNS_SMCBus2()
{
    delete f_pending_command;
    f_pending_command = nullptr;
}

HNS_SMCBus2 &HNS_SMCBus2::operator =(const HNS_SMCBus2 &rhs)
{
    f_receive_buffer = rhs.f_receive_buffer;

    f_output_buffer = rhs.f_output_buffer;

    f_pending_command_queue = rhs.f_pending_command_queue;
    f_completed_command_queue = rhs.f_completed_command_queue;

    delete f_pending_command;
    f_pending_command = nullptr;
    if(rhs.f_pending_command != nullptr)
    {
        f_pending_command = new HNS_SMCBus_Command(*rhs.f_pending_command);
    }

    return *this;
}

void HNS_SMCBus2::fSetLogALine(HNS_LogALine2 *logaline)
{
    g_logaline = logaline;
}

void HNS_SMCBus2::fRunCycle()
{
    if(f_pending_command != nullptr)
    {
        f_pending_command->fCheckForTimeout();
    }
    fCleanPending();

    if(f_pending_command == nullptr)
    {
        fGetNextPendingCommand();
    }
}

void HNS_SMCBus2::fDisplayData(const unsigned char &board_number, const std::vector<unsigned char> &data)
{
    fPushNewPendingCommand(HNS_SMCBus_Command(HNS_SMC_CMD_DISPLAYDATA,board_number,data));
}

void HNS_SMCBus2::fGetData(const unsigned char &board_number)
{
    fPushNewPendingCommand(HNS_SMCBus_Command(HNS_SMC_CMD_RETURNDATA,board_number));
}

void HNS_SMCBus2::fStartPixelOut(const unsigned char &board_number)
{
    fPushNewPendingCommand(HNS_SMCBus_Command(HNS_SMC_CMD_INITPIXELOUT,board_number));
}

void HNS_SMCBus2::fGetPixelOutData(const unsigned char &board_number, const int &num_boards)
{
    if(board_number <= 0x7F)
    {
        fPushNewPendingCommand(HNS_SMCBus_Command(board_number));
    }
    else if((board_number == HNS_SMC_ADDR_ALL_CHARBOARDS) && ((num_boards >= 0) && (num_boards <= 0x7F)))
    {
        for(int i = 0; i < num_boards; i++)
        {
            fPushNewPendingCommand(HNS_SMCBus_Command(i+1));
        }
    }
}

bool HNS_SMCBus2::fSetPWM(const unsigned char &board_number, const unsigned int &pwm_val)
{
    vector<unsigned char> data;
    if(pwm_val > 0xFF)
    {
        data.push_back(0xFF);
    }
    else
    {
        data.push_back(pwm_val);
    }
    //f_pending_command_queue.push_back(HNS_SMCBus_Command(HNS_SMC_CMD_SETPWM,board_number,data));
    fPushNewPendingCommand(HNS_SMCBus_Command(HNS_SMC_CMD_SETPWM,board_number,data));

//    fPrintQueue();

    return false;
}

void HNS_SMCBus2::fPushBuffer(const vector<unsigned char> &input)
{
    f_receive_buffer.insert(f_receive_buffer.end(), input.begin(), input.end());

    fCheckBuffer();
}

void HNS_SMCBus2::fPushBuffer(const unsigned char &input)
{
    fPushBuffer(vector<unsigned char>(1,input));
}

void HNS_SMCBus2::fPushBuffer(const unsigned char *input, const size_t &input_size)
{
    vector<unsigned char> temp_vec(input_size);
    for(size_t ui=0;ui<input_size;ui++)
    {
        temp_vec[ui] = input[ui];
    }

    fPushBuffer(temp_vec);
}

type_hns_smc_bus_error HNS_SMCBus2::fGetPendingCommand(HNS_SMCBus_Command &result)
{
    type_hns_smc_bus_error error = HNS_SMC_BUS_NOERR;

    if(f_pending_command != nullptr)
    {
        result = *f_pending_command;
    }
    else
    {
        error = HNS_SMC_BUS_NO_PENDING_COMMAND;
    }

    return error;
}

void HNS_SMCBus2::fPendingCommandSent()
{
    stringstream ss;
    if(f_pending_command != nullptr)
    {
//        ss.str("");
//        ss << "Data was sent at timestamp " << Clock_to_Int();
//        LogALine_SMC(ss.str(),"HNS_SMCBus2::fPendingCommandSent");
        f_pending_command->fDataWasSent();
    }

    fCleanPending();
}

bool HNS_SMCBus2::fIsBusy() const
{
    return (f_pending_command != nullptr);
}

type_hns_smc_bus_error HNS_SMCBus2::fPopCompleteCommand(HNS_SMCBus_Command &result)
{
    type_hns_smc_bus_error error = HNS_SMC_BUS_NOERR;

    if(!f_completed_command_queue.empty())
    {
        result = f_completed_command_queue.front();
        f_completed_command_queue.pop();
    }
    else
    {
        error = HNS_SMC_BUS_NO_MORE_COMPLETED_COMMANDS;
    }

    return error;
}

void HNS_SMCBus2::fPrintQueue()
{
    stringstream ss;
    string caller = "HNS_SMCBus2::fPrintQueue";

    ss << "There are " << f_pending_command_queue.size() << " commands waiting to send";
    LogALine_SMC(ss.str(),caller);

    ss.str("");
    ss << "Pending command queue command/address pairs are: ";
    for(size_t ui = 0; ui<f_pending_command_queue.size();ui++)
    {
        ss << hex(f_pending_command_queue[ui].fGetCommand()) << "/" << hex(f_pending_command_queue[ui].fGetAddress()) << " ";
    }
    LogALine_SMC(ss.str(),caller);

    if(f_pending_command != nullptr)
    {
        ss.str("");
        ss << "The pending command is " << hex(f_pending_command->fGetCommand()) << " for address " << hex(f_pending_command->fGetAddress());
        LogALine_SMC(ss.str(),caller);
    }

    ss.str("");
    ss << "There are " << f_completed_command_queue.size() << " finished commands waiting";
    LogALine_SMC(ss.str(),caller);
}

void HNS_SMCBus2::fCheckBuffer()
{
    stringstream ss;
    static type_hns_smc_state current_state = HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH;
    vector<unsigned char>::iterator it = f_receive_buffer.begin();
    while(it != f_receive_buffer.end())
    {
        if(f_pending_command == nullptr)
        {
            //no command to look for a response for.  Throw out the response.
            it = f_receive_buffer.erase(f_receive_buffer.begin());
        }
        else
        {
            // only run this if still looking for return data for the currently pending command
            if(!f_pending_command->fReturnDataIsAvailable())
            {
                //looking for a command
                switch(current_state)
                {
                case HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH:
                    if(f_pending_command->fIsPixOutCommand())
                    {
                        current_state = HNS_SMC_BUS_RECV_STATE_DATA_PIX_OUT_GET_SIZE;
                    }
                    else
                    {
                        if(*it == f_pending_command->fGetAddress())
                        {
//                            ss.str("");
//                            ss << "Found address " << static_cast<unsigned int>(f_pending_command->fGetAddress()) << ", now look for data";
//                            LogALine_SMC(ss.str(),"HNS_SMCBus2::fCheckBuffer");
                            current_state = HNS_SMC_BUS_RECV_STATE_DATA_COLLECTION;
                        }
                        it = f_receive_buffer.erase(f_receive_buffer.begin());
                    }
                    break;
                case HNS_SMC_BUS_RECV_STATE_DATA_PIX_OUT_GET_SIZE:
                    f_pending_command->fSetBytesExpected(*it);
                    if(*it > 0)
                    {
                        current_state = HNS_SMC_BUS_RECV_STATE_DATA_COLLECTION;
                    }
                    else
                    {
                        current_state = HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH;
                    }
                    it = f_receive_buffer.erase(f_receive_buffer.begin());
                    break;
                case HNS_SMC_BUS_RECV_STATE_DATA_COLLECTION:
//                    ss.str("");
//                    ss << "Adding data " << static_cast<unsigned int>(*it) << " to pending command";
//                    LogALine_SMC(ss.str(),"HNS_SMCBus2::fCheckBuffer");
                    f_pending_command->fAddReturnData(*it);
                    it = f_receive_buffer.erase(f_receive_buffer.begin());
                    break;
                default:
                    current_state = HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH;
                    break;
                }

                if(f_pending_command->fReturnDataIsAvailable())
                {
//                    ss.str("");
//                    ss << "Found all data for address " << static_cast<unsigned int>(f_pending_command->fGetAddress()) << ", clean pending command";
//                    LogALine_SMC(ss.str(),"HNS_SMCBus2::fCheckBuffer");
                    //no need to clean the queue here, let the event loop take care of it
                    current_state = HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH;
                }
            }
            else
            {
                //probably not necessary, but just in case.
                current_state = HNS_SMC_BUS_RECV_STATE_ADDRESS_SEARCH;
            }
        }
    }
}

void HNS_SMCBus2::fGetNextPendingCommand()
{
    stringstream ss;
    if(f_pending_command == nullptr)
    {
        if(!f_pending_command_queue.empty())
        {
            f_pending_command = new HNS_SMCBus_Command(f_pending_command_queue.front());
            //no need to log display datas, that happens a lot, is of not of concern right now, and would flood the log
//            if(f_pending_command->fGetCommand() != HNS_SMC_CMD_DISPLAYDATA)
//            {
//                ss.str("");
//                ss << "At timestamp " << Clock_to_Int() << " the pending command is now:";
//                LogALine_SMC(ss.str(),"HNS_SMCBus2::fGetNextPendingCommand");
//                LogCommand(*f_pending_command);
//            }
            f_pending_command_queue.erase(f_pending_command_queue.begin());

//            fPrintQueue();
        }
    }
}

void HNS_SMCBus2::fCleanPending()
{
    stringstream ss;
    if(f_pending_command != nullptr)
    {
        if(f_pending_command->fIsCommandDone())
        {
            if(f_pending_command->fIsTimedOut())
            {
                ss.str("");
                ss << "Command " << hex(f_pending_command->fGetCommand()) << " for address " << hex(f_pending_command->fGetAddress()) << " has timed out at timestamp " << std::dec << Clock_to_Int();
                LogALine_SMC(ss.str(),"HNS_SMCBus2::fCleanPending");
            }
            if(f_pending_command->fIsReturnDataExpected() || f_pending_command->fIsPixOutCommand())
            {
                f_completed_command_queue.push(*f_pending_command);
            }
            delete f_pending_command;
            f_pending_command = nullptr;
        }
    }
}

void HNS_SMCBus2::fPushNewPendingCommand(HNS_SMCBus_Command push)
{
    bool found = false;
    for(size_t ui=0;ui<f_pending_command_queue.size();ui++)
    {
        if((push.fGetCommand() == f_pending_command_queue[ui].fGetCommand())
        && (push.fGetAddress() == f_pending_command_queue[ui].fGetAddress()))
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        f_pending_command_queue.push_back(push);
    }
}
