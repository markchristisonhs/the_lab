#ifndef MULTI_H
#define MULTI_H

#include <stdint.h>
#include "hns_multi_info_interface.h"

const uint8_t MAX_PAGE_ITEMS = 48; /* this should be enough */
const uint8_t MAX_PAGES = 6;

const uint8_t MAX_TOKENS = 60; /* should be enough */

const uint8_t MAX_FIELD_DESC_LENGTH = 10;

// define Token types
const uint8_t TOK_NOTUSED = 0;
const uint8_t TOK_TEXT = 1;
const uint8_t TOK_MULTI_TAG = 2;

// define MULTI tags
const uint8_t MULTAG_NONE = 0;
const uint8_t MULTAG_COLOR_BACKGROUND = 1;
const uint8_t MULTAG_COLOR_PAGE_BACKGROUND = 2;
const uint8_t MULTAG_COLOR_FOREGROUND = 3;
const uint8_t MULTAG_COLOR_RECTANGLE = 4;
const uint8_t MULTAG_FIELD = 5;
const uint8_t MULTAG_FLASH = 6;
const uint8_t MULTAG_CLOSE_FLASH = 7;
const uint8_t MULTAG_FONT = 8;
const uint8_t MULTAG_GRAPHIC = 9;
const uint8_t MULTAG_HEXCHAR = 10;
const uint8_t MULTAG_LINE_JUSTIFICATION = 11;
const uint8_t MULTAG_PAGE_JUSTIFICATION = 12;
const uint8_t MULTAG_MANUFACTURE = 13;
const uint8_t MULTAG_CLOSE_MANUFACTURE = 14;
const uint8_t MULTAG_MOVING_TEXT = 15;
const uint8_t MULTAG_NEW_LINE = 16;
const uint8_t MULTAG_NEW_PAGE = 17;
const uint8_t MULTAG_PAGE_TIME = 18;
const uint8_t MULTAG_SPACING_CHAR = 19;
const uint8_t MULTAG_CLOSE_SPACE_CHAR = 20;
const uint8_t MULTAG_TEXT_RECTANGLE = 21;

const uint8_t LINE_JUSTIFICATION_OTHER = 1;
const uint8_t LINE_JUSTIFICATION_LEFT = 2;
const uint8_t LINE_JUSTIFICATION_CENTER = 3;
const uint8_t LINE_JUSTIFICATION_RIGHT = 4;
const uint8_t LINE_JUSTIFICATION_FULL = 5;

const uint8_t PAGE_JUSTIFICATION_OTHER = 1;
const uint8_t PAGE_JUSTIFICATION_TOP = 2;
const uint8_t PAGE_JUSTIFICATION_MIDDLE = 3;
const uint8_t PAGE_JUSTIFICATION_BOTTOM = 4;

const uint8_t FLASH_ON_THEN_OFF = 0;
const uint8_t FLASH_OFF_THEN_ON = 1;

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
        uint8_t tokenType;
        uint8_t tagValue; // if token is a Multitag, this is tag
        uint8_t tokenLength;
        uint16_t multiStringIndex; // start of token
        MULTI_TAG multiTag;
} TOKEN;

typedef TOKEN TOKENS[MAX_TOKENS];

typedef struct {
        uint8_t lineJustification;
        uint8_t pageJustification;
        uint8_t currentFont;
        uint8_t defaultFlashOnTime;
        uint8_t defaultFlashOffTime;
        uint8_t currentPageOnTime;
        uint8_t currentPageOffTime;
} MESSAGE_CONTEXT;

void SetMULTIInterface(HNS_Multi_Info_Interface *interface);

int16_t BuildTokens(TOKEN tokens[],
                  uint16_t maxNumberOfTokens,
                  uint8_t *pMultiString,
                  int16_t *pError,
                  int16_t *pSyntaxError,
                  uint16_t *pMultiIndexOfError);

#endif // MULTI_H
