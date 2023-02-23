#include "PixelOut.h"

using namespace std;

HNS_PixelOut::HNS_PixelOut():
    f_byte(0)
  , f_led(0)
{

}

HNS_PixelOut::HNS_PixelOut(const size_t &byte, const size_t &led):
    f_byte(byte)
  , f_led(led)
{

}

size_t HNS_PixelOut::fByte() const
{
    return f_byte;
}

size_t &HNS_PixelOut::fByte()
{
    return f_byte;
}

size_t HNS_PixelOut::fLED() const
{
    return f_led;
}

size_t &HNS_PixelOut::fLED()
{
    return f_led;
}

bool HNS_PixelOut::operator == (const HNS_PixelOut &rhs) const
{
    return (f_led == rhs.f_led)
        && (f_byte == rhs.f_byte);
}

bool HNS_PixelOut::operator != (const HNS_PixelOut &rhs) const
{
    return !(*this == rhs);
}

vector <HNS_PixelOut> PixelsOut_from_Byte(const unsigned char &data_byte, const size_t &byte_no)
{
    vector<HNS_PixelOut> result;

    if(data_byte != 0)
    {
        unsigned char mask = 0x1;
        for(size_t ui = 0; ui < 8; ui++)
        {
            if(data_byte & mask)
            {
                result.push_back(HNS_PixelOut(byte_no,ui));
            }
            mask = mask << 1;
        }
    }
    //if data byte is 0, there are no pixels out
    return result;
}
