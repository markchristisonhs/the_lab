/*
 * SMC_Bus.h
 *
 *  Created on: Aug 9, 2019
 *      Author: mchristiso
 */

#ifndef SRC_SMC_BUS_H_
#define SRC_SMC_BUS_H_

#include "HNS_CommLink.h"
#include "PixelOut.h"
#include "logaline.h"
#include <vector>
#include <queue>
#include <chrono>

class SMC_Bus_Return_Data;

enum
{
    HNS_SMC_ADDR_ALL = 0xFF,
    HNS_SMC_ADDR_ALL_CHARBOARDS = 0xFE,
    HNS_SMC_ADDR_BATTVOLTAGE = 0xFD,
    HNS_SMC_ADDR_BATTDIAG = 0xFC,
    HNS_SMC_ADDR_PHOTOCELL = 0xFB,
    HNS_SMC_ADDR_RADAR = 0xFA,
    HNS_SMC_ADDR_CHARGER = 0xF9
};

typedef enum
{
    HNS_SMC_BUS_NOERR = 0x0,
    HNS_SMC_BUS_NOCOMMAND_IN_QUEUE,
    HNS_SMC_BUS_BUSY,
    HNS_SMC_BUS_NO_PENDING_COMMAND,
    HNS_SMC_BUS_NO_MORE_COMPLETED_COMMANDS
} type_hns_smc_bus_error;

enum
{
    HNS_SMC_BUS_NOSIM = 0x0,
    HNS_SMC_BUS_SIM_RADAR = 0x1
};

class HNS_SMCBus_Command
{
public:
    HNS_SMCBus_Command();
    HNS_SMCBus_Command(const unsigned char &command, const unsigned char &address, const std::vector<unsigned char> &send_data = std::vector<unsigned char>());
    //HNS_SMCBus_Command(const unsigned char &command, const unsigned char &address);
    //This form is for pixel out tests.
    HNS_SMCBus_Command(const unsigned char &address);

    bool fCheckForTimeout();

    std::vector<unsigned char> fGetSendData() const;
    std::vector<unsigned char> fGetReceivedData() const;
    size_t fGetReceivedDataSize() const;
    void fDataWasSent();
    bool fWasDataSent() const;

    bool fIsCommandDone() const;
    bool fIsSendDelayOver() const;

    bool fIsReturnDataExpected() const;
    bool fReturnDataIsAvailable() const;

    unsigned char fGetAddress() const;
    unsigned char fGetCommand() const;

    bool fIsPixOutCommand() const;

    void fAddReturnData(const unsigned char &data);
    void fSetReturnData(const std::vector<unsigned char> &data);

    void fSetBytesExpected(const size_t &bytes_expected);
    size_t fGetBytesExpected() const;

    static void fSetTimeOut(const bool &times_out, const std::chrono::milliseconds &timeout);
    static void fSetTimeOut(const bool &times_out, const int &timeout);

    bool fIsTimedOut() const;
private:
    std::vector<unsigned char> fBuildPacket() const;

    unsigned char f_command;
    unsigned char f_address;
    std::vector<unsigned char> f_send_data;
    std::vector<unsigned char> f_returned_data;
    bool f_sent;
    bool f_is_pix_out;
    size_t f_bytes_expected;

    static bool f_times_out;
    static std::chrono::milliseconds f_timeout;
    static std::chrono::milliseconds f_send_delay;

    bool f_timed_out;
    std::chrono::time_point<std::chrono::steady_clock> f_command_created;
    std::chrono::time_point<std::chrono::steady_clock> f_command_sent;
};

class HNS_SMCBus2
{
public:
    HNS_SMCBus2();
    HNS_SMCBus2(const HNS_SMCBus2 &copy);
    ~HNS_SMCBus2();

    HNS_SMCBus2 &operator=(const HNS_SMCBus2 &rhs);

    void fSetLogALine(HNS_LogALine2 *logaline);

    void fRunCycle();

    void fDisplayData(const unsigned char &board_number, const std::vector<unsigned char> &data);
    void fGetData(const unsigned char &board_number);
    void fStartPixelOut(const unsigned char &board_number);
    void fGetPixelOutData(const unsigned char &board_number, const int &num_boards = -1);
    bool fSetPWM(const unsigned char &board_number, const unsigned int &pwm_val);
    void fSetStrobe(const bool &onoff, const bool &daynight, const int &pattern);

    //returns any completed commands after pushing.  Commands are purged after completion at this point.
    void fPushBuffer(const std::vector<unsigned char> &input);
    void fPushBuffer(const unsigned char &input);
    void fPushBuffer(const unsigned char *input, const size_t &input_size);

    type_hns_smc_bus_error fGetPendingCommand(HNS_SMCBus_Command &result);
    void fPendingCommandSent();
    bool fIsBusy() const;

    type_hns_smc_bus_error fPopCompleteCommand(HNS_SMCBus_Command &result);

    void fPrintQueue();

    void fSetSimFlag(const unsigned int &flag);
    void fClearSimFlag(const unsigned int &flag);
private:
    bool fGetSimFlag(const unsigned int &flag);
    unsigned char fGetSimSpeed();
    void fCheckBuffer();
    void fGetNextPendingCommand();
    void fCleanPending();
    void fPushNewPendingCommand(HNS_SMCBus_Command push);
    std::vector<unsigned char> f_receive_buffer;

    std::vector< std::vector <unsigned char> > f_output_buffer;

    std::vector<HNS_SMCBus_Command> f_pending_command_queue;
    std::queue<HNS_SMCBus_Command> f_completed_command_queue;
    unsigned int f_sim_flag;

    HNS_SMCBus_Command *f_pending_command;
};

#endif /* SRC_SMC_BUS_H_ */
