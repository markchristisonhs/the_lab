#ifndef PIXELOUT_H
#define PIXELOUT_H

#include <vector>

//carries information on a pixel outage.  Which byte, which LED
class HNS_PixelOut
{
public:
    HNS_PixelOut();
    HNS_PixelOut(const size_t &byte, const size_t &led);

    size_t fByte() const;
    size_t &fByte();

    size_t fLED() const;
    size_t &fLED();

    bool operator == (const HNS_PixelOut &rhs) const;
    bool operator != (const HNS_PixelOut &rhs) const;
private:
    size_t f_byte,f_led;
};

//translates a byte, such as what might be received from the RS485 bus, into a vector of pixels out
std::vector <HNS_PixelOut> PixelsOut_from_Byte(const unsigned char &data_byte, const size_t &byte_no);

#endif // PIXELOUT_H
