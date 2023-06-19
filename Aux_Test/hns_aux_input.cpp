#include "hns_aux_input.h"

#include <QDebug>

using namespace std;

HNS_Aux_Input_Falling_Timer::HNS_Aux_Input_Falling_Timer():
    f_port_number(-1)
  , f_duration(0)
  , f_enabled(false)
  , f_current_val(0)
  , f_running(false)
  , f_edge_time(0)
{

}

HNS_Aux_Input_Falling_Timer::HNS_Aux_Input_Falling_Timer(const int &port_number):
    f_port_number(port_number)
  , f_duration(0)
  , f_enabled(false)
  , f_current_val(0)
  , f_running(false)
  , f_edge_time(0)
{

}

int HNS_Aux_Input_Falling_Timer::fRecvInput(const int &val, const int64_t &time)
{
    int result = HNS_INPUT_NO_EDGE;

    if(f_enabled)
    {
        if(val == 1 && f_current_val == 0)
        {
            f_running = true;
            result = HNS_INPUT_RISING_EDGE;
            f_current_val = val;
        }
        else if(val == 0 && f_current_val == 1)
        {
            f_edge_time = time;
            qDebug() << "Falling edge detected on port" << f_port_number << ", marking edge time as" << f_edge_time;
            result = HNS_INPUT_FALLING_EDGE;
            f_current_val = val;
        }
    }
    return result;
}

void HNS_Aux_Input_Falling_Timer::fCancel()
{
    f_running = false;
    f_edge_time = 0;
}

bool HNS_Aux_Input_Falling_Timer::fIsRunning(const int64_t &time, HNS_NTCIP_MessageIDCode *result_id)
{
    bool result = false;

    if(f_running)
    {
        if(f_current_val == 0)
        {
            int64_t duration_temp = static_cast<int64_t>(f_duration);
            if((time - f_edge_time) > duration_temp)
            {
                qDebug() << "Input port" << f_port_number << "timed out.  Duration was" << duration_temp << "edge time was" << f_edge_time << "and current time is" << time;
                f_running = false;
                result = false;
            }
            else
            {
                result = true;
                if(result_id != nullptr)
                {
                    *result_id = f_msg_id;
                }
            }
        }
        else
        {
            result = true;
            if(result_id != nullptr)
            {
                *result_id = f_msg_id;
            }
        }
    }
    return result;
}

void HNS_Aux_Input_Falling_Timer::fSetDuration(const int &duration)
{
    if(duration >= 0)
    {
        int duration_temp = duration * 1000;
        if(f_duration != duration_temp)
        {
            f_duration = duration_temp;
            fCancel();
        }
    }
}

void HNS_Aux_Input_Falling_Timer::fSetEnabled(const bool &enabled)
{
    f_enabled = enabled;
    if(!enabled)
    {
        fCancel();
    }
}

void HNS_Aux_Input_Falling_Timer::fSetMessage(const HNS_NTCIP_MessageIDCode &msg_code)
{
    if(f_msg_id != msg_code)
    {
        f_msg_id = msg_code;
        fCancel();
    }
}

HNS_Aux_Inputs_PennStyle::HNS_Aux_Inputs_PennStyle():
    f_input_channels(4,nullptr)
{

}

HNS_Aux_Inputs_PennStyle::HNS_Aux_Inputs_PennStyle(const std::vector<int> &port_numbers, const int &algorithm):
    f_input_channels(port_numbers.size(),nullptr)
{
    if(algorithm == HNS_INPUT_STYLE_FALLING_TIMER)
    {
        for(size_t ui=0;ui<f_input_channels.size();ui++)
        {
            f_input_channels[ui] = (HNS_Aux_Input *) new HNS_Aux_Input_Falling_Timer(port_numbers[ui]);
        }
    }
}

void HNS_Aux_Inputs_PennStyle::fRecvInput(const int &val, const int64_t &time, const int &port_number, const HNS_NTCIP_MessageActivationCode &running_code)
{
    int result_edge = HNS_INPUT_NO_EDGE;
    HNS_Aux_Input *channel_to_add = nullptr;
    for(size_t ui=0;ui<f_input_channels.size();ui++)
    {
        if(f_input_channels[ui] != nullptr)
        {
            if(f_input_channels[ui]->fGetPortNumber() == port_number)
            {
                result_edge = f_input_channels[ui]->fRecvInput(val,time);
                channel_to_add = f_input_channels[ui];
                break;
            }
        }
    }
    if(channel_to_add != nullptr && result_edge == HNS_INPUT_RISING_EDGE)
    {
        qDebug() << "Rising edge found";
        bool port_found = false;
        for(size_t ui=0;ui<f_running_queue.size();ui++)
        {
            if(f_running_queue[ui] != nullptr)
            {
                if(f_running_queue[ui]->fGetPortNumber() == port_number)
                {
                    port_found = true;
                    break;
                }
            }
        }

        if(!port_found)
        {
            qDebug() << "Adding input to queue";
            if(f_running_queue.size() == 0)
            {
                f_ret_code = running_code;
            }
            f_running_queue.push_back(channel_to_add);
        }
    }
}

void HNS_Aux_Inputs_PennStyle::fCancel()
{
    for(size_t ui=0;ui<f_input_channels.size();ui++)
    {
        if(f_input_channels[ui] != nullptr)
        {
            f_input_channels[ui]->fCancel();
        }
    }
}

bool HNS_Aux_Inputs_PennStyle::fIsRunning(const int64_t &time, HNS_NTCIP_MessageActivationCode *result_code, int &action)
{
    bool result = false;
    HNS_NTCIP_MessageIDCode temp_id;
    static HNS_Aux_Input *last_running_input = nullptr;

    while(f_running_queue.size() > 0)
    {
        if(f_running_queue.front() != nullptr)
        {
            if(f_running_queue.front()->fIsRunning(time,&temp_id))
            {
                if(result_code != nullptr)
                {
                    result_code->fSetMemoryType(temp_id.fGetMemoryType());
                    result_code->fSetMessageNo(temp_id.fGetMessageNumber());
                    result_code->fSetCRC(temp_id.fGetCRCAsInt());
                    result_code->fSetPriority(127);
                    result_code->fSetDuration(0xFFFF);
                    result_code->fSetSrcAddr("127.0.0.1");
                }
                if(last_running_input != f_running_queue.front())
                {
                    last_running_input = f_running_queue.front();
                    action = HNS_AUX_ACTION_START;
                }
                return true;
            }
            else
            {
                f_running_queue.pop_back();
            }
        }
        else
        {
            f_running_queue.pop_back();
        }
    }

    *result_code = f_ret_code;
    if(last_running_input != nullptr)
    {
        action = HNS_AUX_ACTION_STOP;
    }
    else
    {
        action = HNS_AUX_ACTION_NONE;
    }
    last_running_input = nullptr;
    return result;
}

void HNS_Aux_Inputs_PennStyle::fSetInput(const int &aux_channel, const HNS_NTCIP_MessageIDCode &msg_id, const bool &enabled, const int &duration)
{
    if(aux_channel >= 0 && static_cast<size_t>(aux_channel) < f_input_channels.size())
    {
        if(f_input_channels[aux_channel] != nullptr)
        {
            f_input_channels[aux_channel]->fSetDuration(duration);
            f_input_channels[aux_channel]->fSetEnabled(enabled);
            f_input_channels[aux_channel]->fSetMessage(msg_id);
        }
    }
}
