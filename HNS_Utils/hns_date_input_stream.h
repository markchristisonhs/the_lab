#ifndef HNS_DATE_INPUT_STREAM_H
#define HNS_DATE_INPUT_STREAM_H

#include <string>

class HNS_Date_Input_Stream
{
public:
    HNS_Date_Input_Stream();

    std::string fGetString(const bool &blink_on = true);

    bool fAdd(const char &digit);

    void fBackspace();

    void fClear();

    void fResetEdit();

    void fSetDate(const int &year, const int &month, const int &day);

    int fGetYear();
    int fGetMonth();
    int fGetDay();
private:
    bool fProcessStateMachine(const char &digit, const bool &backspace = false);

    std::string f_digits;
    int f_state;
};

#endif // HNS_DATE_INPUT_STREAM_H
