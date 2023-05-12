#ifndef HNS_AUX_INPUT_H
#define HNS_AUX_INPUT_H

#include "HNS_NTCIP_Datatypes.h"

#include <cstdint>
#include <vector>

enum
{
    HNS_INPUT_STYLE_NONE = -1,
    HNS_INPUT_STYLE_DEFAULT = 0
};

enum
{
    HNS_INPUT_NO_EDGE = -1,
    HNS_INPUT_RISING_EDGE = 0,
    HNS_INPUT_FALLING_EDGE = 1
};

class HNS_Aux_Input
{
public:
    virtual void fSetPortNumber(const int &port_number) = 0;
    virtual int fGetPortNumber() const = 0;

    virtual void fSetEnabled(const unsigned char &enabled) = 0;
    virtual unsigned char fGetEnabled() const = 0;
    virtual void fSetDuration(const int &duration) = 0;
    virtual int fGetDuration() const = 0;
    virtual void fSetMsg(const HNS_NTCIP_MessageIDCode &msg) = 0;
    virtual HNS_NTCIP_MessageIDCode fGetMsg() const = 0;

    virtual int fGetStyle() = 0;

    virtual bool fIsAuxDisplayed(const int64_t &time_in, HNS_NTCIP_MessageIDCode &msg) = 0;
    virtual int fSetInput(const int &val) = 0;
    virtual void fCancel() = 0;

    virtual ~HNS_Aux_Input() {}
};

class HNS_Aux_Input_Basic_Style : public HNS_Aux_Input
{
public:
    HNS_Aux_Input_Basic_Style();
    HNS_Aux_Input_Basic_Style(const int &port_number):f_port_number(port_number) {}

    void fSetPortNumber(const int &port_number) {f_port_number = port_number;}
    int fGetPortNumber() const {return f_port_number;}

    void fSetEnabled(const unsigned char &enabled);
    unsigned char fGetEnabled() const;
    void fSetDuration(const int &duration);
    int fGetDuration() const;
    void fSetMsg(const HNS_NTCIP_MessageIDCode &msg);
    HNS_NTCIP_MessageIDCode fGetMsg() const;

    int fGetStyle() {return HNS_INPUT_STYLE_DEFAULT;}
    bool fIsAuxDisplayed(const int64_t &time_in, HNS_NTCIP_MessageIDCode &msg);

    int fSetInput(const int &val);
    void fCancel();
private:
    void fStateMachine(const int &action = -1, const int64_t &time_in = 0);

    int f_port_number;
    int f_val, f_last_val;
    bool f_enabled;
    int f_duration;
    int f_state;
    HNS_NTCIP_MessageIDCode f_msg;

    int64_t f_delay;
    int64_t f_time_activated;
};

class HNS_Aux_Inputs
{
public:
    void fAddPortNumber(const int &port_number, const size_t &aux_channel, const int &algorithm);

    void fSetEnables(const std::vector<unsigned char> &enables);
    void fSetEnabled(const unsigned char &enabled, const size_t &aux_channel);
    unsigned char fGetEnabled(const size_t &aux_channel) const;
    std::vector<unsigned char> fGetEnables() const;

    void fSetDurations(const std::vector<int> durations);
    void fSetDuration(const int &duration, const size_t &aux_channel);
    int fGetDuration(const size_t &aux_channel) const;
    std::vector<int> fGetDurations() const;

    void fSetMsgs(const std::vector<HNS_NTCIP_MessageIDCode> &msgs);
    void fSetMsg(const HNS_NTCIP_MessageIDCode &msg, const size_t &aux_channel);
    HNS_NTCIP_MessageIDCode fGetMsg(const size_t &aux_channel) const;
    std::vector<HNS_NTCIP_MessageIDCode> fGetMsgs() const;

    int fGetStyle(const size_t &aux_channel);

    bool fIsAuxDisplayed(const int64_t &time_in, HNS_NTCIP_MessageActivationCode *msg = nullptr, bool *turn_on_off = nullptr);
    void fSetInput(const int &val, const int &port_number, const HNS_NTCIP_MessageActivationCode &return_msg);
    void fCancel();

    ~HNS_Aux_Inputs();
private:
    std::vector<HNS_Aux_Input *> f_inputs;
    HNS_NTCIP_MessageActivationCode f_return_act;
};

#endif // HNS_AUX_INPUT_H
