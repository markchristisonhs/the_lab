/*
 * message.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: mchristiso
 */

#include "stdafx.h"

#include "message.h"
#include "utilities.h"

#include <ctype.h>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;


type_justification_line HNS_Message2::f_default_line_justification = HNS_JUSTIFICATION_LINE_CENTER;
type_justification_page HNS_Message2::f_default_page_justification = HNS_JUSTIFICATION_PAGE_CENTER;
double HNS_Message2::f_default_page_time_on = 2.0;
double HNS_Message2::f_default_page_time_off = 0.0;
unsigned int HNS_Message2::f_default_flash_time_on = 10;
unsigned int HNS_Message2::f_default_flash_time_off = 10;
int HNS_Message2::f_default_font = 1;
HNS_SignBoard_Info HNS_Message2::f_default_signboard_info;

//Remember, time is 1000ths of a second, and flash_info stores in 10ths of a second
bool IsFlashOn(const int64_t &time, const HNS_Flashing_Text &flash_info)
{
    bool isTextOn = false;

    unsigned int flash_period = (flash_info.fGetFlashOn() + flash_info.fGetFlashOff()) * 100;

    if(flash_period > 0)
    {
        int64_t adjusted_time = time % flash_period;

        if(flash_info.fGetOnFirst())
        {
            isTextOn = adjusted_time < (flash_info.fGetFlashOn() * 100);
        }
        else
        {
            isTextOn = adjusted_time >= (flash_info.fGetFlashOff() * 100);
        }
    }
    else
    {
        isTextOn = true;
    }

    return isTextOn;
}

//Copy pasted from a blog post  Should remove all duplicates
void remove(vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

vector<int> MergeFontsUsedLists(const vector<int> &font_list1, const vector<int> &font_list2)
{
    vector<int> result(font_list1.size()+font_list2.size(),0);

    merge(font_list1.begin(),font_list1.end(),font_list2.begin(),font_list2.end(),result.begin());

    remove(result);

    return result;
}

vector<int> AddFontToFontUsedList(const vector<int> &font_list, const int &font_no)
{
    vector<int> result = font_list, temp_vec;

    sort(result.begin(), result.end());

    temp_vec = result;
    if(result.empty())
    {
        result.insert(result.begin(), font_no);
    }
    else
    {
        for(size_t i=0;i<temp_vec.size();i++)
        {
            if(font_no < temp_vec[i])
            {
                result.insert(result.begin()+i, font_no);
                break;
            }
            else if(font_no == result[i])
            {
                break;
            }
        }
    }

    return result;
}

enum
{
	HNS_MULTI_PARSER_STATE_TEXT = 0,
	HNS_MULTI_PARSER_STATE_TAG,
    HNS_MULTI_PARSER_STATE_CLOSING_CANDIDATE,
	HNS_MULTI_PARSER_STATE_TAG_C,
	HNS_MULTI_PARSER_STATE_TAG_F,
	HNS_MULTI_PARSER_STATE_TAG_G,
	HNS_MULTI_PARSER_STATE_TAG_H,
	HNS_MULTI_PARSER_STATE_TAG_J,
	HNS_MULTI_PARSER_STATE_TAG_M,
	HNS_MULTI_PARSER_STATE_tAG_N,
	HNS_MULTI_PARSER_STATE_TAG_P,
	HNS_MULTI_PARSER_STATE_TAG_S,
	HNS_MULTI_PARSER_STATE_TAG_T,
	HNS_MULTI_PARSER_STATE_TAG_FW_SLASH
};

HNS_Flashing_Text::HNS_Flashing_Text():
    f_flash_on(0),
    f_flash_off(0),
    f_on_first(true)
{

}

HNS_Flashing_Text::HNS_Flashing_Text(const unsigned int &flash_on, const unsigned int &flash_off, const bool &on_first):
    f_flash_on(flash_on),
    f_flash_off(flash_off),
    f_on_first(on_first)
{

}

unsigned int &HNS_Flashing_Text::fFlashOn()
{
    return f_flash_on;
}

void HNS_Flashing_Text::fSetFlashOn(const unsigned int &flash_on)
{
    f_flash_on = flash_on;
}

unsigned int HNS_Flashing_Text::fGetFlashOn() const
{
    return f_flash_on;
}

unsigned int &HNS_Flashing_Text::fFlashOff()
{
    return f_flash_off;
}

void HNS_Flashing_Text::fSetFlashOff(const unsigned int &flash_off)
{
    f_flash_off = flash_off;
}

unsigned int HNS_Flashing_Text::fGetFlashOff() const
{
    return f_flash_off;
}

bool &HNS_Flashing_Text::fOnFirst()
{
    return f_on_first;
}

void HNS_Flashing_Text::fSetOnFirst(const bool &on_first)
{
    f_on_first = on_first;
}

bool HNS_Flashing_Text::fGetOnFirst() const
{
    return f_on_first;
}

HNS_Message2::HNS_Message2():
    f_current_page_time_on(f_default_page_time_on)
  , f_current_page_time_off(f_default_page_time_off)
  , f_current_line_justification(f_default_line_justification)
  , f_current_page_justification(f_default_page_justification)
  , f_current_font(f_default_font)
  //Line and character spacing are wrong here.  These should be the default font's but this is not accessible here
  //So I am initializing to 1 here to preserve RAII behavior
  , f_current_line_spacing(1)
  , f_current_char_spacing(1)
  , f_current_flash_info(f_default_flash_time_on,f_default_flash_time_off,true)
  , f_current_flashing_state(false)
{

}

HNS_Message2::HNS_Message2(const HNS_SignBoard_Info &):
    f_current_page_time_on(f_default_page_time_on)
  , f_current_page_time_off(f_default_page_time_off)
  , f_current_line_justification(f_default_line_justification)
  , f_current_page_justification(f_default_page_justification)
  , f_current_font(f_default_font)
  , f_current_line_spacing(1)
  , f_current_char_spacing(1)
  , f_current_flash_info(f_default_flash_time_on,f_default_flash_time_off,true)
  , f_current_flashing_state(false)
{

}

int HNS_Message2::fSetMULTI(std::string &multi_string, const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const HNS_Field_Data *field_data, type_multi_syntax_error *multi_error, bool *too_tall, size_t max_num_pages, int *error_position)
{
    type_multi_syntax_error my_error = HNS_MULTI_SYNTAX_ERROR_NONE;
    int error = HNS_MULTI_PARSER_NO_ERROR;
    int fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
    int error_pos = -1;
    string text;
    string temp_string;
    int itemp,itemp_x,itemp_y;
    bool fail = false;
    //true is pm, false is am
    bool am_pm = false;
    vector<int> numbers;
    int temp_error;
    bool on_first_temp;
    bool use_default_flash_on = false;
    bool use_default_flash_off = false;
    HNS_Message_Element2 temp_element(this);
    f_multi = multi_string;
    ssize_t index;
    bool first;
    stringstream ss;
    tm *current_time = nullptr;
    int hour = 12, minute = 30, day = 0, mday = 1, month = 0, year = 120;

    bool temp_too_tall = false;
    type_hns_signboard_error sign_brd_error = HNS_SGNBRD_ERROR_NONE;

    if(fonts == nullptr)
    {
        fail = true;
        error = HNS_MULTI_PARSER_SYNTAX_ERROR;
        my_error = HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED;
        error_pos = 0;
    }
    else
    {
        if((f_current_font < 0) || (static_cast<size_t>(f_current_font) > fonts->size()))
        {
            fail = true;
            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
            my_error = HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED;
            error_pos = 0;
        }
        //current_font is a valid font, populate line and character spacing from the current_font (should be default at this point)
        else
        {
            f_current_line_spacing = fonts->at(f_current_font).fGetLineSpacing();
            f_current_char_spacing = fonts->at(f_current_font).fGetCharSpacing();
        }
    }

    f_pages.clear();

    //there will always be one page, even if its blank
    fAddPage();

    for(string::iterator character = multi_string.begin(); character != multi_string.end() && !fail; ++character)
    {
        switch(fsm_state)
        {
        case HNS_MULTI_PARSER_STATE_TEXT:
            if(*character == '[')
            {
                fsm_state = HNS_MULTI_PARSER_STATE_TAG;
            }
            else if(*character == ']')
            {
                fsm_state = HNS_MULTI_PARSER_STATE_CLOSING_CANDIDATE;
                break;
            }
            else
            {
                text += *character;
            }
            break;
        case HNS_MULTI_PARSER_STATE_TAG:
            switch(toupper(*character))
            {
            case 'C':
                ++character;
                if(character != multi_string.end())
                {
                    if(toupper(*character) == 'B')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while(isdigit(*character) && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                itemp = numbers[0];
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else if(toupper(*character) == 'F')
                    {
                        index = -1;
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character) || *character == ',') && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                itemp = numbers[0];
                            }
                            if(numbers.size() == 3)
                            {
                                //itemp_r = numbers[0];
                                //itemp_g = numbers[1];
                                //itemp_b = numbers[2];
                            }
                            fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else if(toupper(*character) == 'R')
                    {
                        index = -1;
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character) || *character == ',') && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 5)
                            {
                                itemp = numbers[4];
                            }
                            if(numbers.size() == 7)
                            {
                                itemp_x = numbers[0];
                            }
                            fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'F':
                ++character;
                if(character != multi_string.end())
                {
                    //Flashing tag
                    if(toupper(*character) == 'L')
                    {
                        use_default_flash_on = false;
                        use_default_flash_off = false;
                        ++character;
                        if(toupper(*character) == 'T')
                        {
                            on_first_temp = true;
                        }
                        else if(toupper(*character) == 'O')
                        {
                            on_first_temp = false;
                        }
                        //neither t nor o are specified, so use defaults
                        else if(*character == ']')
                        {
                            use_default_flash_on = true;
                            use_default_flash_off = true;
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                        if(!fail)
                        {
                            if(use_default_flash_off && use_default_flash_on)
                            {
                                f_current_flash_info.fFlashOn() = f_default_flash_time_on;
                                f_current_flash_info.fFlashOff() = f_default_flash_time_off;
                                f_current_flash_info.fOnFirst() = true;

                                //already existing text add it as an element to the page.
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element);
                                    text = "";
                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }
                                f_current_flashing_state = true;
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                temp_string = "";
                                do
                                {
                                    ++character;
                                    if(isdigit(*character))
                                    {
                                        temp_string += *character;
                                    }
                                }while((isdigit(*character)) && character != multi_string.end());

                                if(toupper(*character) == (on_first_temp ? 'O' : 'T'))
                                {
                                    temp_string += ",";
                                    do
                                    {
                                        ++character;
                                        if(isdigit(*character))
                                        {
                                            temp_string += *character;
                                        }
                                    }while((isdigit(*character)) && character != multi_string.end());
                                    if(*character == ']')
                                    {
                                        //already existing text add it as an element to the page.
                                        if(!text.empty())
                                        {
                                            if(f_pages.size() == 0)
                                            {
                                                //this is actually the first page.
                                                fAddPage();
                                            }
                                            temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                            sign_brd_error = fAddElementToPage(temp_element);
                                            text = "";
                                            if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                            {
                                                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                                {
                                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                                    {
                                                        temp_too_tall = true;
                                                    }
                                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                                    my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                                    error_pos = character - multi_string.begin() - 1;
                                                    fail = true;
                                                }
                                            }
                                        }

                                        numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                                        if(on_first_temp)
                                        {
                                            if(numbers.size() > 0)
                                            {
                                                f_current_flash_info.fFlashOn() = numbers[0];
                                            }
                                            else
                                            {
                                                f_current_flash_info.fFlashOn() = f_default_flash_time_on;
                                            }

                                            if(numbers.size() > 1)
                                            {
                                                f_current_flash_info.fFlashOff() = numbers[1];
                                            }
                                            else
                                            {
                                                f_current_flash_info.fFlashOff() = f_default_flash_time_off;
                                            }
                                            f_current_flash_info.fSetOnFirst(true);
                                        }
                                        else
                                        {
                                            if(numbers.size() > 1)
                                            {
                                                f_current_flash_info.fFlashOn() = numbers[1];
                                            }
                                            else
                                            {
                                                f_current_flash_info.fFlashOn() = f_default_flash_time_on;
                                            }

                                            if(numbers.size() > 0)
                                            {
                                                f_current_flash_info.fFlashOff() = numbers[0];
                                            }
                                            else
                                            {
                                                f_current_flash_info.fFlashOff() = f_default_flash_time_off;
                                            }
                                            f_current_flash_info.fSetOnFirst(false);
                                        }
                                        if(f_current_flash_info.fGetFlashOff() > 0 && f_current_flash_info.fGetFlashOn() > 0)
                                        {
                                            f_current_flashing_state = false;
                                        }
                                        fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                                    }
                                    else
                                    {
                                        error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                        error_pos = character - multi_string.begin() - 1;
                                        fail = true;
                                    }
                                }
                                else
                                {
                                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                        }
                    }
                    //font tag
                    else if(toupper(*character) == 'O')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character) || *character == ',') && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                //already existing text at another font, add it as an element to the page.
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element);

                                    text = "";

                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }
                                if((numbers[0] < 0) || (static_cast<size_t>(numbers[0]) > fonts->size()))
                                {
                                    fail = true;
                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED;
                                    error_pos = character - multi_string.begin() - 1;
                                }
                                else
                                {
                                    f_current_font = numbers[0];
                                }
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else if(numbers.size() == 2)
                            {
                                //already existing text at another font, add it as an element to the page.
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element);

                                    text = "";
                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }
                                if((numbers[0] < 0) || (static_cast<size_t>(numbers[0]) > fonts->size()))
                                {
                                    fail = true;
                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED;
                                    error_pos = character - multi_string.begin() - 1;
                                }
                                else
                                {
                                    f_current_font = numbers[0];
                                }
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    //Field tag
                    else if(isdigit(*character))
                    {
                        temp_string = "";
                        first = true;
                        do
                        {
                            if(!first)
                            {
                                ++character;
                            }
                            else
                            {
                                first = false;
                            }
                            if(isdigit(*character) || (*character == ','))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character) || *character == ',') && character != multi_string.end());

                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() >= 1)
                            {
                                if(numbers.size() == 2)
                                {
                                    //width
                                    itemp = numbers[1];
                                }
                                else
                                {
                                    itemp = -1;
                                }
                                ss.str("");
                                switch(numbers[0])
                                {
                                //12 hour time
                                case 1:
                                    if(itemp == -1)
                                    {
                                        itemp = 5;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        hour = current_time->tm_hour;
                                        minute = current_time->tm_min;

                                    }
                                    else
                                    {
                                        hour = 12;
                                        minute = 30;
                                    }
                                    ss << Conv24Hto12HTime(hour) << ":" << std::setw(2) << std::setfill('0') << minute;
                                    text += ss.str();
                                    break;
                                //24 hour time
                                case 2:
                                    if(itemp == -1)
                                    {
                                        itemp = 5;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        hour = current_time->tm_hour;
                                        minute = current_time->tm_min;
                                    }
                                    else
                                    {
                                        hour = 12;
                                        minute = 30;
                                    }
                                    ss << hour << ":" << std::setw(2) << std::setfill('0') << minute;
                                    text += ss.str();
                                    break;
                                //temperature in C
                                case 3:
                                    if(itemp == -1)
                                    {
                                        itemp = 3;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        itemp = field_data->fGetTemperature(true);
                                    }
                                    else
                                    {
                                        itemp = 27;
                                    }
                                    ss << itemp;
                                    text += ss.str();
                                    break;
                                //temperature in F
                                case 4:
                                    if(itemp == -1)
                                    {
                                        itemp = 3;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        itemp = field_data->fGetTemperature();
                                    }
                                    else
                                    {
                                        itemp = 80;
                                    }
                                    ss << itemp;
                                    text += ss.str();
                                    break;
                                //speed in kph
                                case 5:
                                    if(itemp == -1)
                                    {
                                        itemp = 3;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        itemp = field_data->fGetSpeed(true);
                                    }
                                    else
                                    {
                                        itemp = 85;
                                    }

                                    if(field_data != nullptr)
                                    {
                                        if(field_data->fGetRadarConnected())
                                        {
                                            ss << itemp;
                                        }
                                        else
                                        {
                                            ss << "???";
                                        }
                                    }
                                    else
                                    {
                                        ss << itemp;
                                    }
                                    text += ss.str();
                                    break;
                                //speed in mph
                                case 6:
                                    if(itemp == -1)
                                    {
                                        itemp = 2;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        itemp = field_data->fGetSpeed();
                                    }
                                    else
                                    {
                                        itemp = 55;
                                    }

                                    if(field_data != nullptr)
                                    {
                                        if(field_data->fGetRadarConnected())
                                        {
                                            ss << itemp;
                                        }
                                        else
                                        {
                                            ss << "???";
                                        }
                                    }
                                    else
                                    {
                                        ss << itemp;
                                    }
                                    text += ss.str();
                                    break;
                                //day of week
                                case 7:
                                    if(itemp == -1)
                                    {
                                        itemp = 3;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        day = current_time->tm_wday;
                                    }
                                    else
                                    {
                                        day = 0;
                                    }
                                    switch(day)
                                    {
                                    case 0:
                                        text += "SUN";
                                        break;
                                    case 1:
                                        text += "MON";
                                        break;
                                    case 2:
                                        text += "TUE";
                                        break;
                                    case 3:
                                        text += "WED";
                                        break;
                                    case 4:
                                        text += "THU";
                                        break;
                                    case 5:
                                        text += "FRI";
                                        break;
                                    case 6:
                                        text += "SAT";
                                        break;
                                    }

                                    break;
                                //day of month
                                case 8:
                                    if(itemp == -1)
                                    {
                                        itemp = 2;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        mday = current_time->tm_mday;
                                    }
                                    else
                                    {
                                        mday = 12;
                                    }
                                    ss << mday;
                                    text += ss.str();
                                    break;
                                //month of year
                                case 9:
                                    if(itemp == -1)
                                    {
                                        itemp = 2;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        month = current_time->tm_mon;
                                    }
                                    else
                                    {
                                        month = 0;
                                    }
                                    ss << month+1;
                                    text += ss.str();
                                    break;
                                //Year in 2 digits
                                case 10:
                                    if(itemp == -1)
                                    {
                                        itemp = 2;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        year = current_time->tm_year;
                                    }
                                    else
                                    {
                                        year = 120;
                                    }
                                    ss << year % 100;
                                    text += ss.str();
                                    break;
                                //year in 4 digits
                                case 11:
                                    if(itemp == -1)
                                    {
                                        itemp = 4;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        year = current_time->tm_year;
                                    }
                                    else
                                    {
                                        year = 120;
                                    }
                                    ss << year + 1900;
                                    text += ss.str();
                                    break;
                                //12 hour time with AM/PM
                                case 12:
                                    if(itemp == -1)
                                    {
                                        itemp = 8;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        hour = current_time->tm_hour;
                                        minute = current_time->tm_min;
                                    }
                                    else
                                    {
                                        hour = 12;
                                        minute = 30;
                                    }
                                    ss << Conv24Hto12HTime(hour, &am_pm) << ":" << std::setw(2) << std::setfill('0') << minute;
                                    ss << (am_pm ? "PM" : "AM");
                                    text += ss.str();
                                    break;
                                //12 hour time with am/pm
                                case 13:
                                    if(itemp == -1)
                                    {
                                        itemp = 8;
                                    }
                                    if(field_data != nullptr)
                                    {
                                        current_time = field_data->fGetTime();
                                        hour = current_time->tm_hour;
                                        minute = current_time->tm_min;
                                    }
                                    else
                                    {
                                        hour = 12;
                                        minute = 30;
                                    }
                                    ss << Conv24Hto12HTime(hour, &am_pm) << ":" << std::setw(2) << std::setfill('0') << minute;
                                    ss << (am_pm ? "pm" : "am");
                                    text += ss.str();
                                    break;
                                default:
                                    break;
                                }
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'G':
                //Graphics Tag
                temp_string = "";
                do
                {
                    ++character;
                    if(isdigit(*character) || (*character == ','))
                    {
                        temp_string += *character;
                    }
                }while((isdigit(*character) || *character == ',') && character != multi_string.end());
                if(*character == ']')
                {
                    numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                    if(numbers.size() == 0 || numbers.size() == 2 || numbers.size() > 4)
                    {
                        error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }

                    if(numbers.size() >= 1)
                    {
                        //already existing text add it as an element to the page.
                        if(!text.empty())
                        {
                            if(f_pages.size() == 0)
                            {
                                //this is actually the first page.
                                fAddPage();
                            }
                            temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                            sign_brd_error = fAddElementToPage(temp_element);
                            text = "";

                            if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                            {
                                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                {
                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                    {
                                        temp_too_tall = true;
                                    }
                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                        }

                        index = -1;
                        if(graphics != nullptr)
                        {
                            for(size_t i=0;i<graphics->size();i++)
                            {
                                if(graphics->at(i).fGetGraphicNumber() == numbers[0])
                                {
                                    index = i;
                                }
                            }
                        }
                        if(index == -1)
                        {
                            error = HNS_MULTI_PARSER_UNDEFINED_GRAPHIC;
                            my_error = HNS_MULTI_SYNTAX_ERROR_GRAPHICNOTDEFINED;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                        else
                        {
                            itemp_x = 0;
                            itemp_y = 0;
                            if(numbers.size() >= 3)
                            {
                                //As per NTCIP 1203, the numbers are 1 based, but should be stored 0 based.
                                itemp_x = numbers[1]-1;
                                itemp_y = numbers[2]-1;
                            }
                            if(f_pages.size() == 0)
                            {
                                //this is actually the first page.
                                fAddPage();
                            }
                            sign_brd_error = fAddGraphicToPage(HNS_Graphical_Element(this,numbers[0],itemp_x,itemp_y,graphics->at(index).fGetBitmap().fGetWidth(),graphics->at(index).fGetBitmap().fGetHeight()));

                            if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                            {
                                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                {
                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                    {
                                        temp_too_tall = true;
                                    }
                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                        }

                        fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                    }
                }
                break;
            case 'H':
                ++character;
                if(character != multi_string.end())
                {
                    //Hex character tag
                    if(toupper(*character) == 'C')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isxdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isxdigit(*character)) && character != multi_string.end());

                        if(toupper(*character) == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error,true);
                            if(numbers.size() == 1)
                            {
                                if(fonts->at(f_current_font-1).fIsCharValid(numbers[0]))
                                {
                                    text += static_cast<char>(numbers[0]);
                                }
                                else
                                {
                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_CHARACTERNOTDEFINED;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }

                        fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                break;
            case 'J':
                ++character;
                if(character != multi_string.end())
                {
                    //Line justification tag
                    if(toupper(*character) == 'L')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character)) && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element);
                                    text = "";
                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }

                                switch(numbers[0])
                                {
                                case 1:
                                    f_current_line_justification = f_default_line_justification;
                                    break;
                                case 2:
                                    f_current_line_justification = HNS_JUSTIFICATION_LEFT;
                                    break;
                                case 3:
                                    f_current_line_justification = HNS_JUSTIFICATION_LINE_CENTER;
                                    break;
                                case 4:
                                    f_current_line_justification = HNS_JUSTIFICATION_RIGHT;
                                    break;
                                case 5:
                                    f_current_line_justification = HNS_JUSTIFICATION_FULL;
                                    break;
                                }
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                    }
                    //Page justification tag
                    else if(toupper(*character) == 'P')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character)) && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                    fAddElementToPage(temp_element);
                                    text = "";
                                }
                                switch(numbers[0])
                                {
                                case 1:
                                    f_current_page_justification = f_default_page_justification;
                                    break;
                                case 2:
                                    f_current_page_justification = HNS_JUSTIFICATION_TOP;
                                    break;
                                case 3:
                                    f_current_page_justification = HNS_JUSTIFICATION_PAGE_CENTER;
                                    break;
                                case 4:
                                    f_current_page_justification = HNS_JUSTIFICATION_BOTTOM;
                                    break;
                                }
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'M':
                break;
            case 'N':
                ++character;
                if(character != multi_string.end())
                {
                    //New Line tag
                    if(toupper(*character) == 'L')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character)) && character != multi_string.end());

                        //++character;
                        if(toupper(*character) == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if((numbers.size()) == 0 || (numbers.size() == 1))
                            {
                                if(numbers.size() == 0)
                                {
                                    f_current_line_spacing = fonts->at(f_current_font).fGetLineSpacing();
                                }
                                else
                                {
                                    f_current_line_spacing = numbers[0];
                                }

                                if(f_pages.size() == 0)
                                {
                                    //this is actually the first page.
                                    fAddPage();
                                }

                                if(!text.empty())
                                {
                                    temp_element = HNS_Message_Element2(this, text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element,true);

                                    text = "";
                                    fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    //New page tag
                    else if(toupper(*character) == 'P')
                    {
                        ++character;
                        if(toupper(*character) == ']')
                        {
                            if(!text.empty())
                            {
                                if(f_pages.size() == 0)
                                {
                                    //this is actually the first page.
                                    fAddPage();
                                }
                                temp_element = HNS_Message_Element2(this, text,fonts->at(f_current_font-1));
                                sign_brd_error = fAddElementToPage(temp_element);

                                text = "";
                                if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                {
                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                        {
                                            temp_too_tall = true;
                                        }
                                        error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                        my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                        error_pos = character - multi_string.begin() - 1;
                                        fail = true;
                                    }
                                }
                            }
                            if(f_pages.size() < max_num_pages)
                            {
                                fAddPage();
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                my_error = HNS_MULTI_SYNTAX_ERROR_TOOMANYPAGES;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }

                            fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'P':
                ++character;
                if(character != multi_string.end())
                {
                    if(toupper(*character) == 'B')
                    {

                    }
                    //Page time tag
                    else if(toupper(*character) == 'T')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character) || toupper(*character) == 'O')
                            {
                                if(toupper(*character) == 'O')
                                {
                                    temp_string += 'o';
                                }
                                else
                                {
                                    temp_string += *character;
                                }
                            }
                        }while((isdigit(*character) || *character == 'o') && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,'o',temp_error);
                            if(numbers.size() == 0)
                            {
                                f_current_page_time_on = f_default_page_time_on;
                                f_current_page_time_off = f_default_page_time_off;
                            }
                            else if(numbers.size() == 1)
                            {
                                f_current_page_time_on = static_cast<double>(numbers[0])/10.0;
                                f_current_page_time_off = f_default_page_time_off;
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else if(numbers.size() == 2)
                            {
                                f_current_page_time_on = static_cast<double>(numbers[0])/10.0;
                                f_current_page_time_off = static_cast<double>(numbers[1])/10.0;
                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                            f_pages[f_pages.size()-1].fPageTimeOn() = f_current_page_time_on;
                            f_pages[f_pages.size()-1].fPageTimeOff() = f_current_page_time_off;
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'S':
                ++character;
                if(character != multi_string.end())
                {
                    //Character Spacing tag
                    if(toupper(*character) == 'C')
                    {
                        temp_string = "";
                        do
                        {
                            ++character;
                            if(isdigit(*character) || (*character == ','))
                            {
                                temp_string += *character;
                            }
                        }while((isdigit(*character) || *character == ',') && character != multi_string.end());
                        if(*character == ']')
                        {
                            numbers = HNS_string_to_int_vector(temp_string,',',temp_error);
                            if(numbers.size() == 1)
                            {
                                if(!text.empty())
                                {
                                    if(f_pages.size() == 0)
                                    {
                                        //this is actually the first page.
                                        fAddPage();
                                    }
                                    temp_element = HNS_Message_Element2(this, text,fonts->at(f_current_font-1));
                                    sign_brd_error = fAddElementToPage(temp_element);
                                    text = "";
                                    if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                    {
                                        if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                        {
                                            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                            {
                                                temp_too_tall = true;
                                            }
                                            error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                            my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                            error_pos = character - multi_string.begin() - 1;
                                            fail = true;
                                        }
                                    }
                                }
                                f_current_char_spacing = numbers[0];

                                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                    }
                    else
                    {
                        error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                        error_pos = character - multi_string.begin() - 1;
                        fail = true;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }
                break;
            case 'T':
                break;
            case '/':
                ++character;
                if(character != multi_string.end())
                {
                    switch(toupper(*character))
                    {
                    case 'S':
                        ++character;
                        if(character != multi_string.end())
                        {
                            if(toupper(*character) == 'C')
                            {
                                ++character;
                                if(character != multi_string.end())
                                {
                                    if(*character == ']')
                                    {
                                        if(!text.empty())
                                        {
                                            if(f_pages.size() == 0)
                                            {
                                                //this is actually the first page.
                                                fAddPage();
                                            }
                                            temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                            sign_brd_error = fAddElementToPage(temp_element);
                                            text = "";
                                            if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                            {
                                                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                                {

                                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                                    {
                                                        temp_too_tall = true;
                                                    } error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                                    my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                                    error_pos = character - multi_string.begin() - 1;
                                                    fail = true;
                                                }
                                            }
                                        }
                                        f_current_char_spacing = fonts->at(f_current_font).fGetCharSpacing();

                                        fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                                    }
                                    else
                                    {
                                        error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                        error_pos = character - multi_string.begin() - 1;
                                        fail = true;
                                    }
                                }
                                else
                                {
                                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                        break;
                    case 'F':
                        ++character;
                        if(character != multi_string.end())
                        {
                            if(toupper(*character) == 'L')
                            {
                                ++character;
                                if(character != multi_string.end())
                                {
                                    if(*character == ']')
                                    {
                                        //already existing text add it as an element to the page.
                                        if(!text.empty())
                                        {
                                            if(f_pages.size() == 0)
                                            {
                                                //this is actually the first page.
                                                fAddPage();
                                            }
                                            temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
                                            sign_brd_error = fAddElementToPage(temp_element);
                                            text = "";
                                            if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
                                            {
                                                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
                                                {
                                                    if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                                                    {
                                                        temp_too_tall = true;
                                                    }
                                                    error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                                                    my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                                                    error_pos = character - multi_string.begin() - 1;
                                                    fail = true;
                                                }
                                            }
                                        }

                                        f_current_flashing_state = false;
                                        fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                                    }
                                    else
                                    {
                                        error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                        my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                        error_pos = character - multi_string.begin() - 1;
                                        fail = true;
                                    }
                                }
                                else
                                {
                                    error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                    error_pos = character - multi_string.begin() - 1;
                                    fail = true;
                                }
                            }
                            else
                            {
                                error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                                error_pos = character - multi_string.begin() - 1;
                                fail = true;
                            }
                        }
                        else
                        {
                            error = HNS_MULTI_PARSER_INCOMPLETE_TAG;
                            my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                            error_pos = character - multi_string.begin() - 1;
                            fail = true;
                        }
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                    my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                    error_pos = character - multi_string.begin() - 1;
                    fail = true;
                }

                break;
            case '[':
                text += *character;
                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
                break;
            default:
                error = HNS_MULTI_PARSER_UNRECOGNIZED_TAG;
                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                error_pos = character - multi_string.begin() - 1;
                fail = true;
                break;
            }
            break;
        case HNS_MULTI_PARSER_STATE_CLOSING_CANDIDATE:
            if(*character == ']')
            {
                text += *character;
                fsm_state = HNS_MULTI_PARSER_STATE_TEXT;
            }
            else
            {
                error = HNS_MULTI_PARSER_UNEXPECTED_CLOSING_BRACE;
                my_error = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                error_pos = character - multi_string.begin() - 1;
                fail = true;
            }
            break;
        case HNS_MULTI_PARSER_STATE_TAG_J:
            break;
        }
    }
    if(!text.empty())
    {
        if(f_pages.size() == 0)
        {
            //this is actually the first page.
            fAddPage();
        }
        temp_element = HNS_Message_Element2(this,text,fonts->at(f_current_font-1));
        sign_brd_error = fAddElementToPage(temp_element);

        if(sign_brd_error != HNS_SGNBRD_ERROR_NONE)
        {
            if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL || sign_brd_error == HNS_SGNBRD_ERROR_TOOWIDE)
            {
                if(sign_brd_error == HNS_SGNBRD_ERROR_TOOTALL)
                {
                    temp_too_tall = true;
                }
                error = HNS_MULTI_PARSER_SYNTAX_ERROR;
                my_error = HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG;
                error_pos = multi_string.length()-1;
                fail = true;
            }
        }
    }

    if(too_tall != nullptr)
    {
        *too_tall = temp_too_tall;
    }

    if(multi_error != nullptr)
    {
        *multi_error = my_error;
    }

    if(error_position != nullptr)
    {
        *error_position = error_pos;
    }
    return error;
}

std::string HNS_Message2::fGetMULTI() const
{
    return f_multi;
}

void HNS_Message2::fAddPage()
{
    HNS_Message_Page2 temp_page(this,f_current_page_time_on,f_current_page_time_off,f_default_signboard_info);
    f_pages.push_back(temp_page);
}

type_hns_signboard_error HNS_Message2::fAddElementToPage(HNS_Message_Element2 &element, const bool &newline)
{
    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;

    if(f_pages.size() > 0)
    {
        error = f_pages[f_pages.size()-1].fAddElement(element,newline);
    }

    return error;
}

type_hns_signboard_error HNS_Message2::fAddGraphicToPage(const HNS_Graphical_Element &graphic)
{
    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;

    if(f_pages.size() > 0)
    {
        error = f_pages[f_pages.size()-1].fAddGraphic(graphic);
    }

    return error;
}

size_t HNS_Message2::fGetNumPages() const
{
    return f_pages.size();
}

type_justification_line HNS_Message2::fGetLastLineJustification(const size_t &page)
{
    if(page < f_pages.size())
    {
        return f_pages[page].fGetLastLineJustification();
    }
    else
    {
        return HNS_JUSTIFICATION_LINE_CENTER;
    }
}

type_justification_page HNS_Message2::fGetLastPageJustification(const size_t &page)
{
    if(page < f_pages.size())
    {
        return f_pages[page].fGetLastPageJustification();
    }
    else
    {
        return HNS_JUSTIFICATION_PAGE_CENTER;
    }
}

double HNS_Message2::fGetPageTimeOn(const size_t &page) const
{
    if(page < f_pages.size())
    {
        return f_pages[page].fGetPageTimeOn();
    }
    else
    {
        return -10.0;
    }
}

double HNS_Message2::fGetPageTimeOff(const size_t &page) const
{
    if(page < f_pages.size())
    {
        return f_pages[page].fGetPageTimeOff();
    }
    else
    {
        return -10.0;
    }
}

size_t HNS_Message2::fGetBoardWidth() const
{
    return f_default_signboard_info.fGetWidth();
}

size_t HNS_Message2::fGetBoardHeight() const
{
    return f_default_signboard_info.fGetHeight();
}

void HNS_Message2::fSetFontTest(const std::vector<HNS_Font> *fonts, const size_t font_no)
{
    f_multi = "";
    f_pages.clear();
    HNS_Message_Element2 temp_element(this);
    HNS_Message_Justified_Element temp_just_element(this,0);
    HNS_Message_Page2 temp_page(this,2.0,0.0,f_default_signboard_info);
    string multi;
    char temp_char = 'A';
    string tempstring;
    HNS_Font tempfont;
    bool first_line = true;
    stringstream ss;

    if(font_no-1 < fonts->size())
    {
        tempfont = fonts->at(font_no-1);
    }

    ss << "[fo" << font_no << "]";

    multi = ss.str();

    if(f_default_signboard_info.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        do
        {
            do
            {
                tempstring += temp_char;
                temp_char++;
                temp_element.fSetText(tempstring,tempfont,font_no,false,HNS_Flashing_Text());
                if(temp_char > 'Z')
                {
                    break;
                }
            } while(temp_element.fGetWidth() < f_default_signboard_info.fGetWidth());

            if(temp_char <= 'Z')
            {
                temp_char--;
            }
            if(temp_element.fGetWidth() >= f_default_signboard_info.fGetWidth())
            {
                tempstring.erase(tempstring.end()-1);
            }
            temp_element.fSetText(tempstring,tempfont,font_no,false,HNS_Flashing_Text());

            temp_just_element.fAddElement(temp_element,true);

            if(temp_just_element.fGetHeight() < f_default_signboard_info.fGetHeight())
            {
                if(!first_line)
                {
                    multi = multi + "[nl]";
                }
                multi = multi + tempstring;
                temp_page.fAddElement(temp_element,true);
            }

            tempstring.clear();

            if(first_line)
            {
                first_line = false;
            }
        }while(temp_just_element.fGetHeight() < f_default_signboard_info.fGetHeight() && temp_char <= 'Z');
    }
    else
    {
        size_t line_count = 0;
        size_t char_count = 0;
        do
        {
            char_count = 0;
            do
            {
                tempstring += temp_char;
                temp_char++;
                temp_element.fSetText(tempstring,tempfont,font_no,false,HNS_Flashing_Text());
                if(temp_char > 'Z')
                {
                    break;
                }
                char_count++;
            } while(char_count < f_default_signboard_info.fGetBoardsWide());

            temp_element.fSetText(tempstring,tempfont,font_no,false,HNS_Flashing_Text());

            temp_just_element.fAddElement(temp_element,true);

            if(!first_line)
            {
                multi = multi + "[nl]";
            }
            multi = multi + tempstring;
            temp_page.fAddElement(temp_element,true);

            tempstring.clear();

            if(first_line)
            {
                first_line = false;
            }
            line_count++;
        }while(line_count < f_default_signboard_info.fGetBoardsTall() && temp_char <= 'Z');
    }

    fSetMULTI(multi,fonts,nullptr,nullptr);

}

type_justification_line HNS_Message2::fGetDefaultLineJustification()
{
    return f_default_line_justification;
}

void HNS_Message2::fSetDefaultLineJustification(const type_justification_line &line_justification)
{
    f_default_line_justification = line_justification;
}

type_justification_page HNS_Message2::fGetDefaultPageJustification()
{
    return f_default_page_justification;
}

void HNS_Message2::fSetDefaultPageJustification(const type_justification_page &page_justification)
{
    f_default_page_justification = page_justification;
}

double HNS_Message2::fGetDefaultPageTimeOn()
{
    return f_default_page_time_on;
}

void HNS_Message2::fSetDefaultPageTimeOn(const double &pagetime_on)
{
    f_default_page_time_on = pagetime_on;
}

double HNS_Message2::fGetDefaultPageTimeOff()
{
    return f_default_page_time_off;
}

void HNS_Message2::fSetDefaultPageTimeOff(const double &pagetime_off)
{
    f_default_page_time_off = pagetime_off;
}

double HNS_Message2::fGetDefaultFlashTimeOn()
{
    return static_cast<double>(f_default_flash_time_on) / 10.0;
}

void HNS_Message2::fSetDefaultFlashTimeOn(const double &flashtime_on)
{
    f_default_flash_time_on = static_cast<unsigned int>(flashtime_on * 10.0);
}

double HNS_Message2::fGetDefaultFlashTimeOff()
{
    return static_cast<double>(f_default_flash_time_off) / 10.0;
}

void HNS_Message2::fSetDefaultFlashTimeOff(const double &flashtime_off)
{
    f_default_flash_time_off = static_cast<unsigned int>(flashtime_off * 10.0);
}

int HNS_Message2::fGetDefaultFont()
{
    return f_default_font;
}

void HNS_Message2::fSetDefaultFont(const int &font)
{
    f_default_font = font;
}

HNS_SignBoard HNS_Message2::fGetSignBoard(const int64_t &time, size_t *page_displayed)
{
    HNS_SignBoard result = HNS_SignBoard(f_default_signboard_info);
    unsigned int time_in_page;
    size_t index = fGetPageIndexFromTime(time,time_in_page);

    if(page_displayed != nullptr)
    {
        *page_displayed = index;
    }

    if(index < f_pages.size())
    {
        result = f_pages[index].fGetSignBoard(time_in_page,false,f_pages.size() == 1);
    }

    return result;
}

HNS_Message_Page2 HNS_Message2::fGetPage(const size_t &page_no) const
{
    if(page_no < f_pages.size())
    {
        return f_pages[page_no];
    }
    else
    {
        return HNS_Message_Page2(this);
    }
}

vector<int> HNS_Message2::fGetFontsUsed()
{
    vector<int> result,temp_vec;

    for(size_t i=0;i<f_pages.size();i++)
    {
        temp_vec = f_pages[i].fGetFontsUsed();
        result = MergeFontsUsedLists(result,temp_vec);
    }

    return result;
}

size_t HNS_Message2::fGetPageIndexFromTime(const int64_t &time, unsigned int &time_in_page)
{
    size_t result = 0;
    unsigned int message_period = fGetMessagePeriod();
    int64_t adjusted_time = 0, search_time = 0, time_in_page_temp = 0;

    if(message_period > 0)
    {
        if(f_pages.size() > 1)
        {
            time_in_page_temp = search_time = adjusted_time = time % message_period;

            for(size_t ui=0;ui<f_pages.size();ui++)
            {
                search_time -= static_cast<unsigned int>(f_pages[ui].fPageTimeOn()*1000.0);
                search_time -= static_cast<unsigned int>(f_pages[ui].fPageTimeOff()*1000.0);

                if(search_time < 0)
                {
                    result = ui;
                    time_in_page = time_in_page_temp;
                    break;
                }
                else
                {
                    time_in_page_temp = search_time;
                }
            }
        }
        else
        {
            time_in_page = time;
        }
    }

    return result;
}

unsigned int HNS_Message2::fGetMessagePeriod() const
{
    unsigned int result = 0;

    if(f_pages.size() > 1)
    {
        for(size_t ui=0;ui<f_pages.size();ui++)
        {
            result += static_cast<unsigned int>((f_pages[ui].fGetPageTimeOn() * 1000.0) + (f_pages[ui].fGetPageTimeOff() * 1000.0));
        }
    }
    else if(f_pages.size() > 0)
    {
        result = static_cast<unsigned int>(f_pages[0].fGetPageTimeOn() * 1000.0);
    }

    return result;
}

void HNS_Message2::fResetMulti()
{
}

HNS_Message_Page2::HNS_Message_Page2(const HNS_Message2 *parent):
    f_page_time_on(HNS_Message2::fGetDefaultPageTimeOn()),
    f_page_time_off(HNS_Message2::fGetDefaultPageTimeOff()),
    f_signboard_info(HNS_Message2::fGetDefaultSignBoardInfo()),
    f_parent_message(parent)
{

}

HNS_Message_Page2::HNS_Message_Page2(const HNS_Message2 *parent, const double &page_time_on, const double &page_time_off, const HNS_SignBoard_Info &sign_board_info):
    f_page_time_on(page_time_on)
  , f_page_time_off(page_time_off)
  , f_signboard_info(sign_board_info)
  , f_parent_message(parent)
{
}

//void HNS_Message_Page2::fNewJustification(const type_justification_line &line_justification, const type_justification_page &page_justification)
//{
//    if(f_elements.size() > 0)
//    {
//        //for some reason a justification tag immediately followed a justification tag with no text.  don't add a new element, just change the last justification
//        if(f_elements[f_elements.size()-1].fGetNumElements() == 0)
//        {
//            f_elements[f_elements.size()-1].fChangeJustification(line_justification,page_justification);
//        }
//        else
//        {
//            f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,line_justification,page_justification));
//        }
//    }
//    else
//    {
//        f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,line_justification,page_justification));
//    }
//}

//type_hns_signboard_error HNS_Message_Page2::fAddElement(HNS_Message_Element2 &element,
//                                    const type_justification_line &line_justification,
//                                    const type_justification_page &page_justification,
//                                    const bool &newline,
//                                    const ssize_t &line_spacing,
//                                    const ssize_t &/*char_spacing*/)
//{
//    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;
//    if(f_elements.size() > 0)
//    {
//        if(element.fIsGraphic())
//        {
//            f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,line_justification,page_justification));
//            f_elements[f_elements.size()-1].fAddElement(element,newline,line_spacing);
//        }
//        else
//        {
//            f_elements[fLastTextElement()].fAddElement(element,newline,line_spacing);
//        }
//    }
//    else
//    {
//        f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,line_justification,page_justification));
//        f_elements[f_elements.size()-1].fAddElement(element,newline,line_spacing);
//    }

//    fSortElements();
//    HNS_SignBoard temp_signboard(f_signboard_info);
//    error = fUpdateSignBoard(temp_signboard);

//    return error;
//}

type_hns_signboard_error HNS_Message_Page2::fAddElement(HNS_Message_Element2 &element,
                                    const bool &newline)
{
    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;

    if(f_parent_message != nullptr)
    {
        if(f_page_elements.size() > 0)
        {
            //New page justification, add a new block
            if(f_parent_message->fGetCurrentPageJustification() != f_page_elements[f_page_elements.size()-1].fGetPageJustification())
            {
                f_page_elements.push_back(HNS_Message_Page_Justified_Element(f_parent_message));
            }
            f_page_elements[f_page_elements.size()-1].fAddElement(element,newline);
        }
        else
        {
            f_page_elements.push_back(HNS_Message_Page_Justified_Element(f_parent_message));
            f_page_elements[f_page_elements.size()-1].fAddElement(element,newline);
        }

        HNS_SignBoard temp_signboard(f_signboard_info);
        error = fUpdateSignBoard(temp_signboard);
    }

    return error;
}

type_hns_signboard_error HNS_Message_Page2::fAddGraphic(const HNS_Graphical_Element &graphic)
{
    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;

    f_graphics.push_back(graphic);

    HNS_SignBoard temp_signboard(f_signboard_info);
    error = fUpdateSignBoard(temp_signboard);

    return error;
}

size_t HNS_Message_Page2::fGetNumElements() const
{
    return f_elements.size();
}

void HNS_Message_Page2::fAddNewline()
{
    if(f_elements.size() > 0)
    {
        //f_elements[f_elements.size()-1].
    }
}

double HNS_Message_Page2::fGetPageTimeOn() const
{
    return f_page_time_on;
}

double HNS_Message_Page2::fGetPageTimeOff() const
{
    return f_page_time_off;
}

double &HNS_Message_Page2::fPageTimeOn()
{
    return f_page_time_on;
}

double &HNS_Message_Page2::fPageTimeOff()
{
    return f_page_time_off;
}

type_justification_line HNS_Message_Page2::fGetLastLineJustification()
{
    return f_elements[f_elements.size()-1].fGetLineJustification();
}

type_justification_page HNS_Message_Page2::fGetLastPageJustification()
{
    return f_elements[f_elements.size()-1].fGetPageJustification();
}

HNS_SignBoard HNS_Message_Page2::fGetSignBoard(const int64_t &time, const bool &preview_mode, const bool &single_page)
{
    HNS_SignBoard result(f_signboard_info);
    fUpdateSignBoard(result,time,preview_mode,single_page);
    return result;
}

vector<int> HNS_Message_Page2::fGetFontsUsed()
{
    vector<int> result,temp_vec;

    for(size_t i=0;i<f_elements.size();i++)
    {
        temp_vec = f_elements[i].fGetFontsUsed();
        result = MergeFontsUsedLists(result,temp_vec);
    }

    return result;
}

bool HNS_Message_Page2::fSortElements()
{
    bool success = true;
    vector<HNS_Message_Justified_Element> sorted_elements,graphic_elements;

    for(size_t i=0;i<f_elements.size();i++)
    {
        sorted_elements.push_back(f_elements[i]);
    }
    f_elements.clear();
    f_elements.insert(f_elements.end(),sorted_elements.begin(),sorted_elements.end());
    f_elements.insert(f_elements.end(),graphic_elements.begin(),graphic_elements.end());

    return success;
}

type_hns_signboard_error HNS_Message_Page2::fUpdateSignBoard(HNS_SignBoard &sign_board, const int64_t &time, const bool &preview_mode, const bool &single_page)
{
    type_hns_signboard_error error = HNS_SGNBRD_ERROR_NONE;

    sign_board.fClearBoard();
    //MSC20220114 Making this less than or equals I think should eliminate blanks during one page messages and messages with no page off time.
//    if(time <= static_cast<int64_t>(f_page_time_on*1000.0) || single_page)
//    {
//        for(size_t ui=0;ui<f_elements.size();ui++)
//        {
//            error = static_cast<type_hns_signboard_error>(static_cast<int>(error) | static_cast<int>(sign_board.fAddElement(f_elements[ui],time,preview_mode)));
//        }
//    }

    if(time <= static_cast<int64_t>(f_page_time_on*1000.0) || single_page)
    {
        for(size_t ui=0;ui<f_page_elements.size();ui++)
        {
            error = static_cast<type_hns_signboard_error>(static_cast<int>(error) | static_cast<int>(sign_board.fAddElement(f_page_elements[ui],time,preview_mode)));
        }

        for(size_t ui=0;ui<f_graphics.size();ui++)
        {
            error = static_cast<type_hns_signboard_error>(static_cast<int>(error) | static_cast<int>(sign_board.fAddGraphic(f_graphics[ui],time,preview_mode)));
        }
    }

    return error;
}

HNS_Graphical_Element::HNS_Graphical_Element(const HNS_Message2 *parent):
    f_graphic_no(0)
  , f_x(0)
  , f_y(0)
  , f_parent_message(parent)
{
    if(f_parent_message != nullptr)
    {
        f_flash_info = f_parent_message->fGetCurrentFlashingInfo();
        f_is_flashing = f_parent_message->fGetCurrentFlashState();
    }
    else
    {
        f_is_flashing = false;
    }
}

HNS_Graphical_Element::HNS_Graphical_Element(const HNS_Message2 *parent, const int &graphic_no, const int &x, const int &y, const int &width, const int &height):
    f_graphic_no(graphic_no)
  , f_x(x)
  , f_y(y)
  , f_width(width)
  , f_height(height)
  , f_parent_message(parent)
{
    if(f_parent_message != nullptr)
    {
        f_flash_info = f_parent_message->fGetCurrentFlashingInfo();
        f_is_flashing = f_parent_message->fGetCurrentFlashState();
    }
    else
    {
        f_is_flashing = false;
    }
}

HNS_Bitmap HNS_Graphical_Element::fGetBitmap(const std::vector<HNS_Graphic> *graphics, const int64_t &time, const bool &) const
{
    HNS_Bitmap result;

    if(f_parent_message != nullptr)
    {
        if(IsFlashOn(time,f_parent_message->fGetCurrentFlashingInfo()))
        {
            for(size_t i = 0;i<graphics->size();i++)
            {
                if(graphics->at(i).fGetGraphicNumber() == f_graphic_no)
                {
                    result = graphics->at(i).fGetBitmap();
                    break;
                }
            }
        }
    }

    return result;
}

HNS_Message_Page_Justified_Element::HNS_Message_Page_Justified_Element(const HNS_Message2 *parent):
    f_page_justification(HNS_JUSTIFICATION_PAGE_CENTER)
  , f_parent_message(parent)
  , f_line_no(0)
{
    if(f_parent_message != nullptr)
    {
        f_page_justification = f_parent_message->fGetCurrentPageJustification();
    }
    f_elements.clear();
}

void HNS_Message_Page_Justified_Element::fAddElement(const HNS_Message_Element2 &element, const bool &newline)
{
    if(f_elements.size() > 0)
    {
        if(f_parent_message->fGetCurrentLineJustification() != f_elements[f_elements.size()-1].fGetLineJustification())
        {
            int lines = 0;
            for(size_t ui=0;ui<f_elements.size();ui++)
            {
                lines += f_elements[f_elements.size()-1].fGetNumLines();
            }
            f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,lines));
        }
    }
    else
    {
        f_elements.push_back(HNS_Message_Justified_Element(f_parent_message,0));
    }
    f_elements[f_elements.size()-1].fAddElement(element,newline);
}

HNS_Message_Justified_Element HNS_Message_Page_Justified_Element::fGetElement(const size_t &index) const
{
    HNS_Message_Justified_Element result(f_parent_message,0);

    if(index < f_elements.size())
    {
        return f_elements[index];
    }

    return result;
}

//void HNS_Message_Page_Justified_Element::fAddText(const std::string &text, const bool &newline)
//{
//    if(f_parent_message != nullptr)
//    {
//        size_t dummy = f_elements.size();
//        bool test = f_elements.empty();
//        if(f_elements.size() > 0)
//        {
//            if(f_elements[f_elements.size()-1].fGetLineJustification() != f_parent_message->fGetCurrentLineJustification())
//            {
//                f_elements.push_back(HNS_Message_Justified_Element(f_parent_message));
//            }
//        }
//        else
//        {
//            f_elements.push_back(HNS_Message_Justified_Element(f_parent_message));
//        }
//        f_elements[f_elements.size()-1].fAddText(text,newline);
//    }
//}

HNS_Bitmap HNS_Message_Page_Justified_Element::fGetBitmap(const std::vector<HNS_Font> *fonts, const int64_t &time, const HNS_SignBoard *signboard, type_hns_signboard_error *error, const bool &preview_mode) const
{
    HNS_Bitmap result,temp_bitmap;
    type_hns_signboard_error temp_error = HNS_SGNBRD_ERROR_NONE;

    HNS_SignBoard_Info temp_signboard_info;
    int x=0,y=0;

    if(signboard != nullptr)
    {
        temp_signboard_info = signboard->fGetSignBoardInfo();
    }
    else
    {
        temp_signboard_info = HNS_Message2::fGetDefaultSignBoardInfo();
    }

    int height = 0;

    for(size_t ui=0;ui<f_elements.size();ui++)
    {
        height += f_elements[ui].fGetHeight();
        if(ui < f_elements.size() - 1)
        {
            height += f_elements[ui].fGetLastLineSpacing();
        }
    }

    result.fResize(height,temp_signboard_info.fGetWidth());

    for(size_t ui=0;ui<f_elements.size();ui++)
    {
        temp_bitmap = f_elements[ui].fGetBitmap(fonts,time,preview_mode);
        switch(f_elements[ui].fGetLineJustification())
        {
        case HNS_JUSTIFICATION_LEFT:
            if(temp_bitmap.fGetWidth() > result.fGetWidth())
            {
                temp_error = static_cast<type_hns_signboard_error>(static_cast<int>(temp_error) | static_cast<int>(HNS_SGNBRD_ERROR_TOOWIDE));
            }
            x = 0;
            break;
        case HNS_JUSTIFICATION_FULL:
        case HNS_JUSTIFICATION_LINE_CENTER:
            if(temp_bitmap.fGetWidth() <= result.fGetWidth())
            {
                x = (result.fGetWidth() - temp_bitmap.fGetWidth())/2;
            }
            else
            {
                temp_error = static_cast<type_hns_signboard_error>(static_cast<int>(temp_error) | static_cast<int>(HNS_SGNBRD_ERROR_TOOWIDE));
            }
            break;
        case HNS_JUSTIFICATION_RIGHT:
            if(temp_bitmap.fGetWidth() <= result.fGetWidth())
            {
                x = result.fGetWidth() - temp_bitmap.fGetWidth();
            }
            else
            {
                temp_error = static_cast<type_hns_signboard_error>(static_cast<int>(temp_error) | static_cast<int>(HNS_SGNBRD_ERROR_TOOWIDE));
            }
            break;
        }
        result.fCopy(temp_bitmap,x,y);

        y += f_elements[ui].fGetHeight() + f_elements[ui].fGetLastLineSpacing();

        if(temp_error != HNS_SGNBRD_ERROR_NONE)
        {
            break;
        }
    }

    *error = temp_error;
    return result;
}

type_justification_page HNS_Message_Page_Justified_Element::fGetPageJustification() const
{
    return HNS_JUSTIFICATION_PAGE_CENTER;
}

size_t HNS_Message_Page_Justified_Element::fGetNumLines() const
{
    size_t result = 0;
    for(size_t ui=0;ui<f_elements.size();ui++)
    {
        result += f_elements[ui].fGetNumLines();
    }
    return result;
}

HNS_Message_Justified_Element::HNS_Message_Justified_Element(const HNS_Message2 *parent, const int &starting_line):
    f_line_justification(HNS_JUSTIFICATION_LINE_CENTER)
  , f_page_justification(HNS_JUSTIFICATION_PAGE_CENTER)
  , f_temp_line_no(starting_line)
  , f_parent_message(parent)
{
    if(f_parent_message != nullptr)
    {
        f_line_justification = f_parent_message->fGetCurrentLineJustification();
        f_page_justification = f_parent_message->fGetCurrentPageJustification();
    }
}

void HNS_Message_Justified_Element::fAddElement(HNS_Message_Element2 element, const bool &newline)
{
    element.fSetLineNo(f_temp_line_no);
    f_elements.push_back(element);
    size_t new_line_spacing;
    if(newline)
    {
        new_line_spacing = f_parent_message->fGetCurrentLineSpacing();

        f_line_spacing.push_back(new_line_spacing);
        f_temp_line_no++;
    }
}

void HNS_Message_Justified_Element::fChangeJustification(const type_justification_line &line_justification, const type_justification_page &page_justification)
{
    f_line_justification = line_justification;
    f_page_justification = page_justification;
}

//TODO Get the bitmap to respect flashing and times.
HNS_Bitmap HNS_Message_Justified_Element::fGetBitmap(const vector<HNS_Font> *fonts, const int64_t &time, const bool &preview_mode) const
{
    size_t line = 0;
    int x,y;
    size_t widest_line = 0;
    vector<size_t> line_widths = fGetLineWidths();
    vector<size_t> line_heights = fGetLineHeights();
    size_t line_widths_index = 0;
    size_t height = fGetHeight();
    HNS_Bitmap result;

    for(size_t i = 0;i<line_widths.size();i++)
    {
        if(line_widths[i] > widest_line)
        {
            widest_line = line_widths[i];
        }
    }

    result = HNS_Bitmap(height,widest_line);

    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i==0)
        {
            line = f_elements[i].fGetLineNo();
        }
        if((line != f_elements[i].fGetLineNo()) || (i == 0))
        {
            if(i != 0)
            {
                line_widths_index++;
            }
            switch(f_line_justification)
            {
            case HNS_JUSTIFICATION_LEFT:
                x = 0;
                break;
            case HNS_JUSTIFICATION_LINE_CENTER:
                x = (widest_line - line_widths[line_widths_index])/2;
                break;
            case HNS_JUSTIFICATION_RIGHT:
                x = widest_line - line_widths[line_widths_index];
                break;
            case HNS_JUSTIFICATION_FULL:
                //shouldn't ever get here
                break;
            }

            line = f_elements[i].fGetLineNo();
        }
        else
        {
            x = x + f_elements[i-1].fGetWidth() + f_elements[i].fGetCharSpacing();
        }
        y = fGetYPos(line_heights,line_widths_index);
        //NTCIP 1203 section 6.4.7 says that differing height fonts on the same line should align on the bottom
        y = y + (line_heights[line_widths_index] - f_elements[i].fGetHeight());
        if(preview_mode || !f_elements[i].fGetIsFlashing() || IsFlashOn(time,f_elements[i].fGetFlashInfo()))
        {
            result.fCopy(f_elements[i].fGetBitmap(fonts),x,y);
        }
    }

    return result;
}

HNS_Bitmap HNS_Message_Justified_Element::fGetBitmapFullJustified(const std::vector<HNS_Font> *fonts)
{
    size_t line = 0;

    //first find the widest line
    size_t widest_line = 0;
    size_t widest_line_index = 0;
    vector<size_t> line_widths = fGetLineWidths();
    vector<size_t> line_heights = fGetLineHeights();
    vector<size_t> character_count = fGetCharactersPerLine();
    size_t line_widths_index = 0;
    size_t spacing, spacing_remainder;
    size_t new_width;
    size_t x,y;
    size_t height = fGetHeight();

    for(size_t i = 0;i<line_widths.size();i++)
    {
        if(line_widths[i] > widest_line)
        {
            widest_line = line_widths[i];
            widest_line_index = i;
        }
    }

    HNS_Bitmap result(height,widest_line);

    //now for a little magic.  resize the lines
    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i==0)
        {
            line = f_elements[i].fGetLineNo();
            line_widths_index = 0;
        }
        if((line != f_elements[i].fGetLineNo()) || (i == 0))
        {
            if(i != 0)
            {
                line = f_elements[i].fGetLineNo();
                line_widths_index++;
            }
            //new line encountered that is not the widest line
            //calculate line growth
            if(line != widest_line_index)
            {
                if(character_count[line_widths_index] > 1)
                {
                   spacing = (widest_line - line_widths[line_widths_index]) / (character_count[line_widths_index] - 1);
                   spacing_remainder = (widest_line - line_widths[line_widths_index]) % (character_count[line_widths_index] - 1);
                }
                else
                {
                    spacing = 0;
                    spacing_remainder = 0;
                }
                if(spacing == 0)
                {
                    spacing = 1;
                }

               new_width = f_elements[i].fGetCharacterWidth(fonts) + (spacing * (f_elements[i].fGetCharacterCount() -1));
               if((f_elements[i].fGetCharacterCount() - 1) < spacing_remainder)
               {
                   new_width += (f_elements[i].fGetCharacterCount() - 1);
                   spacing_remainder -= (f_elements[i].fGetCharacterCount() - 1);
               }
               else
               {
                   new_width += spacing_remainder;
               }
            }
            x = 0;
        }
        else
        {
            //no need to resize the widest line
            if(line != widest_line_index)
            {
                new_width = f_elements[i].fGetCharacterWidth(fonts) + (spacing * (f_elements[i].fGetCharacterCount() -1));
                if((f_elements[i].fGetCharacterCount() - 1) < spacing_remainder)
                {
                    new_width += (f_elements[i].fGetCharacterCount() - 1);
                    spacing_remainder -= (f_elements[i].fGetCharacterCount() - 1);
                }
                else
                {
                    new_width += spacing_remainder;
                }
            }
            x = f_elements[i-1].fGetWidth() + spacing;
        }
        y = fGetYPos(line_heights,line_widths_index);
        //NTCIP 1203 section 6.4.7 says that differing height fonts on the same line should align on the bottom
        y = y + (line_heights[line_widths_index] - f_elements[i].fGetHeight());
        //now resize the element
        if(line == widest_line_index)
        {
            result.fCopy(f_elements[i].fGetBitmap(fonts),x,y);
        }
        else
        {
            result.fCopy(f_elements[i].fGetBitmapFixedWidth(fonts,new_width),x,y);
        }
    }

    return result;
}

type_justification_line HNS_Message_Justified_Element::fGetLineJustification() const
{
    return f_line_justification;
}

type_justification_page HNS_Message_Justified_Element::fGetPageJustification() const
{
    return f_page_justification;
}

vector<size_t> HNS_Message_Justified_Element::fGetLineWidths() const
{
    vector<size_t> result;
    size_t current_line = 0;
    size_t current_width = 0;

    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i == 0)
        {
            current_line = f_elements[i].fGetLineNo();
        }
        if(current_line != f_elements[i].fGetLineNo())
        {
            result.push_back(current_width);
            current_width = f_elements[i].fGetWidth();
            current_line = f_elements[i].fGetLineNo();
        }
        else
        {
            if(i != 0)
            {
                if(f_elements[i].fGetWidth() > 0)
                {
                    current_width += f_elements[i].fGetCharSpacing();
                }
            }
            current_width = current_width + f_elements[i].fGetWidth();
        }
    }
    result.push_back(current_width);

    return result;
}

std::vector<size_t> HNS_Message_Justified_Element::fGetLineHeights() const
{
    vector<size_t> result;
    size_t current_line = 0;
    size_t current_height = 0;

    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i == 0)
        {
            current_line = f_elements[i].fGetLineNo();
        }
        if(current_line != f_elements[i].fGetLineNo())
        {
            result.push_back(current_height);
            current_height = f_elements[i].fGetHeight();
            current_line = f_elements[i].fGetLineNo();
        }
        else
        {
            current_height = max(current_height, f_elements[i].fGetHeight());
        }
    }
    result.push_back(current_height);

    return result;
}

vector<size_t> HNS_Message_Justified_Element::fGetCharactersPerLine() const
{
    vector<size_t> result;

    size_t current_line = 0;
    size_t current_count = 0;

    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i == 0)
        {
            current_count = f_elements[i].fGetCharacterCount();
        }
        if(current_line != f_elements[i].fGetLineNo())
        {
            result.push_back(current_count);
            current_count = f_elements[i].fGetCharacterCount();
            current_line = f_elements[i].fGetLineNo();
        }
        else
        {
            current_count = current_count + f_elements[i].fGetCharacterCount();
        }
    }
    result.push_back(current_count);

    return result;
}

size_t HNS_Message_Justified_Element::fGetHeight() const
{
    bool first_line = true;
    size_t height = 0;
    size_t current_line_height = 0;
    size_t current_line = 0;
    size_t num_lines = 0;
    size_t index = 0;

    for(size_t i=0;i<f_elements.size();i++)
    {
        if(i == 0)
        {
            current_line = f_elements[i].fGetLineNo();
            num_lines++;
        }
        if(current_line != f_elements[i].fGetLineNo())
        {
            height += current_line_height;
            num_lines++;
            if(!first_line)
            {
                height+=f_line_spacing[index];
                index++;
            }
            else
            {
                first_line = false;
            }
            current_line_height = f_elements[i].fGetHeight();
            current_line = f_elements[i].fGetLineNo();
        }
        else
        {
            current_line_height = max(current_line_height,f_elements[i].fGetHeight());
        }
    }
    //add the last line
    if(num_lines > 1)
    {
        height+=f_line_spacing[index];
    }
    height += current_line_height;

    return height;
}

size_t HNS_Message_Justified_Element::fGetNumLines() const
{
    size_t num_lines = 0;
    size_t current_line = 0;

    for(size_t ui=0;ui<f_elements.size();ui++)
    {
        if(ui == 0)
        {
            current_line = f_elements[ui].fGetLineNo();
            num_lines++;
        }
        if(current_line != f_elements[ui].fGetLineNo())
        {
            current_line = f_elements[ui].fGetLineNo();
            num_lines++;
        }
    }

    return num_lines;
}

size_t HNS_Message_Justified_Element::fGetLastLineSpacing() const
{
    size_t result = 1;

    if(f_line_spacing.size() > 0)
    {
        result = f_line_spacing[f_line_spacing.size()-1];
    }

    return result;
}

vector<int> HNS_Message_Justified_Element::fGetFontsUsed()
{
    vector<int> result;

    for(size_t i=0;i<f_elements.size();i++)
    {
        result = AddFontToFontUsedList(result,f_elements[i].fGetFontNo());
    }

    return result;
}

size_t HNS_Message_Justified_Element::fGetYPos(const std::vector<size_t> &line_heights, const size_t &line_index) const
{
    size_t result = 0;

    if(line_index < line_heights.size())
    {
        for(size_t i=0;i<line_index;i++)
        {
            result += line_heights[i] + f_line_spacing[i];
        }
    }

    return result;
}

size_t HNS_Message_Justified_Element::fGetNumElements() const
{
    return f_elements.size();
}

HNS_Message_Element2 HNS_Message_Justified_Element::fGetElement(const size_t &index) const
{
    if(index < f_elements.size())
    {
        return f_elements[index];
    }
    else
    {
        return HNS_Message_Element2(f_parent_message);
    }
}

HNS_Message_Element2::HNS_Message_Element2(const HNS_Message2 *parent):
    f_font_no(1),
    f_is_flashing(false),
    f_char_spacing(1),
    f_width(0),
    f_height(0),
    f_line_no(0),
    f_parent_message(parent)
{
    if(f_parent_message != nullptr)
    {
        f_font_no = f_parent_message->fGetCurrentFont();
        f_is_flashing = f_parent_message->fGetCurrentFlashState();
        f_flash_info = f_parent_message->fGetCurrentFlashingInfo();
        f_char_spacing = f_parent_message->fGetCurrentCharSpacing();
    }
}

HNS_Message_Element2::HNS_Message_Element2(const HNS_Message2 *parent,const string &input, const HNS_Font &font, const int &font_no, const int &line_no):
    f_is_flashing(false),
    f_char_spacing(1),
    f_width(0),
    f_height(0),
    f_line_no(line_no),
    f_parent_message(parent)
{
    if(f_parent_message != nullptr)
    {
        if(font_no < 1)
        {
            f_font_no = f_parent_message->fGetCurrentFont();
        }
        else
        {
            f_font_no = font_no;
        }
        f_is_flashing = f_parent_message->fGetCurrentFlashState();
        f_flash_info = f_parent_message->fGetCurrentFlashingInfo();
        f_char_spacing = f_parent_message->fGetCurrentCharSpacing();
    }
    else
    {
        if(font_no < 1)
        {
            f_font_no = 1;
        }
        else
        {
            f_font_no = font_no;
        }
    }
    fSetText(input,font, f_font_no, f_is_flashing, f_flash_info);
}

void HNS_Message_Element2::fSetText(const std::string &input, const HNS_Font &font, const int &font_no, const bool &is_flashing, const HNS_Flashing_Text &flash_info)
{
    f_text = input;
    f_font_no = font_no;
    f_is_flashing = is_flashing;
    f_flash_info = flash_info;

    if(static_cast<size_t>(font.fGetFontHeight()) > f_height)
    {
        f_height = font.fGetFontHeight();
    }
    f_width = 0;
    for(size_t i=0;i<input.size();i++)
    {
        if(i!=0)
        {
            f_width+=f_char_spacing;
        }
        f_width += font.fGetCharacter(input[i]).fGetWidth();
    }
}

void HNS_Message_Element2::fSetLineNo(const size_t &line_no)
{
    f_line_no = line_no;
}

size_t HNS_Message_Element2::fGetWidth() const
{
    return f_width;
}

size_t HNS_Message_Element2::fGetHeight() const
{
    return f_height;
}

size_t HNS_Message_Element2::fGetLineNo() const
{
    return f_line_no;
}

size_t HNS_Message_Element2::fGetCharacterCount() const
{
    return f_text.size();
}

size_t HNS_Message_Element2::fGetCharacterWidth(const vector<HNS_Font> *fonts) const
{
    HNS_Character temp_character;
    size_t result = 0;
    if(static_cast<size_t>(f_font_no-1) < fonts->size())
    {
        for(size_t i=0;i<f_text.size();i++)
        {
            temp_character = fonts->at(f_font_no-1).fGetCharacter(f_text[i]);
            result += temp_character.fGetWidth();
        }
    }
    return result;
}

size_t HNS_Message_Element2::fGetCharSpacing() const
{
    return f_char_spacing;
}

string HNS_Message_Element2::fGetText() const
{
    return f_text;
}

int HNS_Message_Element2::fGetFontNo() const
{
    return f_font_no;
}

HNS_Bitmap HNS_Message_Element2::fGetBitmap(const vector<HNS_Font> *fonts) const
{
    HNS_Character temp_character;
    HNS_Bitmap result;
    if(fonts != nullptr)
    {
        if(static_cast<size_t>(f_font_no-1) < fonts->size())
        {
            for(size_t i=0;i<f_text.size();i++)
            {
                temp_character = fonts->at(f_font_no-1).fGetCharacter(f_text[i]);
                if(temp_character != HNS_Font::f_null_character)
                {
                    result.fAddCharacter(temp_character,f_char_spacing);
                }
            }
        }
    }

    return result;
}

HNS_Bitmap HNS_Message_Element2::fGetBitmapFixedWidth(const std::vector<HNS_Font> *fonts, const size_t &fixed_width)
{
    HNS_Character temp_character;
    HNS_Bitmap result;
    //sum of the widths of all characters
    size_t character_width = 0;
    size_t spacing, spacing_remainder;
    size_t temp_spacing;

    if(static_cast<size_t>(f_font_no-1) < fonts->size())
    {
        for(size_t i=0;i<f_text.size();i++)
        {
            character_width += fonts->at(f_font_no-1).fGetCharacter(f_text[i]).fGetWidth();
        }
    }

    if((character_width + (f_text.size()-1)) > fixed_width)
    {
        //fixed_width does not provide enough space
        return result;
    }

    if((f_text.size() -1) > 0)
    {
        spacing = (fixed_width - character_width) / (f_text.size()-1);
        spacing_remainder = (fixed_width - character_width) % (f_text.size()-1);
    }
    else
    {
        spacing = 0;
        spacing_remainder = 0;
    }

    if(static_cast<size_t>(f_font_no-1) < fonts->size())
    {
        for(size_t i=0;i<f_text.size();i++)
        {
            temp_character = fonts->at(f_font_no-1).fGetCharacter(f_text[i]);
            if(temp_character != HNS_Font::f_null_character)
            {
                temp_spacing = spacing;
                if(spacing_remainder > 0)
                {
                    temp_spacing++;
                    spacing_remainder--;
                }
                result.fAddCharacter(temp_character,temp_spacing);
            }
        }
    }
    f_width = result.fGetWidth();
    f_height = result.fGetHeight();

    return result;
}

HNS_Field_Data::HNS_Field_Data():
    f_current_time(1),
    f_speed(90),
    f_radar_connected(false),
    f_temperature(27)
{

}

HNS_Field_Data::HNS_Field_Data(const time_t &input_time, const int &speed, const int &temperature, const bool &kph, const bool &celsius):
    f_current_time(input_time),
    f_radar_connected(false)
{
    fSetSpeed(speed,kph);
    fSetTemperature(temperature,celsius);
}

void HNS_Field_Data::fSetSpeed(const int &speed, const bool &kph)
{
    if(kph)
    {
        f_speed = speed;
    }
    else
    {
        f_speed = static_cast<double>(speed) / 0.621371;
    }
}

void HNS_Field_Data::fSetRadarConnected(const bool &radar_connected)
{
    f_radar_connected = radar_connected;
}

void HNS_Field_Data::fSetTemperature(const int &temperature, const bool &celsius)
{
    if(celsius)
    {
        f_temperature = temperature;
    }
    else
    {
        f_temperature = (static_cast<double>(temperature) - 32.0) * (5.0/9.0);
    }
}

void HNS_Field_Data::fSetTime(const time_t &input_time)
{
    f_current_time = input_time;
}

int HNS_Field_Data::fGetSpeed(const bool &kph) const
{
    if(kph)
    {
        return f_speed;
    }
    else
    {
        return static_cast<int>(round(f_speed * 0.621371));
    }
}

bool HNS_Field_Data::fGetRadarConnected() const
{
    return f_radar_connected;
}

int HNS_Field_Data::fGetTemperature(const bool &celsius) const
{
    if(celsius)
    {
        return f_temperature;
    }
    else
    {
        return static_cast<int>(round((f_temperature * (9.0/5.0)) + 32.0));
    }
}

tm *HNS_Field_Data::fGetTime() const
{
    return localtime(&f_current_time);
}
