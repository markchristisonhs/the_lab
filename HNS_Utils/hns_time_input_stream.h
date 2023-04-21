#ifndef HNS_TIME_INPUT_STREAM_H
#define HNS_TIME_INPUT_STREAM_H

#include <string>

class HNS_Time_Input_Stream
{
public:
    HNS_Time_Input_Stream();

    std::string fGetString(const bool &blink_on = true);

    bool fAdd(const char &digit);

    void fSetAM_PM(const bool &is_pm);
    void fToggleAM_PM();

    void fBackspace();

    void fClear();

    void fResetEdit();

    void fSetTime(const int &hour, const int &minute, const bool &is_pm);

    int fGetHour();
    int fGetHour24();
    int fGetMinute();
    bool fGetIsPM();
private:
    bool fProcessStateMachine(const char &digit, const bool &backspace = false);
    std::string f_digits;
    bool f_is_pm;

    int f_state;
};

#endif // HNS_TIME_INPUT_STREAM_H
