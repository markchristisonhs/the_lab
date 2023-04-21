#ifndef MESSAGE2_H
#define MESSAGE2_H

#include "hns_multi_info_interface.h"
#include "hns_sign_info_interface.h"

//eventually this should probably go away
#include "message.h"

#include <string>
#include <fonts.h>
#include <graphics.h>

namespace HNS_MESSAGE_V2
{
    // define Token types
    typedef enum
    {
        TOK_NOTUSED = 0,
        TOK_TEXT = 1,
        TOK_MULTI_TAG = 2
    } type_token;

    // define MULTI tags
    typedef enum
    {
        MULTAG_NONE = 0,
        MULTAG_COLOR_BACKGROUND = 1,
        MULTAG_COLOR_PAGE_BACKGROUND = 2,
        MULTAG_COLOR_FOREGROUND = 3,
        MULTAG_COLOR_RECTANGLE = 4,
        MULTAG_FIELD = 5,
        MULTAG_FLASH = 6,
        MULTAG_CLOSE_FLASH = 7,
        MULTAG_FONT = 8,
        MULTAG_GRAPHIC = 9,
        MULTAG_HEXCHAR = 10,
        MULTAG_LINE_JUSTIFICATION = 11,
        MULTAG_PAGE_JUSTIFICATION = 12,
        MULTAG_MANUFACTURE = 13,
        MULTAG_CLOSE_MANUFACTURE = 14,
        MULTAG_MOVING_TEXT = 15,
        MULTAG_NEW_LINE = 16,
        MULTAG_NEW_PAGE = 17,
        MULTAG_PAGE_TIME = 18,
        MULTAG_SPACING_CHAR = 19,
        MULTAG_CLOSE_SPACE_CHAR = 20,
        MULTAG_TEXT_RECTANGLE = 21
    } type_multi_tag;

    typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } RGB;

    typedef struct {
        uint8_t isRGB;
        union {
            uint8_t z;
            RGB rgb;
        }color;
    } COLOR;

    typedef struct {
        uint16_t x;
    } COLOR_BACKGROUND_M;

    typedef struct {
        COLOR color;
    } COLOR_PAGE_BACKGROUND;

    typedef struct {
        COLOR color;
    } COLOR_FOREGROUND;

    typedef struct {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        COLOR color;
    } COLOR_RECTANGLE;

    typedef struct {
        uint8_t x;
        uint8_t y;
        uint8_t orderInMultiString;
    } FIELD;

    typedef struct {
        uint8_t order; // 0 = on/off, 1 = off/on
        int16_t on; // -1 for default
        int16_t off; // -1 for default
    } FLASH_TAG;

    class HNS_MULTI_Tag
    {
    public:
        HNS_MULTI_Tag() : f_tag_type(MULTAG_NONE){}
        virtual type_multi_tag fGetTagType() {return f_tag_type;}

        type_multi_tag &fTagType() {return f_tag_type;}
    private:
        type_multi_tag f_tag_type;
    };

    class HNS_TAG_Field : public HNS_MULTI_Tag
    {
    public:
        HNS_TAG_Field() : x(0),y(0),orderInMultiString(0) {}
        type_multi_tag fGetTagType() {return MULTAG_FIELD;}

        uint8_t &fX() {return x;}
        uint8_t fGetX() const {return x;}

        uint8_t &fY() {return y;}
        uint8_t fGetY() const {return y;}

        uint8_t &fOrderInMultiString() {return orderInMultiString;}
        uint8_t fGetOrderInMultiString() const {return orderInMultiString;}
    private:
        uint8_t x;
        uint8_t y;
        uint8_t orderInMultiString;
    };

    class HNS_TAG_Flash : public HNS_MULTI_Tag
    {
    public:
        HNS_TAG_Flash():order(0),on(0),off(0){}

        uint8_t &fOrder() {return order;}
        uint8_t fGetOrder() const {return order;}

        int16_t &fOn() {return on;}
        int16_t fGetOn() const {return on;}

        int16_t &fOff() {return off;}
        int16_t fGetOff() const {return off;}

        type_multi_tag fGetTagType() {return MULTAG_FLASH;}
    private:
        uint8_t order; // 0 = on/off, 1 = off/on
        int16_t on; // -1 for default
        int16_t off; // -1 for default
    };

    class HNS_TAG_Font : public HNS_MULTI_Tag
    {
    public:
        HNS_TAG_Font():number(0),versionID(0){}

        uint8_t &fNumber() {return number;}
        uint8_t fGetNumber() const {return number;}

        uint16_t &fVersionID() {return versionID;}
        uint16_t fGetVersionID() const {return versionID;}

        type_multi_tag fGetTagType() {return MULTAG_FONT;}
    private:
        uint8_t number;
        uint16_t versionID;
    };

    class HNS_TAG_Graphic : public HNS_MULTI_Tag
    {
    public:
        HNS_TAG_Graphic():
            number(0),
            x(0),
            y(0),
            versionID(0) {}

        uint8_t &fNumber() {return number;}
        uint8_t fGetNumber() const {return number;}

        uint8_t &fX() {return x;}
        uint8_t fGetX() const {return x;}

        uint8_t &fY() {return y;}
        uint8_t fGetY() const {return y;}

        uint16_t &fVersionID() {return versionID;}
        uint16_t fGetVersionID() const {return versionID;}

        type_multi_tag fGetTagType() {return MULTAG_GRAPHIC;}
    private:
        uint8_t number;
        uint8_t x;
        uint8_t y;
        uint16_t versionID;
    };

    typedef struct {
        uint8_t number;
        uint16_t versionID;
    } FONT_TAG;

    typedef struct {
        uint8_t number;
        uint8_t x;
        uint8_t y;
        uint16_t versionID;
    } GRAPHIC_TAG;

    typedef struct {
        uint16_t x;
        uint8_t  *pStartOfText;
        uint8_t  textLength;
    } MANUFACTURER_TAG;

    typedef struct {
        uint8_t c; // if c ==1 then circular, otherwise linear
        uint8_t x; //delay
        uint8_t isLeftMoving; // 1 == left, 0 = right moving
        uint8_t w; // width of window
        uint8_t s; // number of pixels shall move at defined rate r
        uint8_t r; // time, in 0.1 sec between two steps 's'
    } MOVING_TEXT_INFO;

    typedef struct {
        MOVING_TEXT_INFO movingInfo;
        uint8_t *pStartOfText;
        uint8_t textLength;
    } MOVING_TEXT_TAG;

    typedef struct {
        uint8_t x;
    } NEW_LINE_TAG;

    typedef struct {
        int16_t on; // -1 means default
        int16_t off; // -1 means default
    } PAGE_TIME_TAG;

    typedef struct {
        uint8_t x;
        uint8_t y;
        uint8_t w;
        uint8_t h;
    } TEXT_RECTANGLE_TAG;

    typedef struct {
        uint8_t tagId;
        union {
            COLOR_BACKGROUND_M colorBackground;
            COLOR_FOREGROUND colorForeground;
            COLOR_PAGE_BACKGROUND colorPageBackground;
            FIELD field;
            FLASH_TAG flash;
            FONT_TAG font;
            GRAPHIC_TAG graphic;
            uint16_t hexCharacter;
            uint8_t lineJustification;
            uint8_t pageJustification;
            MANUFACTURER_TAG manufacturerTag;
            NEW_LINE_TAG newLine;
            PAGE_TIME_TAG pageTime;
            uint8_t spacingChar;
            TEXT_RECTANGLE_TAG textRectangle;
            MOVING_TEXT_TAG movingText;
        } tag;

    } MULTI_TAG;

    typedef struct {
            uint8_t lineJustification;
            uint8_t pageJustification;
            uint8_t currentFont;
            uint8_t defaultFlashOnTime;
            uint8_t defaultFlashOffTime;
            uint8_t currentPageOnTime;
            uint8_t currentPageOffTime;
    } MESSAGE_CONTEXT;

    class HNS_Token
    {
    public:
        HNS_Token():
            tokenType(TOK_NOTUSED)
          , tokenLength(0)
          , multiStringIndex(0) {}

        type_token &fTokenType() {return tokenType;}
        type_token fGetTokenType() const {return tokenType;}

        uint8_t &fTokenLength() {return tokenLength;}
        uint8_t fGetTokenLength() const {return tokenLength;}

        uint16_t &fMultiStringIndex() {return multiStringIndex;}
        uint16_t fGetMultiStringIndex() const {return multiStringIndex;}

        HNS_MULTI_Tag &fMultiTag() {return multiTag;}
        HNS_MULTI_Tag fGetMultiTag() const {return multiTag;}
    private:
        type_token tokenType;
        uint8_t tokenLength;
        uint16_t multiStringIndex; // start of token
        HNS_MULTI_Tag multiTag;
    };

    typedef struct {
            uint8_t tokenType;
            uint8_t tagValue; // if token is a Multitag, this is tag
            uint8_t tokenLength;
            uint16_t multiStringIndex; // start of token
            MULTI_TAG multiTag;
    } TOKEN;

    typedef struct movingTextWindowTag {
        uint16_t charRow;  // character row of moving text window
        uint16_t TopLeftY;  // relative to sign face
        uint16_t TopLeftX;  // relative to sign face
        uint16_t currentFont;
        MOVING_TEXT_TAG movingTextTag;
        uint8_t  linearTimer;
        uint8_t  movingTimer;
        uint16_t pixelLengthOfText;
        uint8_t  innerCharSpacing;
        uint16_t  pixelsToShift;
        uint16_t startIndexInMessage; // index of first pixel in the message
        uint16_t startColInWindow; // starting col in Text Window
        uint8_t  notFirstTime; // true if not the first time displaying moving text
        uint8_t  isActive; // true if active
    } MOVING_TEXT_WINDOW;

    typedef struct graphic_bitmap
    {
            uint16_t length;
            uint8_t bitmap[180];
    } GRAPHIC_BITMAP;

    typedef struct  graphicDescriptorTag{
        uint16_t graphicNumber;
        GRAPHIC_BITMAP *pgraphicBitmap;
        uint16_t  height;
        uint16_t width;
        uint16_t topLeftX; // relative to sign face
        uint16_t topLeftY; // relative to sign face
        uint16_t visible;
    } GRAPHIC_DESCRIPTOR;

    typedef struct fcharEntry {
            uint16_t characterNumber;
            uint8_t characterWidth;
            uint8_t  characterBitMap[25];
    } CHARACTER_ENTRY;

    typedef struct charDescriptorTag {
        uint16_t charNum;
        uint8_t charRow; // 1 based
        uint8_t charCol; // 1 based
        uint8_t fontNumber;
        //UINT8 fontWidth;
        CHARACTER_ENTRY *pCharEntry;
        uint8_t fontHeight;
        uint8_t fontCharSpacing;
        uint8_t fontLineSpacing;
        uint8_t lineJustification;


        uint16_t topLeftX;
        uint16_t topLeftY;
        uint8_t topLeftXpositionLocked; // true if the x position of the character
                                      // has been fixed and is not allowed to change
                                      // by further line justification tags
        uint8_t windowNumber;
        uint8_t attribute;
        uint8_t visible;
        uint8_t spaceToRtNeighbor;
        uint8_t textType;
        uint8_t flashTagIndex;
        uint16_t movingWindowPixelWidth;
    } CHAR_DESCRIPTOR;

    typedef struct textWindowTag {
        uint16_t TopLeftY;  // relative to sign face
        uint16_t TopLeftX;  // relative to sign face
        uint16_t width; // in pixels
        uint16_t height; // in pixels
        uint8_t  lineJustification[5];
        uint8_t  pageJustification[5];
    } TEXT_WINDOW;

    typedef struct  pageTag {
        uint16_t numberTextWindows;
        TEXT_WINDOW textWindow[4];

        uint16_t numberGraphics;
        GRAPHIC_DESCRIPTOR graphics[10];

        uint16_t numberChars;
        CHAR_DESCRIPTOR chars[50];

        //UINT16 numberMovingTextWindows;
        MOVING_TEXT_WINDOW	movingTextWindows[5];


    } PAGE;

    class HNS_Message
    {
    public:
        HNS_Message();
        HNS_Message(const std::string &multi);

        void fSetMulti(const std::string &multi);

        static void fSetMultiInfoInterface(HNS_Multi_Info_Interface *interface);
        static void fSetSignInfoInterface(HNS_Sign_Info_Interface *interface);
    private:
        type_multi_tag GetMultiTag(uint8_t *pMulti,uint8_t *pTokenLength);

        int8_t BuildMultiTag(type_multi_tag tag, uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);

        int8_t BuildColorBackgroundTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildColorPageBackgroundTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildColorForegroundTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildColorRectangleTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildFieldTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildFlashTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildCloseFlashTag(uint8_t *pMulti, uint8_t TokenLength,
                HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildFontTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildGraphicTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildHexCharTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildLineJustificationTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildPageJustificationTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildManufacturerTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildCloseManufacturerTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildMovingTextTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildNewLineTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildNewPageTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildPageTimeTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildSpacingCharTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildCloseSpacingCharTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);
        int8_t BuildTextRectangleTag(uint8_t *pMulti, uint8_t TokenLength,
                MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex);

        uint8_t GetDecimalInteger(uint8_t *p, int16_t *pResult);
        uint8_t GetHexInteger(uint8_t *p, int16_t *pResult);

        void InitMessageContext(MESSAGE_CONTEXT* pContext);
        void InitializePage(uint8_t isNewPage, PAGE* pPage, MESSAGE_CONTEXT* pContext);

        int16_t BuildTokens(TOKEN tokens[],
                          uint16_t maxNumberOfTokens,
                          uint8_t *pMultiString,
                          int16_t *pError,
                          int16_t *pSyntaxError,
                          uint16_t *pMultiIndexOfError);
        int16_t BuildTokens(std::vector<HNS_Token> &tokens,
                          uint16_t maxNumberOfTokens,
                          uint8_t *pMultiString,
                          int16_t *pError,
                          int16_t *pSyntaxError,
                          uint16_t *pMultiIndexOfError);
        int16_t ValidateMultiString(uint8_t *pMultiString,
                                  int16_t *pError,
                                  int16_t *pSyntaxError,
                                  int16_t *pMultiIndexOfError);

        static HNS_Multi_Info_Interface *fs_multi_interface;
        static HNS_Sign_Info_Interface *fs_sign_info_interface;

        type_multi_syntax_error f_syntax_error;
    };

    class HNS_Message_Char_Descriptor
    {
    public:
    private:
        uint16_t charNum;
        uint8_t charRow; // 1 based
        uint8_t charCol; // 1 based
        uint8_t fontNumber;
        //UINT8 fontWidth;
        HNS_Character pCharEntry;
        uint8_t fontHeight;
        uint8_t fontCharSpacing;
        uint8_t fontLineSpacing;
        uint8_t lineJustification;


        uint16_t topLeftX;
        uint16_t topLeftY;
        uint8_t topLeftXpositionLocked; // true if the x position of the character
                                      // has been fixed and is not allowed to change
                                      // by further line justification tags
        uint8_t windowNumber;
        uint8_t attribute;
        uint8_t visible;
        uint8_t spaceToRtNeighbor;
        uint8_t textType;
        uint8_t flashTagIndex;
        uint16_t movingWindowPixelWidth;
    };

    class HNS_Message_Graphic_Descriptor
    {
    public:
    private:
        HNS_Graphic pgraphicBitmap;
        uint16_t topLeftX; // relative to sign face
        uint16_t topLeftY; // relative to sign face
        uint16_t visible;
    };

    class HNS_Message_Text_Window
    {
    public:
    private:
        uint16_t TopLeftY;  // relative to sign face
        uint16_t TopLeftX;  // relative to sign face
        uint16_t width; // in pixels
        uint16_t height; // in pixels
        uint8_t  lineJustification[5];
        uint8_t  pageJustification[5];
    };

    class HNS_Message_Moving_Text_Window
    {
    public:
        HNS_Message_Moving_Text_Window();
    private:
        uint16_t charRow;  // character row of moving text window
        uint16_t TopLeftY;  // relative to sign face
        uint16_t TopLeftX;  // relative to sign face
        uint16_t currentFont;
        MOVING_TEXT_TAG movingTextTag;
        uint8_t  linearTimer;
        uint8_t  movingTimer;
        uint16_t pixelLengthOfText;
        uint8_t  innerCharSpacing;
        uint16_t  pixelsToShift;
        uint16_t startIndexInMessage; // index of first pixel in the message
        uint16_t startColInWindow; // starting col in Text Window
        uint8_t  notFirstTime; // true if not the first time displaying moving text
        uint8_t  isActive; // true if active
    };

    class HNS_Message_Page
    {
    public:
        std::vector<HNS_Message_Text_Window> textWindow;
        std::vector<HNS_Message_Graphic_Descriptor> graphics;
        std::vector<HNS_Message_Char_Descriptor> chars;
        std::vector<HNS_Message_Moving_Text_Window> movingTextWindows;
    };
}

#endif // MESSAGE2_H
