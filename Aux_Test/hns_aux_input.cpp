#include "hns_aux_input.h"

using namespace std;

enum
{
    AUX_ACTION_NONE = -1,
    AUX_ACTION_CANCEL = 0,
    AUX_ACTION_RISING_EDGE = 1,
    AUX_ACTION_FALLING_EDGE = 2
};

HNS_Aux_Input_Basic_Style::HNS_Aux_Input_Basic_Style():
    f_port_number(-1)
  , f_val(-1)
  , f_last_val(-1)
  , f_enabled(false)
  , f_state(-1)
  , f_delay(0)
  , f_time_activated(0)
{

}

void HNS_Aux_Input_Basic_Style::fSetEnabled(const unsigned char &enabled)
{
    f_enabled = (enabled != 0);
}

unsigned char HNS_Aux_Input_Basic_Style::fGetEnabled() const
{
    return f_enabled ? 1 : 0;
}

void HNS_Aux_Input_Basic_Style::fSetDuration(const int &duration)
{
    f_duration = duration;
}

int HNS_Aux_Input_Basic_Style::fGetDuration() const
{
    return f_duration;
}

void HNS_Aux_Input_Basic_Style::fSetMsg(const HNS_NTCIP_MessageIDCode &msg)
{
    f_msg = msg;
}

HNS_NTCIP_MessageIDCode HNS_Aux_Input_Basic_Style::fGetMsg() const
{
    return f_msg;
}

bool HNS_Aux_Input_Basic_Style::fIsAuxDisplayed(const int64_t &/*time_in*/, HNS_NTCIP_MessageIDCode &/*msg*/)
{
    return false;
}

int HNS_Aux_Input_Basic_Style::fSetInput(const int &/*val*/)
{
    return HNS_INPUT_NO_EDGE;
}

void HNS_Aux_Input_Basic_Style::fCancel()
{

}

void HNS_Aux_Input_Basic_Style::fStateMachine(const int &/*action*/, const int64_t &/*time_in*/)
{

}

void HNS_Aux_Inputs::fAddPortNumber(const int &port_number, const size_t &aux_channel, const int &algorithm)
{
    bool duplicate = false;

    for(size_t ui=0;ui<f_inputs.size();ui++)
    {
        if(f_inputs[ui] != nullptr)
        {
            if(f_inputs[ui]->fGetPortNumber() == port_number)
            {
                duplicate = true;
                break;
            }
        }
    }

    if(!duplicate)
    {
        if(aux_channel >= f_inputs.size())
        {
            f_inputs.resize(aux_channel+1,nullptr);
        }

        if(algorithm == HNS_INPUT_STYLE_DEFAULT)
        {
            f_inputs[aux_channel] = (HNS_Aux_Input *)(new HNS_Aux_Input_Basic_Style(port_number));
        }
    }
}

void HNS_Aux_Inputs::fSetEnables(const vector<unsigned char> &enables)
{
    size_t ui_limit = min(enables.size(),f_inputs.size());

    for(size_t ui=0;ui<ui_limit;ui++)
    {
        fSetEnabled(enables[ui],ui);
    }
}

void HNS_Aux_Inputs::fSetEnabled(const unsigned char &enabled, const size_t &aux_channel)
{
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            f_inputs[aux_channel]->fSetEnabled(enabled);
        }
    }
}

unsigned char HNS_Aux_Inputs::fGetEnabled(const size_t &aux_channel) const
{
    unsigned char result = 0;
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            result = f_inputs[aux_channel]->fGetEnabled();
        }
    }
    return result;
}

vector<unsigned char> HNS_Aux_Inputs::fGetEnables() const
{
    vector<unsigned char> result(f_inputs.size(),0);
    for(size_t ui=0;ui<result.size();ui++)
    {
        result[ui] = fGetEnabled(ui);
    }
    return result;
}

void HNS_Aux_Inputs::fSetDurations(const vector<int> durations)
{
    size_t ui_limit = min(durations.size(),f_inputs.size());

    for(size_t ui=0;ui<ui_limit;ui++)
    {
        fSetDuration(durations[ui],ui);
    }
}

void HNS_Aux_Inputs::fSetDuration(const int &duration, const size_t &aux_channel)
{
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            f_inputs[aux_channel]->fSetDuration(duration);
        }
    }
}

int HNS_Aux_Inputs::fGetDuration(const size_t &aux_channel) const
{
    int result = 0;
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            result = f_inputs[aux_channel]->fGetDuration();
        }
    }
    return result;
}

vector<int> HNS_Aux_Inputs::fGetDurations() const
{
    vector<int> result(f_inputs.size(),0);
    for(size_t ui=0;ui<result.size();ui++)
    {
        result[ui] = fGetDuration(ui);
    }
    return result;
}

void HNS_Aux_Inputs::fSetMsgs(const std::vector<HNS_NTCIP_MessageIDCode> &msgs)
{
    size_t ui_limit = min(msgs.size(),f_inputs.size());

    for(size_t ui=0;ui<ui_limit;ui++)
    {
        fSetMsg(msgs[ui],ui);
    }
}

void HNS_Aux_Inputs::fSetMsg(const HNS_NTCIP_MessageIDCode &msg, const size_t &aux_channel)
{
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            f_inputs[aux_channel]->fSetMsg(msg);
        }
    }
}

HNS_NTCIP_MessageIDCode HNS_Aux_Inputs::fGetMsg(const size_t &aux_channel) const
{
    HNS_NTCIP_MessageIDCode result;
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            result = f_inputs[aux_channel]->fGetMsg();
        }
    }
    return result;
}

vector<HNS_NTCIP_MessageIDCode> HNS_Aux_Inputs::fGetMsgs() const
{
    vector<HNS_NTCIP_MessageIDCode> result(f_inputs.size(),HNS_NTCIP_MessageIDCode());
    for(size_t ui=0;ui<result.size();ui++)
    {
        result[ui] = fGetMsg(ui);
    }
    return result;
}

int HNS_Aux_Inputs::fGetStyle(const size_t &aux_channel)
{
    int result = HNS_INPUT_STYLE_NONE;
    if(aux_channel < f_inputs.size())
    {
        if(f_inputs[aux_channel] != nullptr)
        {
            result = f_inputs[aux_channel]->fGetStyle();
        }
    }
    return result;
}

bool HNS_Aux_Inputs::fIsAuxDisplayed(const int64_t &time_in, HNS_NTCIP_MessageActivationCode *msg, bool *turn_on_off)
{
    return false;
}

void HNS_Aux_Inputs::fSetInput(const int &val, const int &port_number, const HNS_NTCIP_MessageActivationCode &return_msg)
{
    //This really won't work for multiple inputs, but for now I just want to get a single input working.
    for(size_t ui=0;ui<=f_inputs.size();ui++)
    {
        if(f_inputs[ui] != nullptr)
        {
            if(f_inputs[ui]->fGetPortNumber() == port_number)
            {
                f_inputs[ui]->fSetInput(val);
                break;
            }
        }
    }
}

void HNS_Aux_Inputs::fCancel()
{
    for(size_t ui=0;ui<f_inputs.size();ui++)
    {
        if(f_inputs[ui] != nullptr)
        {
            f_inputs[ui]->fCancel();
        }
    }
}

HNS_Aux_Inputs::~HNS_Aux_Inputs()
{
    for(size_t ui=0;ui<f_inputs.size();ui++)
    {
        if(f_inputs[ui] != nullptr)
        {
            delete f_inputs[ui];
            f_inputs[ui] = nullptr;
        }
    }
}
