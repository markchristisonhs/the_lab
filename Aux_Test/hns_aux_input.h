#ifndef HNS_AUX_INPUT_H
#define HNS_AUX_INPUT_H

#include "HNS_NTCIP_Datatypes.h"

#include <cstdint>
#include <vector>
#include <deque>

enum
{
    HNS_INPUT_STYLE_NONE = -1,
    HNS_INPUT_STYLE_FALLING_TIMER = 0
};

enum
{
    HNS_INPUTS_STYLE_NONE = -1,
    HNS_INPUTS_STYLE_PENN = 0
};

enum
{
    HNS_INPUT_NO_EDGE = 0,
    HNS_INPUT_RISING_EDGE = 1,
    HNS_INPUT_FALLING_EDGE = -1
};

enum
{
    HNS_AUX_ACTION_NONE = 0,
    HNS_AUX_ACTION_START,
    HNS_AUX_ACTION_STOP,
};

class HNS_Aux_Inputs
{
public:
    virtual void fRecvInput(const int &val, const int64_t &time, const int &port_number, const HNS_NTCIP_MessageActivationCode &running_code) = 0;
    virtual void fCancel() = 0;
    virtual bool fIsRunning(const int64_t &time, HNS_NTCIP_MessageActivationCode *result_code, int &action) = 0;
    virtual int fGetType() = 0;

    virtual void fSetInput(const int &aux_channel, const HNS_NTCIP_MessageIDCode &msg_id, const bool &enabled, const int &duration) = 0;

    virtual ~HNS_Aux_Inputs() {}
};

class HNS_Aux_Input
{
public:
    virtual int fRecvInput(const int &val, const int64_t &time) = 0;
    virtual void fCancel() = 0;
    virtual bool fIsRunning(const int64_t &time, HNS_NTCIP_MessageIDCode *result_id ) = 0;
    virtual int fGetType() = 0;

    virtual void fSetDuration(const int &duration) = 0;
    virtual void fSetEnabled(const bool &enabled) = 0;
    virtual void fSetMessage(const HNS_NTCIP_MessageIDCode &msg_code) = 0;

    virtual int fGetPortNumber() = 0;

    virtual ~HNS_Aux_Input() {}
};

class HNS_Aux_Input_Falling_Timer : public HNS_Aux_Input
{
public:
    HNS_Aux_Input_Falling_Timer();
    HNS_Aux_Input_Falling_Timer(const int &port_number);

    int fRecvInput(const int &val, const int64_t &time);
    void fCancel();
    bool fIsRunning(const int64_t &time, HNS_NTCIP_MessageIDCode *result_id);
    int fGetType() {return HNS_INPUT_FALLING_EDGE;}

    void fSetDuration(const int &duration);
    void fSetEnabled(const bool &enabled);
    void fSetMessage(const HNS_NTCIP_MessageIDCode &msg_code);

    int fGetPortNumber() {return f_port_number;}

private:
    int f_port_number;
    int f_duration;
    bool f_enabled;
    HNS_NTCIP_MessageIDCode f_msg_id;

    int f_current_val;
    bool f_running;

    int64_t f_edge_time;
};

class HNS_Aux_Inputs_PennStyle : public HNS_Aux_Inputs
{
public:
    HNS_Aux_Inputs_PennStyle();
    HNS_Aux_Inputs_PennStyle(const std::vector<int> &port_numbers, const int &algorithm);

    void fRecvInput(const int &val, const int64_t &time, const int &port_number, const HNS_NTCIP_MessageActivationCode &running_code);
    void fCancel();
    bool fIsRunning(const int64_t &time, HNS_NTCIP_MessageActivationCode *result_code, int &action);
    int fGetType() {return HNS_INPUTS_STYLE_PENN;}

    void fSetInput(const int &aux_channel, const HNS_NTCIP_MessageIDCode &msg_id, const bool &enabled, const int &duration);

private:
    HNS_NTCIP_MessageActivationCode f_ret_code;

    std::vector<HNS_Aux_Input *> f_input_channels;
    std::deque<HNS_Aux_Input *> f_running_queue;
};

#endif // HNS_AUX_INPUT_H
