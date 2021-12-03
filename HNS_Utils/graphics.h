#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include "fonts.h"

class HNS_Character;

class HNS_Point
{
public:
    HNS_Point();
    HNS_Point(const size_t &x, const size_t &y);

    size_t &fX();
    void fSetX(const size_t &x);
    size_t fGetX() const;

    size_t &fY();
    void fSetY(const size_t &y);
    size_t fGetY() const;
private:
    size_t f_x,f_y;
};

class HNS_Color
{
public:
    HNS_Color();
    //RGB Constructor
    HNS_Color(const unsigned char &r, const unsigned char &g, const unsigned char &b);
    //Greyscale Constructor
    HNS_Color(const unsigned char &greyscale);

    unsigned char &fR();
    void fSetR(const int &r);
    unsigned char fGetR() const;

    unsigned char &fG();
    void fSetG(const int &g);
    unsigned char fGetG() const;

    unsigned char &fB();
    void fSetB(const int &b);
    unsigned char fGetB() const;

    bool operator == (const HNS_Color &rhs) const;
    bool operator != (const HNS_Color &rhs) const;
private:
    unsigned char f_r,f_g,f_b;
};

class HNS_Bitmap
{
public:
    HNS_Bitmap();
    HNS_Bitmap(const size_t &height,const size_t &width);
    HNS_Bitmap(const bool *bitmap, const size_t &height, const size_t &width);
    HNS_Bitmap(const std::vector<unsigned char> &bitmap, const size_t height, const size_t &width);

    bool fSetPixel(const size_t &x, const size_t &y, const HNS_Color &color);
    //for monochrome 1 bit mode
    bool fSetPixel(const size_t &x, const size_t &y, const bool &onoff);

    HNS_Color fGetPixel(const size_t &x, const size_t &y) const;

    void fCopy(const HNS_Bitmap &bitmap, const size_t &dest_x, const size_t &dest_y);

    void fResize(const size_t &height, const size_t &width);

    size_t fGetHeight() const;
    size_t fGetWidth() const;

    bool fAddCharacter(const HNS_Character &character, const size_t &char_spacing);
private:
    size_t fXYtoIndex(const HNS_Point &point, bool *outofbounds = nullptr) const;
    size_t fXYtoIndex(const size_t &x, const size_t &y, bool *outofbounds = nullptr) const;
    HNS_Point fIndextoXY(const size_t &index, bool *outofbounds = nullptr) const;
    void fIndextoXY(const size_t &index, size_t &x, size_t &y, bool *outofbounds = nullptr) const;

    bool fIsInbounds(const HNS_Point &point) const;
    bool fIsInbounds(const size_t &x, const size_t &y) const;

    std::vector<HNS_Color> f_bitmap;
    size_t f_height,f_width;
};

//The graphic as defined by NTCIP.  Including the name and number as well as the bitmap
class HNS_Graphic
{
public:
    HNS_Graphic();
    HNS_Graphic(const HNS_Bitmap &bitmap,const int &number, const std::string name);

    HNS_Bitmap fGetBitmap() const;
    void fSetBitmap(const HNS_Bitmap &bitmap);
    HNS_Bitmap &fBitmap();

    int fGetGraphicNumber() const;
    void fSetGraphicNumber(const int &number);
    int &fGraphicNumber();

    std::string fGetName() const;
    void fSetName(const std::string &name);
    std::string &fName();
private:
    HNS_Bitmap f_bitmap;
    int f_graphic_number;
    std::string f_name;
};

#endif // GRAPHICS_H
