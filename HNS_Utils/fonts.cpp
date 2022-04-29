/*
 * fonts.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: mchristiso
 */

#include "stdafx.h"

#include "fonts.h"
#include <utilities.h>
#include <CRC.h>


using namespace std;

HNS_Character HNS_Font::f_null_character = HNS_Character(nullptr,0,0,0);

HNS_Font::HNS_Font():
f_font_number(0),
f_maxheight(0),
f_maxwidth(0),
f_char_spacing(1),
f_line_spacing(1)
{
}

HNS_Font::~HNS_Font()
{

}

HNS_Character HNS_Font::fGetCharacter(const int &character_number) const
{
	size_t i;
	for(i=0;i<f_characters.size();i++)
	{
		if(f_characters[i].fGetCharacterNumber() == character_number)
		{
			return f_characters[i];
		}
	}
	return f_null_character;
}

size_t HNS_Font::fGetNumCharacters() const
{
    return f_characters.size();
}

HNS_Character HNS_Font::fGetCharacterByIndex(const size_t &index) const
{
    if(index < f_characters.size())
    {
        return f_characters[index];
    }
    return f_null_character;
}

void HNS_Font::fAddCharacter(const HNS_Character &character)
{
	size_t i;
	for(i=0;i<f_characters.size();i++)
	{
		if(f_characters[i].fGetCharacterNumber() == character.fGetCharacterNumber())
		{
			f_characters[i] = character;
			if(character.fGetHeight() > f_maxheight)
			{
				f_maxheight = character.fGetHeight();
			}
			if(character.fGetWidth() > f_maxwidth)
			{
				f_maxwidth = character.fGetWidth();
			}
			return;
		}
	}
	f_characters.push_back(character);
	if(character.fGetHeight() > f_maxheight)
	{
		f_maxheight = character.fGetHeight();
	}
	if(character.fGetWidth() > f_maxwidth)
	{
		f_maxwidth = character.fGetWidth();
	}
}

void HNS_Font::fChangeCharacter(const HNS_Character &character, const int &character_number)
{
    f_maxheight = 0;
    f_maxwidth = 0;
    for(vector<HNS_Character>::iterator it = f_characters.begin() ; it != f_characters.end(); ++it)
    {
        if(it->fGetCharacterNumber() == character_number)
        {
            *it = character;
        }
        else
        {
            if(it->fGetHeight() > f_maxheight)
            {
                f_maxheight = it->fGetHeight();
            }
            if(it->fGetWidth() > f_maxwidth)
            {
                f_maxwidth = it->fGetWidth();
            }
        }
    }
}

void HNS_Font::fRemoveCharacter(const int &character_number)
{
	f_maxheight = 0;
	f_maxwidth = 0;
	for(vector<HNS_Character>::iterator it = f_characters.begin() ; it != f_characters.end(); ++it)
	{
		if(it->fGetCharacterNumber() == character_number)
		{
			f_characters.erase(it);
		}
		else
		{
			if(it->fGetHeight() > f_maxheight)
			{
				f_maxheight = it->fGetHeight();
			}
			if(it->fGetWidth() > f_maxwidth)
			{
				f_maxwidth = it->fGetWidth();
			}
		}
	}
}

void HNS_Font::fSetFontNumber(const int &font_number)
{
	f_font_number = font_number;
}

int HNS_Font::fGetFontNumber() const
{
	return f_font_number;
}

bool HNS_Font::fCharacterExists(const int &character_number) const
{
	size_t i;
	for(i=0;i<f_characters.size();i++)
	{
		if(f_characters[i].fGetCharacterNumber() == character_number)
		{
			return true;
		}
	}

	return false;
}

int HNS_Font::fGetFontHeight() const
{
	return f_maxheight;
}

void HNS_Font::fSetFontHeight(const int &height)
{
    f_maxheight = height;
}

int HNS_Font::fGetMaxFontWidth() const
{
	return f_maxwidth;
}

void HNS_Font::fSetLineSpacing(const int &spacing)
{
    f_line_spacing = spacing;
}

int HNS_Font::fGetLineSpacing() const
{
    return f_line_spacing;
}

void HNS_Font::fSetCharSpacing(const int &spacing)
{
    f_char_spacing = spacing;
}

int HNS_Font::fGetCharSpacing() const
{
    return f_char_spacing;
}

void HNS_Font::fSetFontName(const string &name)
{
    f_fontname = name;
}

string HNS_Font::fGetFontName() const
{
    return f_fontname;
}

//This follows the FontInfoID as defined in NTCIP1203 5.4.2.7, page 143 to page 145.  This follows the description not the example
//which doesn't match the description
uint16_t HNS_Font::fGetVersionID() const
{
    uint16_t result = 0,itemp;
    size_t i,ilimit;

    vector<unsigned char> bytestream,temp_vec;

    //encode the font information sequence
    bytestream.push_back(static_cast<unsigned char>(f_font_number));
    bytestream.push_back(static_cast<unsigned char>(f_maxheight));
    bytestream.push_back(static_cast<unsigned char>(f_char_spacing));
    bytestream.push_back(static_cast<unsigned char>(f_line_spacing));

    ilimit = min(MAX_NUM_CHARACTERS,f_characters.size());

    //this is the quantity field of the "sequence of" portion, charinfolist
    temp_vec = ConvSizeT_to_Legnth(ilimit);
    bytestream.insert(bytestream.end(),temp_vec.begin(),temp_vec.end());

    for(i = 0;i<f_characters.size();i++)
    {
        //encode the character information segment.
        //start with character number
        temp_vec = HNS_intToByteArray(f_characters[i].fGetCharacterNumber());
        //trim it down to two bytes
        while(temp_vec.size() > 2)
        {
            temp_vec.erase(temp_vec.begin());
        }
        bytestream.insert(bytestream.end(),temp_vec.begin(),temp_vec.end());

        //character width
        bytestream.push_back(static_cast<unsigned char>(f_characters[i].fGetWidth()));

        //character bitmap
        temp_vec = f_characters[i].fGetNTCIPData();
        bytestream.insert(bytestream.end(),temp_vec.begin(),temp_vec.end());
    }

    //do not insert nonvalid characters.  Not sure if this is right or not.

    itemp = CalcCRC(bytestream);

    result = itemp >> 8;
    result = result | ((itemp & 0xFF) << 8);

    return result;
}

HNS_Character::HNS_Character():
f_character_number(0),
f_width(5),
f_height(7),
f_data(nullptr)
{

}

HNS_Character::~HNS_Character()
{
	if(f_data != nullptr)
	{
		delete [] f_data;
		f_data = nullptr;
	}
}

HNS_Character::HNS_Character(const bool *data, const int &width, const int &height, const int &character_number):
f_character_number(character_number),
f_width(width),
f_height(height),
f_data(nullptr),
f_datasize(0)
{
	int i;
	if((f_width * f_height > 0) && data != nullptr)
	{
        f_data = new bool [f_width * f_height];
		for(i=0;i < (f_width * f_height); i++)
		{
			f_data[i] = data[i];
		}
	}
}

HNS_Character &HNS_Character::operator=(const HNS_Character &rhs)
{
	int i;

	delete [] f_data;
	f_data = nullptr;

	f_character_number = rhs.f_character_number;
	f_width = rhs.f_width;
	f_height = rhs.f_height;
	f_datasize = rhs.f_datasize;

	if(f_datasize > 0 && rhs.f_data != nullptr)
	{
        f_data = new bool [f_datasize];

		for(i=0; i < f_datasize;i++)
		{
			f_data[i] = rhs.f_data[i];
		}
	}

	return *this;
}

HNS_Character::HNS_Character(const HNS_Character &copy):
f_character_number(copy.f_character_number),
f_width(copy.f_width),
f_height(copy.f_height),
f_data(nullptr)
{
	*this = copy;
}

void HNS_Character::fSetData(const HNS_Bitmap &data)
{
    bool *pix_data = nullptr;

    if((data.fGetHeight() * data.fGetWidth()) > 0)
    {
        pix_data = new bool[data.fGetHeight() * data.fGetWidth()];

        for(size_t uy = 0; uy < data.fGetHeight(); uy++)
        {
            for(size_t ux = 0; ux < data.fGetWidth();ux++)
            {
                pix_data[uy*data.fGetWidth() + ux] = !(data.fGetPixel(ux,uy) == HNS_Color(0,0,0));
            }
        }

        fSetData(pix_data,data.fGetHeight()*data.fGetWidth(),data.fGetWidth(),data.fGetHeight());
    }
}

void HNS_Character::fSetData(const std::vector<unsigned char> &data, const int &width, const int &height)
{
    bool *result = new bool[width*height];
    unsigned char itemp;

    for(int i=0;i<(width * height);i++)
    {
        if(i%8 == 0)
        {
            if(static_cast<size_t>(i/8) < data.size())
            {
                itemp = data[i/8];
            }
            else
            {
                break;
            }
        }
        result[i] = ((itemp & 0x80) != 0);
        itemp = itemp << 1;
    }

    fSetData(result,width*height,width,height);

    delete [] result;
}

void HNS_Character::fSetData(const bool *data, const long long &datasize,  const int &width, const int &height)
{
	long long i;

	if(f_data != nullptr)
	{
		delete [] f_data;
		f_data = nullptr;
	}
    f_data = new bool [datasize];
	f_width = width;
	f_height = height;
	f_datasize = datasize;

	for(i=0;i<f_datasize;i++)
	{
		f_data[i] = data[i];
	}
}

bool *HNS_Character::fGetData() const
{
	return f_data;
}

HNS_Bitmap HNS_Character::fGetBitmap() const
{
    HNS_Bitmap result(f_data,f_height,f_width);

    return result;
}

vector<unsigned char> HNS_Character::fGetNTCIPData() const
{
    unsigned char temp_char = 0;
    vector<unsigned char> result;
    size_t i;
    for(i=0;i<static_cast<size_t>(f_width*f_height);i++)
    {
        if(((i%8) == 0) && (i != 0))
        {
            result.push_back(temp_char);
            temp_char = 0;
        }
        else
        {
            temp_char = temp_char << 1;
        }
        temp_char = temp_char | (f_data[i] ? 0x1 : 0x0);
    }
    //don't forget any dangling bits!
    if(i%8 > 0)
    {
        temp_char = temp_char << (8 - (i%8));
    }
    result.push_back(temp_char);
    return result;
}

void HNS_Character::fSetCharacterNumber(const int &character_number)
{
	f_character_number = character_number;
}

int HNS_Character::fGetCharacterNumber() const
{
	return f_character_number;
}

int HNS_Character::fGetHeight() const
{
	return f_height;
}

int HNS_Character::fGetWidth() const
{
	return f_width;
}

bool HNS_Character::operator ==(const HNS_Character &rhs)
{
    bool is_same = true;
    is_same = (f_character_number == rhs.f_character_number)
        && (f_width == rhs.f_width)
        && (f_height == rhs.f_height)
        && (f_datasize == rhs.f_datasize);

    if(is_same)
    {
        for(int i=0;i<f_datasize;i++)
        {
            is_same = (f_data[i] == rhs.f_data[i]);
            if(!is_same)
            {
                break;
            }
        }
    }

    return is_same;
}

bool HNS_Character::operator !=(const HNS_Character &rhs)
{
    return !(*this == rhs);
}
