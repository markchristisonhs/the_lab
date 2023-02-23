#include "graphics.h"
#include <utilities.h>

using namespace std;

HNS_Point::HNS_Point():
    f_x(0),f_y(0)
{

}

HNS_Point::HNS_Point(const size_t &x, const size_t &y):
    f_x(x),f_y(y)
{

}

size_t &HNS_Point::fX()
{
    return f_x;
}

void HNS_Point::fSetX(const size_t &x)
{
    f_x = x;
}

size_t HNS_Point::fGetX() const
{
    return f_x;
}

size_t &HNS_Point::fY()
{
    return f_y;
}

void HNS_Point::fSetY(const size_t &y)
{
    f_y = y;
}

size_t HNS_Point::fGetY() const
{
    return f_y;
}

HNS_Color::HNS_Color():
f_r(0),
f_g(0),
f_b(0)
{

}

HNS_Color::HNS_Color(const unsigned char &r, const unsigned char &g, const unsigned char &b):
f_r(r),
f_g(g),
f_b(b)
{

}

HNS_Color::HNS_Color(const unsigned char &greyscale):
    f_r(greyscale)
  , f_g(greyscale)
  , f_b(greyscale)
{

}

unsigned char &HNS_Color::fR()
{
    return f_r;
}

void HNS_Color::fSetR(const int &r)
{
    f_r = r;
}

unsigned char HNS_Color::fGetR() const
{
    return f_r;
}

unsigned char &HNS_Color::fG()
{
    return f_g;
}

void HNS_Color::fSetG(const int &g)
{
    f_g = g;
}

unsigned char HNS_Color::fGetG() const
{
    return f_g;
}

unsigned char &HNS_Color::fB()
{
    return f_b;
}

void HNS_Color::fSetB(const int &b)
{
    f_b = b;
}

unsigned char HNS_Color::fGetB() const
{
    return f_b;
}

bool HNS_Color::operator == (const HNS_Color &rhs) const
{
    return (f_r == rhs.f_r)
            && (f_g == rhs.f_g)
            && (f_b == rhs.f_b);
}

bool HNS_Color::operator !=(const HNS_Color &rhs) const
{
    return !(*this == rhs);
}

HNS_Bitmap::HNS_Bitmap():
    f_height(0),
    f_width(0)
{

}

HNS_Bitmap::HNS_Bitmap(const size_t &height, const size_t &width):
    f_bitmap(vector<HNS_Color>(width*height)),
    f_height(height),
    f_width(width)
{

}

HNS_Bitmap::HNS_Bitmap(const bool *bitmap, const size_t &height, const size_t &width):
    f_bitmap(vector<HNS_Color>(width*height)),
    f_height(height),
    f_width(width)
{
    HNS_Point temp_point;
    for(size_t i=0; i<height*width; i++)
    {
        temp_point = fIndextoXY(i);
        fSetPixel(temp_point.fGetX(),temp_point.fGetY(),bitmap[i]);
    }
}

HNS_Bitmap::HNS_Bitmap(const std::vector<unsigned char> &bitmap, const size_t height, const size_t &width):
    f_bitmap(vector<HNS_Color>(width*height)),
    f_height(height),
    f_width(width)
{
    HNS_Point temp_point;
    size_t index = 0;
    unsigned char itemp;
    bool btemp,bitmap_complete = false;
    bool outofrange = false;
    for(size_t i=0;i<bitmap.size();i++)
    {
        itemp = bitmap[i];
        for(size_t j=0;j<8;j++)
        {
            btemp = itemp & 0x80;
            temp_point = fIndextoXY(index, &outofrange);
            if(!outofrange)
            {
                fSetPixel(temp_point.fGetX(),temp_point.fGetY(),btemp);
                index++;
                itemp = itemp << 1;
                if(index > (f_height * f_width))
                {
                    bitmap_complete = true;
                    break;
                }
            }
            else
            {
                bitmap_complete = true;
                break;
            }
        }
        if(bitmap_complete)
        {
            break;
        }
    }
}

bool HNS_Bitmap::fSetPixel(const size_t &x, const size_t &y, const HNS_Color &color)
{
    if(fIsInbounds(x,y))
    {
        size_t index;

        index = fXYtoIndex(x,y);

        f_bitmap[index] = color;

        return true;
    }
    else
    {
        return false;
    }
}

bool HNS_Bitmap::fSetPixel(const size_t &x, const size_t &y, const bool &onoff)
{
    return fSetPixel(x,y,onoff ? HNS_Color(0xFF,0xFF,0xFF) : HNS_Color(0,0,0));
}

HNS_Color HNS_Bitmap::fGetPixel(const size_t &x, const size_t &y) const
{
    if(fIsInbounds(x,y))
    {
        return f_bitmap[fXYtoIndex(x,y)];
    }
    else
    {
        return HNS_Color(0,0,0);
    }
}

void HNS_Bitmap::fCopy(const HNS_Bitmap &bitmap, const size_t &dest_x, const size_t &dest_y)
{
    bool outofbounds;
    size_t index;
    for(size_t copy_y=0;copy_y<bitmap.fGetHeight();copy_y++)
    {
        for(size_t copy_x=0;copy_x<bitmap.fGetWidth();copy_x++)
        {
            index = fXYtoIndex(dest_x+copy_x,dest_y+copy_y,&outofbounds);
            if(!outofbounds)
            {
                f_bitmap[index] = bitmap.fGetPixel(copy_x,copy_y);
            }
        }
    }
}

void HNS_Bitmap::fResize(const size_t &height, const size_t &width)
{
    HNS_Bitmap temp_bitmap = *this;

    f_bitmap = vector<HNS_Color>(height*width);
    f_height = height;
    f_width = width;

    fCopy(temp_bitmap,0,0);
}

size_t HNS_Bitmap::fGetHeight() const
{
    return f_height;
}

size_t HNS_Bitmap::fGetWidth() const
{
    return f_width;
}

bool HNS_Bitmap::fAddCharacter(const HNS_Character &character, const size_t &char_spacing)
{
    size_t old_width = fGetWidth();

    bool success = true;

    if(f_bitmap.size() == 0)
    {
        fResize(character.fGetHeight(),character.fGetWidth());
        fCopy(HNS_Bitmap(character.fGetData(),character.fGetHeight(),character.fGetWidth()),0,0);
    }
    else
    {
        //Assume that all characters using this function are the same height
        if(character.fGetHeight() != static_cast<int>(fGetHeight()))
        {
            success = false;
        }
        else
        {
            fResize(character.fGetHeight(),fGetWidth()+character.fGetWidth()+char_spacing);
            fCopy(HNS_Bitmap(character.fGetData(),character.fGetHeight(),character.fGetWidth()),old_width+char_spacing,0);
        }
    }
    return success;
}

vector<unsigned char> HNS_Bitmap::fGetByteArray() const
{
    vector<unsigned char> answer, temp_vec;

    for(size_t y=0; y< f_height;y++)
    {
        for(size_t x=0; x< f_width;x++)
        {
            if(fGetPixel(x,y) == HNS_Color(0,0,0))
            {
                temp_vec.push_back(0);
            }
            else
            {
                temp_vec.push_back(1);
            }
        }
    }

    answer = BoolArray_to_ByteArray(temp_vec);

    return answer;
}

size_t HNS_Bitmap::fXYtoIndex(const HNS_Point &point, bool *outofbounds) const
{
    if(!fIsInbounds(point))
    {
        if(outofbounds != nullptr)
        {
            *outofbounds = true;
        }
        return 0;
    }
    else
    {
        if(outofbounds != nullptr)
        {
            *outofbounds = false;
        }
        return (point.fGetY() * f_width) + point.fGetX();
    }
}

size_t HNS_Bitmap::fXYtoIndex(const size_t &x, const size_t &y, bool *outofbounds) const
{
    return fXYtoIndex(HNS_Point(x,y),outofbounds);
}

HNS_Point HNS_Bitmap::fIndextoXY(const size_t &index, bool *outofbounds) const
{
    if(index >= f_bitmap.size())
    {
        if(outofbounds != nullptr)
        {
            *outofbounds = true;
        }
        return HNS_Point();
    }
    else
    {
        if(outofbounds != nullptr)
        {
            *outofbounds = false;
        }
        return HNS_Point( (index - ((index/f_width)*f_width)), (index/f_width) );
    }
}

void HNS_Bitmap::fIndextoXY(const size_t &index, size_t &x, size_t &y, bool *outofbounds) const
{
    HNS_Point temp = fIndextoXY(index,outofbounds);
    x = temp.fX();
    y = temp.fY();
}

bool HNS_Bitmap::fIsInbounds(const HNS_Point &point) const
{
    return (point.fGetX() < f_width) && (point.fGetY() < f_height);
}

bool HNS_Bitmap::fIsInbounds(const size_t &x, const size_t &y) const
{
    return fIsInbounds(HNS_Point(x,y));
}

HNS_Graphic::HNS_Graphic():
    f_graphic_number(0)
{

}

HNS_Graphic::HNS_Graphic(const HNS_Bitmap &bitmap, const int &number, const string name):
    f_graphic_number(number),
    f_name(name)
{
    f_bitmap = bitmap;
}

HNS_Bitmap HNS_Graphic::fGetBitmap() const
{
    return f_bitmap;
}

void HNS_Graphic::fSetBitmap(const HNS_Bitmap &bitmap)
{
    f_bitmap = bitmap;
}

HNS_Bitmap &HNS_Graphic::fBitmap()
{
    return f_bitmap;
}

int HNS_Graphic::fGetGraphicNumber() const
{
    return f_graphic_number;
}

void HNS_Graphic::fSetGraphicNumber(const int &number)
{
    f_graphic_number = number;
}

int &HNS_Graphic::fGraphicNumber()
{
    return f_graphic_number;
}

string HNS_Graphic::fGetName() const
{
    return f_name;
}

void HNS_Graphic::fSetName(const string &name)
{
    f_name = name;
}

string &HNS_Graphic::fName()
{
    return f_name;
}
