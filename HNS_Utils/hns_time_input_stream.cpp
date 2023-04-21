#include "hns_time_input_stream.h"

using namespace std;

enum
{
    STATE_DIGIT1,
    STATE_DIGIT2A,
    STATE_DIGIT2B,
    STATE_DIGIT3,
    STATE_DIGIT4
};

HNS_Time_Input_Stream::HNS_Time_Input_Stream():
    f_digits(4,'0')
  , f_is_pm(true)
  , f_state(STATE_DIGIT1)
{
    f_digits[0] = '1';
    f_digits[1] = '2';
}

string HNS_Time_Input_Stream::fGetString(const bool &blink_on)
{
    string result;

    result += f_digits[0];
    result += f_digits[1];
    result += ":";
    result += f_digits[2];
    result += f_digits[3];

    if(f_is_pm)
    {
        result += " PM";
    }
    else
    {
        result += " AM";
    }

    if(!blink_on)
    {
        switch(f_state)
        {
        case STATE_DIGIT1:
            result[0] = ' ';
            break;
        case STATE_DIGIT2A:
        case STATE_DIGIT2B:
            result[1] = ' ';
            break;
        case STATE_DIGIT3:
            result[3] = ' ';
            break;
        case STATE_DIGIT4:
            result[4] = ' ';
            break;
        default:
            break;
        }
    }

    return result;
}

bool HNS_Time_Input_Stream::fAdd(const char &digit)
{
    return fProcessStateMachine(digit);
}

void HNS_Time_Input_Stream::fSetAM_PM(const bool &is_pm)
{
    f_is_pm = is_pm;
}

void HNS_Time_Input_Stream::fToggleAM_PM()
{
    f_is_pm = !f_is_pm;
}

void HNS_Time_Input_Stream::fBackspace()
{
    fProcessStateMachine('0',true);
}

void HNS_Time_Input_Stream::fClear()
{
    f_digits[0] = '1';
    f_digits[1] = '2';
    f_digits[2] = '0';
    f_digits[3] = '0';
    f_is_pm = true;
    f_state = STATE_DIGIT1;
}

void HNS_Time_Input_Stream::fResetEdit()
{
    f_state = STATE_DIGIT1;
}

void HNS_Time_Input_Stream::fSetTime(const int &hour, const int &minute, const bool &is_pm)
{
    if((hour >= 0 && hour <= 11)
    && (minute >= 0 && minute <= 59))
    {
        f_digits[0] = (hour / 10) + 0x30;
        f_digits[1] = (hour % 10) + 0x30;
        f_digits[2] = (minute / 10) + 0x30;
        f_digits[3] = (minute % 10) + 0x30;

        f_is_pm = is_pm;
    }
}

int HNS_Time_Input_Stream::fGetHour()
{
    return ((f_digits[0] - 0x30) * 10) + (f_digits[1] - 0x30);
}

int HNS_Time_Input_Stream::fGetHour24()
{
    int result;
    if(fGetHour() == 12)
    {
        result = f_is_pm ? 12 : 0;
    }
    else
    {
        result = fGetHour() + (f_is_pm ? 12 : 0);
    }

    return result;
}

int HNS_Time_Input_Stream::fGetMinute()
{
    return ((f_digits[2] - 0x30) * 10) + (f_digits[3] - 0x30);
}

bool HNS_Time_Input_Stream::fGetIsPM()
{
    return f_is_pm;
}

bool HNS_Time_Input_Stream::fProcessStateMachine(const char &digit, const bool &backspace)
{
    bool success = true;

    switch(f_state)
    {
    case STATE_DIGIT1:
        if(backspace)
        {
            f_digits[0] = '0';
        }
        else if(digit == '0')
        {
            f_digits[0] = digit;
            f_state = STATE_DIGIT2A;
        }
        else if(digit == '1')
        {
            f_digits[0] = digit;
            f_state = STATE_DIGIT2B;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT2A:
        if(backspace)
        {
            f_digits[1] = '0';
            f_state = STATE_DIGIT1;
        }
        else if(digit >= '1' && digit <= '9')
        {
            f_digits[1] = digit;
            f_state = STATE_DIGIT3;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT2B:
        if(backspace)
        {
            f_digits[1] = '0';
            f_state = STATE_DIGIT1;
        }
        else if(digit >= '0' && digit <= '2')
        {
            f_digits[1] = digit;
            f_state = STATE_DIGIT3;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT3:
        if(backspace)
        {
            f_digits[2] = '0';
            if(f_digits[0] == '0')
            {
                f_state = STATE_DIGIT2A;
            }
            else
            {
                f_state = STATE_DIGIT2B;
            }
        }
        else if(digit >= '0' && digit <= '5')
        {
            f_digits[2] = digit;
            f_state = STATE_DIGIT4;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT4:
        if(backspace)
        {
            f_digits[3] = '0';
            f_state = STATE_DIGIT3;
        }
        else if(digit >= '0' && digit <= '9')
        {
            f_digits[3] = digit;
        }
        else
        {
            success = false;
        }
        break;
    default:
        success = false;
        f_state = STATE_DIGIT1;
        break;
    }

    return success;
}
