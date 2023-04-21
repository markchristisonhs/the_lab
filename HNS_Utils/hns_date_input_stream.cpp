#include "hns_date_input_stream.h"

using namespace std;

enum
{
    STATE_DIGIT1,
    STATE_DIGIT2A,
    STATE_DIGIT2B,
    STATE_DIGIT3,
    STATE_DIGIT4,
    STATE_DIGIT5,
    STATE_DIGIT6,
    STATE_DIGIT7,
    STATE_DIGIT8
};

HNS_Date_Input_Stream::HNS_Date_Input_Stream():
    f_digits(8,'0')
  , f_state(STATE_DIGIT1)
{

}

string HNS_Date_Input_Stream::fGetString(const bool &blink_on)
{
    string result;

    result += f_digits[0];
    result += f_digits[1];
    result += "/";
    result += f_digits[2];
    result += f_digits[3];
    result += "/";
    result += f_digits[4];
    result += f_digits[5];
    result += f_digits[6];
    result += f_digits[7];

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
        case STATE_DIGIT5:
            result[6] = ' ';
            break;
        case STATE_DIGIT6:
            result[7] = ' ';
            break;
        case STATE_DIGIT7:
            result[8] = ' ';
            break;
        case STATE_DIGIT8:
            result[9] = ' ';
            break;
        default:
            break;
        }
    }

    return result;
}

bool HNS_Date_Input_Stream::fAdd(const char &digit)
{
    return fProcessStateMachine(digit);
}

void HNS_Date_Input_Stream::fBackspace()
{
    fProcessStateMachine('0',true);
}

void HNS_Date_Input_Stream::fClear()
{
    fResetEdit();
}

void HNS_Date_Input_Stream::fResetEdit()
{
    f_digits = string(8,'0');
    f_state = STATE_DIGIT1;
}

void HNS_Date_Input_Stream::fSetDate(const int &year, const int &month, const int &day)
{
    if((year >= 0 && year <= 9999)
            && (month >= 0 && month <= 11)
            && (day >= 0 && day <= 31))
    {
        f_digits[0] = (month / 10) + 0x30;
        f_digits[1] = (month % 10) + 0x30;

        f_digits[2] = (day / 10) + 0x30;
        f_digits[3] = (day % 10) + 0x30;

        f_digits[4] = (year / 1000) + 0x30;
        f_digits[5] = ((year / 100) % 10) + 0x30;
        f_digits[6] = ((year / 10) % 10) + 0x30;
        f_digits[7] = (year % 10) + 0x30;
    }
}

int HNS_Date_Input_Stream::fGetYear()
{
    return ((f_digits[4] - 0x30) * 1000) + ((f_digits[5] - 0x30) * 100) + ((f_digits[6] - 0x30) * 10) + (f_digits[7] - 0x30);
}

int HNS_Date_Input_Stream::fGetMonth()
{
    return ((f_digits[0] - 0x30) * 10) + (f_digits[1] - 0x30);
}

int HNS_Date_Input_Stream::fGetDay()
{
    return ((f_digits[2] - 0x30) * 10) + (f_digits[3] - 0x30);
}

bool HNS_Date_Input_Stream::fProcessStateMachine(const char &digit, const bool &backspace)
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
        else if(digit >= '0' && digit <= '3')
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
            f_state = STATE_DIGIT5;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT5:
        if(backspace)
        {
            f_digits[4] = '0';
            f_state = STATE_DIGIT4;
        }
        else if(digit >= '0' && digit <= '9')
        {
            f_digits[4] = digit;
            f_state = STATE_DIGIT6;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT6:
        if(backspace)
        {
            f_digits[5] = '0';
            f_state = STATE_DIGIT5;
        }
        else if(digit >= '0' && digit <= '9')
        {
            f_digits[5] = digit;
            f_state = STATE_DIGIT7;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT7:
        if(backspace)
        {
            f_digits[6] = '0';
            f_state = STATE_DIGIT6;
        }
        else if(digit >= '0' && digit <= '9')
        {
            f_digits[6] = digit;
            f_state = STATE_DIGIT8;
        }
        else
        {
            success = false;
        }
        break;
    case STATE_DIGIT8:
        if(backspace)
        {
            f_digits[7] = '0';
            f_state = STATE_DIGIT7;
        }
        else if(digit >= '0' && digit <= '9')
        {
            f_digits[7] = digit;
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
