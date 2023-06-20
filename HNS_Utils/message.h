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

typedef enum
{
    HNS_ACTIVATION_ERROR_OTHER = 1,
    HNS_ACTIVATION_ERROR_NONE = 2,
    HNS_ACTIVATION_ERROR_PRIORITY = 3,
    HNS_ACTIVATION_ERROR_MESSAGE_STATUS = 4,
    HNS_ACTIVATION_ERROR_MESSAGE_TYPE = 5,
    HNS_ACTIVATION_ERROR_MESSAGE_NUMBER = 6,
    HNS_ACTIVATION_ERROR_MESSAGE_CRC = 7,
    HNS_ACTIVATION_ERROR_SYNTAXMULTI = 8,
    HNS_ACTIVATION_ERROR_LOCAL_MODE = 9,
    HNS_ACTIVATION_ERROR_CENTRAL_MODE = 10,
    HNS_ACTIVATION_ERROR_CENTRAL_OVERRIDE_MODE = 11
} type_activation_error;

class HNS_Message_Element2;
class HNS_Message;
class HNS_Message2;
class HNS_Message_Page;
class HNS_Message_Element;
class HNS_Message_Justified_Element;
class HNS_Message_Page_Justified_Element;
class HNS_Graphical_Element;
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

bool IsFlashOn(const int64_t &time, const HNS_Flashing_Text &flash_info);

class HNS_Message2
{
public:
    HNS_Message2();
    HNS_Message2(const HNS_SignBoard_Info &signboard_info);

    int fSetMULTI(std::string &multi_string, const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const HNS_Field_Data *field_data, type_multi_syntax_error *multi_error = nullptr, bool *too_tall = nullptr, size_t max_num_pages = 3, int *error_position = nullptr);
    std::string fGetMULTI() const;
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

    //returns which fonts are used by this message.
    std::vector<int> fGetFontsUsed();

    double fGetCurrentPageTimeOn() const {return f_current_page_time_on;}
    double fGetCurrentPageTimeOff() const {return f_current_page_time_off;}
    type_justification_line fGetCurrentLineJustification() const {return f_current_line_justification;}
    type_justification_page fGetCurrentPageJustification() const {return f_current_page_justification;}
    int fGetCurrentFont() const {return f_current_font;}
    HNS_Flashing_Text fGetCurrentFlashingInfo() const {return f_current_flash_info;}
    bool fGetCurrentFlashState() const {return f_current_flashing_state;}
    int fGetCurrentLineSpacing() const {return f_current_line_spacing;}
    int fGetCurrentCharSpacing() const {return f_current_char_spacing;}
private:
    void fAddPage();
    type_hns_signboard_error fAddElementToPage(HNS_Message_Element2 &element, const bool &newline = false);
    type_hns_signboard_error fAddGraphicToPage(const HNS_Graphical_Element &graphic);
    void fAddNewLineToPage();
    void fResetMulti();
    size_t fGetPageIndexFromTime(const int64_t &time, unsigned int &time_in_page);
    unsigned int fGetMessagePeriod() const;
    void fResetDefaults();
    std::vector<HNS_Message_Page2> f_pages;

    static type_justification_line f_default_line_justification;
    static type_justification_page f_default_page_justification;
    static double f_default_page_time_on, f_default_page_time_off;
    static unsigned int f_default_flash_time_on, f_default_flash_time_off;
    static int f_default_font;
    static HNS_SignBoard_Info f_default_signboard_info;

    //Stuff that is settable across pages, such as page times.
    //for example, [pt40o5]TEST[np]HELLO[np]WORLD the page times of 4/.5 would apply to all pages but
    //[pt40o5]TEST[np]HELLO[pt50o0][np]WORLD would have 4/.5 apply to the first two pages and 5/0 apply to the last page.
    //So the page times and justifications and fonts need to be stored at the message level.
    double f_current_page_time_on, f_current_page_time_off;
    type_justification_line f_current_line_justification;
    type_justification_page f_current_page_justification;
    int f_current_font;
    //Also line and character spacing.
    int f_current_line_spacing, f_current_char_spacing;
    //I can't find anything in NTCIP 1203 Chapter 6 that says whether flashing tags are shared across pages.
    //For now I am guessing that they can be
    HNS_Flashing_Text f_current_flash_info;
    bool f_current_flashing_state;


    //the currently set multistring
    std::string f_multi;
};

class HNS_Message_Page2
{
public:
    HNS_Message_Page2(const HNS_Message2 *parent);
    HNS_Message_Page2(const HNS_Message2 *parent, const double &page_time_on, const double &page_time_off,const HNS_SignBoard_Info &sign_board_info);

//    void fNewJustification(const type_justification_line &line_justification, const type_justification_page &page_justification);
    type_hns_signboard_error fAddElement(HNS_Message_Element2 &element, const bool &newline = false);
    type_hns_signboard_error fAddGraphic(const HNS_Graphical_Element &graphic);
    size_t fGetNumElements() const;
    void fAddNewline();

    double fGetPageTimeOn() const;
    double fGetPageTimeOff() const;

    double &fPageTimeOn();
    double &fPageTimeOff();

    type_justification_line fGetLastLineJustification();
    type_justification_page fGetLastPageJustification();

    HNS_SignBoard fGetSignBoard(const int64_t &time = 0, const bool &preview_mode = false, const bool &single_page = false);

    //Gets a list of all fonts used on this page
    std::vector<int> fGetFontsUsed();
private:
    //This function places all graphical elements on the end of the vector and also verifies that all justified elements are in logical order.
    //That is left,center,right for line and top,center,bottom for page.  Also remember that for each page justification no line justification
    //can coexist with full justification
    bool fSortElements();
    //This function puts the text on to the sign board
    type_hns_signboard_error fUpdateSignBoard(HNS_SignBoard &sign_board, const int64_t &time = 0, const bool &preview_mode = false, const bool &single_page = false);
    std::vector<HNS_Message_Justified_Element> f_elements;
    double f_page_time_on, f_page_time_off;

    HNS_SignBoard_Info f_signboard_info;

    //New stuff to fix problems with justification
    const HNS_Message2 *f_parent_message;
    std::vector<HNS_Message_Page_Justified_Element> f_page_elements;
    std::vector<HNS_Graphical_Element> f_graphics;
};

class HNS_Graphical_Element
{
public:
    HNS_Graphical_Element(const HNS_Message2 *parent);
    HNS_Graphical_Element(const HNS_Message2 *parent, const int &graphic_no, const int &x, const int &y, const int &width, const int &height);

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Graphic> *graphics, const int64_t &time, const bool &preview_mode = false) const;
    HNS_Point fGetGraphicPos() const {return HNS_Point(f_x,f_y);}
    int fGetX() const {return f_x;}
    int fGetY() const {return f_y;}
private:
    int f_graphic_no;
    size_t f_x,f_y;
    size_t f_width,f_height;

    HNS_Flashing_Text f_flash_info;
    bool f_is_flashing;

    const HNS_Message2 *f_parent_message;
};

//This class groups line justified elements that are page-justified together.  This is for text only.  Remember they must be in logical order.
class HNS_Message_Page_Justified_Element
{
public:
    HNS_Message_Page_Justified_Element(const HNS_Message2 *parent);

    void fAddElement(const HNS_Message_Element2 &element, const bool &newline = false);
    void fAddNewLine();
    size_t fGetNumElements() const {return f_elements.size();}
    HNS_Message_Justified_Element fGetElement(const size_t &index) const;

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Font> *fonts, const int64_t &time = 0, const HNS_SignBoard *signboard = nullptr, type_hns_signboard_error *error = nullptr, const bool &preview_mode = false) const;

    type_justification_page fGetPageJustification() const;
    size_t fGetNumLines() const;

    const HNS_Message2 *fGetParentMessage() const {return f_parent_message;}
private:
    std::vector<HNS_Message_Justified_Element> f_elements;

    type_justification_page f_page_justification;

    const HNS_Message2 *f_parent_message;
};

//This represents a text element on a single line with a single line justification
class HNS_Message_Justified_Element
{
public:
    HNS_Message_Justified_Element(const HNS_Message2 *parent, const int &starting_line);

    void fAddElement(HNS_Message_Element2 element, const bool &newline = false);
    void fAddNewLine();
    size_t fGetNumElements() const;
    HNS_Message_Element2 fGetElement(const size_t &index) const;
    void fChangeJustification(const type_justification_line &line_justification, const type_justification_page &page_justification);

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Font> *fonts, const int64_t &time = 0, const bool &preview_mode = false) const;
    //non-const because this will end up modifying the elements for width
    HNS_Bitmap fGetBitmapFullJustified(const std::vector<HNS_Font> *fonts);

    type_justification_line fGetLineJustification() const;
    type_justification_page fGetPageJustification() const;

    size_t fGetHeight() const;
    size_t fGetNumLines() const;
    size_t fGetLastLineSpacing() const;
    size_t fGetFirstLineNo() const;
    size_t fGetLastLineNo() const;
    size_t fGetFirstLineHeight() const;

    //Returns the fonts used in this element
    std::vector<int> fGetFontsUsed();

    const HNS_Message2 *fGetParentMessage() const {return f_parent_message;}
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
    size_t f_temp_line_no;

    std::vector<size_t> f_line_spacing;

    const HNS_Message2 *f_parent_message;
};

//I just realized that no matter what, all elements are fundamentally pictures.  That is to say, a rectangular block of pixels that are on or off.
//Even text can be represented that way.
//Note this may also represent a graphic.  In which case the rules laid out for overlaying in NTCIP1203 Chapter 6 section 4.18 must be obeyed.
//If this is a graphic, the x and y values are the coordinates of the graphic.  Justification is ignored in this case.
//Likewise if this is text, then the placement info is ignored.
class HNS_Message_Element2
{
public:
    HNS_Message_Element2(const HNS_Message2 *parent);
    HNS_Message_Element2(const HNS_Message2 *parent,const std::string &input, const HNS_Font &font, const int &font_no = -1, const int &line_no = 0);
    void fSetText(const std::string &input, const HNS_Font &font, const int &font_no, const bool &is_flashing, const HNS_Flashing_Text &flash_info);
    void fSetLineNo(const size_t &line_no);

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

    HNS_Bitmap fGetBitmap(const std::vector<HNS_Font> *fonts) const;
    //this is non const because it resizes the text block to a new width
    HNS_Bitmap fGetBitmapFixedWidth(const std::vector<HNS_Font> *fonts, const size_t &fixed_width);
private:
    std::string f_text;
    int f_font_no;
    HNS_Flashing_Text f_flash_info;
    bool f_is_flashing;

    size_t f_char_spacing;

    //bounding box
    size_t f_width,f_height;
    //remember that this is per page justification.  So if you have [jp2]Hello[nl]World[jp4]Test[nl]Now you would get on a hypothetical 5 line display
    //Hello
    //World
    //
    //Test
    //Now
    size_t f_line_no;

    const HNS_Message2 *f_parent_message;
};

//a handy container for field data transmitted from the NTCIP machine (not defined here) to the Message
class HNS_Field_Data
{
public:
    HNS_Field_Data();
    HNS_Field_Data(const time_t &input_time, const int &speed, const int &temperature, const bool &kph = false, const bool &celsius = false);

    void fSetSpeed(const int &speed, const bool &kph = false);
    void fSetRadarConnected(const bool &radar_connected);
    void fSetTemperature(const int &temperature, const bool &celsius = false);
    void fSetTime(const time_t &input_time);

    int fGetSpeed(const bool &kph = false) const;
    bool fGetRadarConnected() const;
    int fGetTemperature(const bool &celsius = false) const;
    tm *fGetTime() const;

private:
    time_t f_current_time;
    //current speed in kph
    double f_speed;
    bool f_radar_connected;
    //current exterior temperature in degrees C
    double f_temperature;
};


#endif /* SRC_MESSAGE_H_ */
