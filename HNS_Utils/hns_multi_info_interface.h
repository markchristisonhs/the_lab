#ifndef HNS_MULTI_INFO_INTERFACE_H
#define HNS_MULTI_INFO_INTERFACE_H

#include <fonts.h>
#include <graphics.h>
#include <vector>

class HNS_Multi_Info_Interface
{
public:
    virtual HNS_Font fGetFont(const int &font_number, bool &valid) = 0;
    virtual std::vector<HNS_Font> fGetFonts() = 0;
    virtual bool fIsValidFontNumber(const int &font_number) = 0;
    virtual int fGetDefaultFont() = 0;
    virtual int fGetDefaultFontActivate() = 0;

    virtual HNS_Graphic fGetGraphic(const int &graphic_number, bool &valid) = 0;
    virtual std::vector<HNS_Graphic> fGetGraphics() = 0;

    virtual int64_t fGetLocalTime() = 0;
    virtual int fGetTemperature() = 0;
    virtual int fGetSpeed() = 0;

    virtual int fGetDefaultForegroundColor() = 0;
    virtual int fGetDefaultBackgroundColor() = 0;

    virtual int fGetDefaultFlashOnTime() = 0;
    virtual int fGetDefaultFlashOnTimeActivate() = 0;

    virtual int fGetDefaultFlashOffTime() = 0;
    virtual int fGetDefaultFlashOffTimeActivate() = 0;

    virtual int fGetDefaultLineJustification() = 0;
    virtual int fGetDefaultLineJustificationActivate() = 0;

    virtual int fGetDefaultPageJustification() = 0;
    virtual int fGetDefaultPageJustificationActivate() = 0;

    virtual int fGetDefaultPageTimeOn() = 0;
    virtual int fGetDefaultPageTimeOnActivate() = 0;

    virtual int fGetDefaultPageTimeOff() = 0;
    virtual int fGetDefaultPageTimeOffActivate() = 0;

    virtual std::vector<uint8_t> fGetDefaultBackgroundColorRGB() = 0;
    virtual std::vector<uint8_t> fGetDefaultBackgroundColorRGBActivate() = 0;

    virtual std::vector<uint8_t> fGetDefaultForegroundColorRGB() = 0;
    virtual std::vector<uint8_t> fGetDefaultForegroundColorRGBActivate() = 0;
};

#endif // HNS_MULTI_INFO_INTERFACE_H
