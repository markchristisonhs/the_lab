/*
 * message.h
 *
 *  Created on: Aug 14, 2019
 *      Author: mchristiso
 */

#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#include <vector>
#include <string>
#include "fonts.h"
#include "graphics.h"
#include "MessageBoard.h"

typedef enum
{
	HNS_JUSTIFICATION_LINE_CENTER = 0,
	HNS_JUSTIFICATION_LEFT,
    HNS_JUSTIFICATION_RIGHT,
    HNS_JUSTIFICATION_FULL
} type_justification_line;

typedef enum
{
	HNS_JUSTIFICATION_PAGE_CENTER = 0,
	HNS_JUSTIFICATION_TOP,
	HNS_JUSTIFICATION_BOTTOM
} type_justification_page;

typedef enum
{
	HNS_MESSAGE_ELEMENT_IMAGE = 0,
	HNS_MESSAGE_ELEMENT_TEXT
} type_message_element_type;

typedef enum
{
	HNS_MULTI_PARSER_NO_ERROR = 0,
	HNS_MULTI_PARSER_UNEXPECTED_CLOSING_BRACE,
	HNS_MULTI_PARSER_INCOMPLETE_TAG,
	HNS_MULTI_PARSER_UNRECOGNIZED_TAG,
	HNS_MULTI_PARSER_UNSUPPORTED_TAG,
    HNS_MULTI_PARSER_SYNTAX_ERROR,
    HNS_MULTI_PARSER_UNDEFINED_GRAPHIC
} type_mutli_parser_error;

typedef enum
{
    HNS_MULTI_SYNTAX_ERROR_OTHER = 1,
    HNS_MULTI_SYNTAX_ERROR_NONE = 2,
    HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG = 3,
    HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAGVALUE = 4,
    HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG = 5,
    HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED = 6,
    HNS_MULTI_SYNTAX_ERROR_CHARACTERNOTDEFINED = 7,
    HNS_MULTI_SYNTAX_ERROR_FIELDDEVICENOTEXIST = 8,
    HNS_MULTI_SYNTAX_ERROR_FIELDDEVICEERROR = 9,
    HNS_MULTI_SYNTAX_ERROR_FLASHREGIONERROR = 10,
    HNS_MULTI_SYNTAX_ERROR_TAGCONFLICT = 11,
    HNS_MULTI_SYNTAX_ERROR_TOOMANYPAGES = 12,
    HNS_MULTI_SYNTAX_ERROR_FONTVERSIONID = 13,
    HNS_MULTI_SYNTAX_ERROR_GRAPHICID = 14,
    HNS_MULTI_SYNTAX_ERROR_GRAPHICNOTDEFINED = 15
} type_multi_syntax_error;

class HNS_Message_Element2;
class HNS_Message;
class HNS_Message_Page;
class HNS_Message_Element;
class HNS_Message_Justified_Element;
class HNS_Message_Page2;
class HNS_Field_Data;
class HNS_SignBoard;

class HNS_Flashing_Text
{
public:
    HNS_Flashing_Text();
    HNS_Flashing_Text(const unsigned int &flash_on, const unsigned int &flash_off, const bool &on_first);

    unsigned int &fFlashOn();
    void fSetFlashOn(const unsigned int &flash_on);
    unsigned int fGetFlashOn() const;

    unsigned int &fFlashOff();
    void fSetFlashOff(const unsigned int &flash_off);
    unsigned int fGetFlashOff() const;

    bool &fOnFirst();
    void fSetOnFirst(const bool &on_first);
    bool fGetOnFirst() const;
private:
    //flash times in whole numbers of tenths of seconds.
    unsigned int f_flash_on, f_flash_off;
    bool f_on_first;
};

class HNS_Message2
{
public:
    HNS_Message2();
    HNS_Message2(const HNS_SignBoard_Info &signboard_info);

    int fSetMULTI(std::string &multi_string, const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const HNS_Field_Data *field_data, type_multi_syntax_error *multi_error = nullptr, bool *too_tall = nullptr);
    std::string fGetMULTI();
    size_t fGetNumPages() const;

    type_justification_line fGetLastLineJustification(const size_t &page);
    type_justification_page fGetLastPageJustification(const size_t &page);

    double fGetPageTimeOn(const size_t &page) const;
    double fGetPageTimeOff(const size_t &page) const;
    size_t fGetBoardWidth() const;
    size_t fGetBoardHeight() const;

    void fSetFontTest(const std::vector<HNS_Font> *fonts, const size_t font_no);

    static type_justification_line fGetDefaultLineJustification();
    static void fSetDefaultLineJustification(const type_justification_line &line_justification);

    static type_justification_page fGetDefaultPageJustification();
    static void fSetDefaultPageJustification(const type_justification_page &page_justification);

    static double fGetDefaultPageTimeOn();
    static void fSetDefaultPageTimeOn(const double &pagetime_on);

    static double fGetDefaultPageTimeOff();
    static void fSetDefaultPageTimeOff(const double &pagetime_off);

    static double fGetDefaultFlashTimeOn();
    static void fSetDefaultFlashTimeOn(const double &flashtime_on);

    static double fGetDefaultFlashTimeOff();
    static void fSetDefaultFlashTimeOff(const double &flashtime_off);

    static int fGetDefaultFont();
    static void fSetDefaultFont(const int &font);

    static HNS_SignBoard_Info fGetDefaultSignBoardInfo() {return f_default_signboard_info;}
    static void fSetDefaultSignBoardInfo(const HNS_SignBoard_Info &sign_board_info) {f_default_signboard_info = sign_board_info;}

    HNS_SignBoard fGetSignBoard(const int64_t &time, size_t *page_displayed = nullptr);
    HNS_Message_Page2 fGetPage(const size_t &page_no) const;
private:
    void fAddPage(const double &page_time_on, const double &page_time_off);
    type_hns_signboard_error fAddElementToPage(HNS_Message_Element2 &element, const type_justification_line &line_justification, const type_justification_page &page_justification, const bool &newline = false, const ssize_t &line_spacing = -1);
    void fSetJustification(const type_justification_line &line_justification, const type_justification_page &page_justification);
    void fResetMulti();
    size_t fGetPageIndexFromTime(const int64_t &time, unsigned int &time_in_page);
    unsigned int fGetMessagePeriod() const;
    std::vector<HNS_Message_Page2> f_pages;

    static type_justification_line f_default_line_justification;
    static type_justification_page f_default_page_justification;
    static double f_default_page_time_on, f_default_page_time_off;
    static unsigned int f_default_flash_time_on, f_default_flash_time_off;
    static int f_default_font;
    static HNS_SignBoard_Info f_default_signboard_info;

    //the currently set multistring
    std::string f_multi;
};

class HNS_Message_Page2
{
public:
    HNS_Message_Page2();
    HNS_Message_Page2(const double &page_time_on, const double &page_time_off,const HNS_SignBoard_Info &sign_board_info);

    void fNewJustification(const type_justification_line &line_justification, const type_justification_page &page_justification);
    type_hns_signboard_error fAddElement(HNS_Message_Element2 &element,const type_justification_line &line_justification, const type_justification_page &page_justification, const bool &newline = false, const ssize_t &line_spacing = -1, const ssize_t &char_spacing = -1);
    size_t fGetNumElements() const;
    void fAddNewline();

    double fGetPageTimeOn() const;
    double fGetPageTimeOff() const;

    double &fPageTimeOn();
    double &fPageTimeOff();

    type_justification_line fGetLastLineJustification();
    type_justification_page fGetLastPageJustification();

    HNS_SignBoard fGetSignBoard(const int64_t &time = 0, const bool &preview_mode = false);
private:
    //This function places all graphical elements on the end of the vector and also verifies that all justified elements are in logical order.
    //That is left,center,right for line and top,center,bottom for page.  Also remember that for each page justification no line justification
    //can coexist with full justification
    bool fSortElements();
    //This function puts the text on to the sign board
    type_hns_signboard_error fUpdateSignBoard(HNS_SignBoard &sign_board, const int64_t &time = 0, const bool &preview_mode = false);
    //Assuming the array is sorted, this returns the last text element
    size_t fLastTextElement() const;
    std::vector<HNS_Message_Justified_Element> f_elements;
    double f_page_time_on, f_page_time_off;

    HNS_SignBoard_Info f_signboard_info;
};

//This is legitimately confusing so bare with me.  The MULTI specification laid out in NTCIP1203 chapter 6 indicates that each line may have text blocks that are justified left, center, right,
//or any combination thereof, so long as they are ordered left to right.  However, within those justified text blocks you may have other tags such as different fonts or flashing text.
//So this class represents one chunk of text that is justified together.  This block of text may consist of multiple elements, but they all share the same justification
//For now, if this is a picture, there can only be one element
class HNS_Message_Justified_Element
{
public:
    HNS_Message_Justified_Element();
    HNS_Message_Justified_Element(const type_justification_line &line_justification, const type_justification_page &page_justification);

    void fAddElement(HNS_Message_Element2 &element, const bool &newline = false, const ssize_t &line_spacing = -1);
    size_t fGetNumElements() const;
    HNS_Message_Element2 fGetElement(const size_t &index) const;
    void fChangeJustification(const type_justification_line &line_justification, const type_justification_page &page_justification);

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const int64_t &time = 0, const bool &preview_mode = false) const;
    //non-const because this will end up modifying the elements for width
    HNS_Bitmap fGetBitmapFullJustified(const std::vector<HNS_Font> *fonts);

    type_justification_line fGetLineJustification() const;
    type_justification_page fGetPageJustification() const;
    bool fIsGraphic() const;

    HNS_Point fGetGraphicPos() const;
    size_t fGetHeight() const;
    size_t fGetNumLines() const;
private:
    std::vector<size_t> fGetLineWidths() const;
    std::vector<size_t> fGetLineHeights() const;
    std::vector<size_t> fGetCharactersPerLine() const;

    size_t fGetYPos(const std::vector<size_t> &line_heights, const size_t &line_index) const;

    type_justification_line f_line_justification;
    type_justification_page f_page_justification;
    std::vector <HNS_Message_Element2> f_elements;
    //remember that this is per page justification.  So if you have [jp2]Hello[nl]World[jp4]Test[nl]Now you would get on a hypothetical 5 line display
    //Hello
    //World
    //
    //Test
    //Now
    size_t f_line_no;

    std::vector<size_t> f_line_spacing;
};

//I just realized that no matter what, all elements are fundamentally pictures.  That is to say, a rectangular block of pixels that are on or off.
//Even text can be represented that way.
//Note this may also represent a graphic.  In which case the rules laid out for overlaying in NTCIP1203 Chapter 6 section 4.18 must be obeyed.
//If this is a graphic, the x and y values are the coordinates of the graphic.  Justification is ignored in this case.
//Likewise if this is text, then the placement info is ignored.
class HNS_Message_Element2
{
public:
    HNS_Message_Element2();
    HNS_Message_Element2(const std::string &input, const HNS_Font &font, const int &font_no, const bool &is_flashing, const HNS_Flashing_Text &flash_info, const size_t &char_spacing, const int &line_no = 0);
    HNS_Message_Element2(const size_t &x, const size_t &y, const size_t &height, const size_t &width, const int &graphic_no, const bool &is_flashing, const HNS_Flashing_Text &flash_info);
    void fSetText(const std::string &input, const HNS_Font &font, const int &font_no, const bool &is_flashing, const HNS_Flashing_Text &flash_info);
    void fSetGraphic(const size_t &x, const size_t &y, const size_t &height, const size_t &width, const int &graphic_no);
    void fSetLineNo(const size_t &line_no);

    bool fIsGraphic() const;
    size_t fGetX() const;
    size_t fGetY() const;
    size_t fGetWidth() const;
    size_t fGetHeight() const;
    size_t fGetLineNo() const;
    size_t fGetCharacterCount() const;
    size_t fGetCharacterWidth(const std::vector<HNS_Font> *fonts) const;
    size_t fGetCharSpacing() const;
    std::string fGetText() const;
    int fGetFontNo() const;

    HNS_Flashing_Text fGetFlashInfo() const {return f_flash_info;}
    bool fGetIsFlashing() const {return f_is_flashing;}

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics) const;
    //this is non const because it resizes the text block to a new width
    HNS_Bitmap fGetBitmapFixedWidth(const std::vector<HNS_Font> *fonts, const size_t &fixed_width);
private:
    bool f_is_graphic;
    std::string f_text;
    int f_graphic_no, f_font_no;
    HNS_Flashing_Text f_flash_info;
    bool f_is_flashing;

    size_t f_char_spacing;

    //graphic placement;
    size_t f_x,f_y;

    //bounding box
    size_t f_width,f_height;
    //remember that this is per page justification.  So if you have [jp2]Hello[nl]World[jp4]Test[nl]Now you would get on a hypothetical 5 line display
    //Hello
    //World
    //
    //Test
    //Now
    size_t f_line_no;
};

//a handy container for field data transmitted from the NTCIP machine (not defined here) to the Message
class HNS_Field_Data
{
public:
    HNS_Field_Data();
    HNS_Field_Data(const time_t &input_time, const int &speed, const int &temperature, const bool &kph = false, const bool &celsius = false);

    void fSetSpeed(const int &speed, const bool &kph = false);
    void fSetTemperature(const int &temperature, const bool &celsius = false);
    void fSetTime(const time_t &input_time);

    int fGetSpeed(const bool &kph = false) const;
    int fGetTemperature(const bool &celsius = false) const;
    tm *fGetTime() const;

private:
    time_t f_current_time;
    //current speed in kph
    double f_speed;
    //current exterior temperature in degrees C
    double f_temperature;
};


#endif /* SRC_MESSAGE_H_ */
