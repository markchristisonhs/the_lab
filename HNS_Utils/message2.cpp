#include "message2.h"

#include <ctype.h>
#include <stdio.h>
#include <cstring>

using namespace HNS_MESSAGE_V2;
using namespace std;

const uint8_t MAX_PAGE_ITEMS = 48; /* this should be enough */
const uint8_t MAX_PAGES = 6;

const uint8_t MAX_TOKENS = 60; /* should be enough */

const uint8_t MAX_FIELD_DESC_LENGTH = 10;

// define Token types
//const uint8_t TOK_NOTUSED = 0;
//const uint8_t TOK_TEXT = 1;
//const uint8_t TOK_MULTI_TAG = 2;

//// define MULTI tags
//const uint8_t MULTAG_NONE = 0;
//const uint8_t MULTAG_COLOR_BACKGROUND = 1;
//const uint8_t MULTAG_COLOR_PAGE_BACKGROUND = 2;
//const uint8_t MULTAG_COLOR_FOREGROUND = 3;
//const uint8_t MULTAG_COLOR_RECTANGLE = 4;
//const uint8_t MULTAG_FIELD = 5;
//const uint8_t MULTAG_FLASH = 6;
//const uint8_t MULTAG_CLOSE_FLASH = 7;
//const uint8_t MULTAG_FONT = 8;
//const uint8_t MULTAG_GRAPHIC = 9;
//const uint8_t MULTAG_HEXCHAR = 10;
//const uint8_t MULTAG_LINE_JUSTIFICATION = 11;
//const uint8_t MULTAG_PAGE_JUSTIFICATION = 12;
//const uint8_t MULTAG_MANUFACTURE = 13;
//const uint8_t MULTAG_CLOSE_MANUFACTURE = 14;
//const uint8_t MULTAG_MOVING_TEXT = 15;
//const uint8_t MULTAG_NEW_LINE = 16;
//const uint8_t MULTAG_NEW_PAGE = 17;
//const uint8_t MULTAG_PAGE_TIME = 18;
//const uint8_t MULTAG_SPACING_CHAR = 19;
//const uint8_t MULTAG_CLOSE_SPACE_CHAR = 20;
//const uint8_t MULTAG_TEXT_RECTANGLE = 21;

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

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;

HNS_Multi_Info_Interface *HNS_MESSAGE_V2::HNS_Message::fs_multi_interface = nullptr;
HNS_Sign_Info_Interface *HNS_MESSAGE_V2::HNS_Message::fs_sign_info_interface = nullptr;

HNS_MESSAGE_V2::HNS_Message::HNS_Message():
    f_syntax_error(HNS_MULTI_SYNTAX_ERROR_NONE)
{

}

HNS_MESSAGE_V2::HNS_Message::HNS_Message(const std::string &multi)
{
    fSetMulti(multi);
}

void HNS_MESSAGE_V2::HNS_Message::fSetMulti(const std::string &multi)
{
    vector<HNS_Token> tokens;
    string tempstring = multi;
    int16_t error;
    int16_t syntax_error;
    uint16_t error_pos;
    BuildTokens(tokens,MAX_TOKENS,(uint8_t*)tempstring.c_str(),&error,&syntax_error,&error_pos);
}

type_multi_tag HNS_MESSAGE_V2::HNS_Message::GetMultiTag(uint8_t *pMulti,uint8_t *pTokenLength)
{
    // pMulti - points to start of multi string
    // pTokenLength - points to location to receive length of
    //				  the multitag
    // returns the MULTITAG
    type_multi_tag result = MULTAG_NONE;

    UINT8 *pEnd = (UINT8 *)strchr((char *)pMulti,']');

    INT8 ch1,ch2,ch3;
    UINT8 tokenLength;

    *pTokenLength = 0;
    if (pEnd == NULL) return result; // no tag
    tokenLength = pEnd-pMulti+1;

    pMulti++;
    ch1 = toupper((char)*pMulti++); // first char of tag
    ch2 = toupper((char)*pMulti++); // second char of tag


    switch (ch1) {
        case 'C':
            switch (ch2){
                case 'B':
                    result = MULTAG_COLOR_BACKGROUND;
                    break;
                case 'F':
                    result = MULTAG_COLOR_FOREGROUND;
                    break;
                case 'R':
                    result = MULTAG_COLOR_RECTANGLE;

                    break;
            }
            break;
        case 'F':
            if (ch2 == 'L')
                result = MULTAG_FLASH;
            else if (ch2 == 'O')
                result = MULTAG_FONT;
            else
                result = MULTAG_FIELD;
            break;
        case 'G':
            result = MULTAG_GRAPHIC;
            break;
        case 'H':
            if (ch2 == 'C')
                result = MULTAG_HEXCHAR;
            break;
        case 'J':
            if (ch2 == 'L')
                result = MULTAG_LINE_JUSTIFICATION;
            else if (ch2 == 'P')
                result = MULTAG_PAGE_JUSTIFICATION;
            break;
        case 'M':
            if (ch2 == 'S')
                result = MULTAG_MANUFACTURE;
            else if (ch2 == 'V')
                result = MULTAG_MOVING_TEXT;
            break;
        case 'N':
            if (ch2 == 'L')
                result = MULTAG_NEW_LINE;
            else if (ch2 == 'P')
                result = MULTAG_NEW_PAGE;
            break;
        case 'P':
            if (ch2 == 'B')
                result = MULTAG_COLOR_PAGE_BACKGROUND;
            else if (ch2 == 'T' || ch2 == 'O')
                result = MULTAG_PAGE_TIME;
            break;
        case '/':
            ch3 = toupper(*pMulti++); // third char of tag
            if (ch2 == 'F' && ch3 == 'L')
                result = MULTAG_CLOSE_FLASH;
            else if (ch2 == 'S' && ch3 == 'C')
                result = MULTAG_CLOSE_SPACE_CHAR;
            else if (ch2 == 'M' && ch3 == 'S')
                result = MULTAG_CLOSE_MANUFACTURE;
            break;
        case 'S':
            if (ch2 == 'C')
                result = MULTAG_SPACING_CHAR;
            break;
        case 'T':
            if (ch2 == 'R')
                result = MULTAG_TEXT_RECTANGLE;
            break;
    }
    if (result)
        *pTokenLength = tokenLength;
    return result;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildMultiTag(type_multi_tag tag, uint8_t *pMulti, uint8_t TokenLength,
        HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 result = 0;

    // initialize the MULTI_TAG
    pMultiTag = HNS_MULTI_Tag();

//    pMultiTag->tagId = tag;
//    switch (tag) {
//        case MULTAG_NONE:
//            break;
//        case MULTAG_COLOR_BACKGROUND:
//            result = BuildColorBackgroundTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_COLOR_PAGE_BACKGROUND:
//            result = BuildColorPageBackgroundTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_COLOR_FOREGROUND:
//            result = BuildColorForegroundTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_COLOR_RECTANGLE:
//            result = BuildColorRectangleTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_FIELD:
//            result = BuildFieldTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_FLASH:
//            result = BuildFlashTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_CLOSE_FLASH:
//            result = BuildCloseFlashTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_FONT:
//            result = BuildFontTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_GRAPHIC:
//            result = BuildGraphicTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_HEXCHAR:
//            result = BuildHexCharTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_LINE_JUSTIFICATION:
//            result = BuildLineJustificationTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_PAGE_JUSTIFICATION:
//            result = BuildPageJustificationTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_MANUFACTURE:
//            result = BuildManufacturerTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_CLOSE_MANUFACTURE:
//            result = BuildCloseManufacturerTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_MOVING_TEXT:
//            result = BuildMovingTextTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_NEW_LINE:
//            result = BuildNewLineTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_NEW_PAGE:
//            result = BuildNewPageTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_PAGE_TIME:
//            result = BuildPageTimeTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_SPACING_CHAR:
//            result = BuildSpacingCharTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_CLOSE_SPACE_CHAR:
//            result = BuildCloseSpacingCharTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//        case MULTAG_TEXT_RECTANGLE:
//            result = BuildTextRectangleTag(pMulti, TokenLength,
//                pMultiTag, pError, pErrorIndex);
//            break;
//    }
    return result;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildColorBackgroundTag(uint8_t *, uint8_t,
        HNS_MULTI_Tag &, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildColorPageBackgroundTag(uint8_t *, uint8_t,
        HNS_MULTI_Tag &, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildColorForegroundTag(uint8_t *, uint8_t,
        HNS_MULTI_Tag &, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildColorRectangleTag(uint8_t *, uint8_t,
        HNS_MULTI_Tag &, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildFieldTag(uint8_t *pMulti, uint8_t,
        HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */

    INT16 temp;
    UINT8 numDigits;
    UINT8 *p = pMulti+2;
    UINT8 id;

    HNS_TAG_Field tag_result;

    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && (temp > 0 && temp < 100))
        tag_result.fX() = (UINT8)temp;
    else
        goto errorExit;
    id = (UINT8)temp;
    p += numDigits;

    if (*p++ != ',')
    {
        UINT8 defaultWidth = 0;
        switch (id) {
            case 1:
            case 2:
                defaultWidth = 5;
                break;
            case 6:
            case 8:
            case 9:
            case 10:
                defaultWidth = 2;
                break;
            case 3:
            case 4:
            case 5:
            case 7:
                defaultWidth = 3;
                break;
            case 11:
                defaultWidth = 4;
                break;
            case 12:
            case 13:
                defaultWidth = 8;
        }
        tag_result.fY() = defaultWidth;
        goto exit;
    }

    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && (temp > 0 && temp < 100))
    { // check allowable widths
        UINT8 allowable = 1; // true
        switch (id) {
            case 1:
            case 2:
                allowable = (temp == 5);
                break;
            case 3:
            case 4:
            case 5:
            case 6:
                allowable = (temp == 2 || temp == 3);
                break;
            case 7:
                allowable = (temp == 3);
                break;
            case 8:
            case 9:
            case 10:
                allowable = (temp == 2);
                break;
            case 11:
                allowable = (temp == 4);
                break;
            case 12:
            case 13:
                allowable = (temp == 8);
                break;
        }
        if (!allowable)
            goto errorExit;
        tag_result.fY() = (UINT8)temp;
    }
    else
        goto errorExit;


exit:
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    pMultiTag = tag_result;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    pMultiTag = tag_result;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildFlashTag(uint8_t *pMulti, uint8_t TokenLength,
       HNS_MULTI_Tag &pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 buffer[20], i, ch;
    UINT8 numDigits;
    INT16 temp;
    UINT8 *p;

    if (TokenLength > 19)
        goto errorExit;

    pMultiTag->tag.flash.on = -1;
    pMultiTag->tag.flash.off = -1;
    pMultiTag->tag.flash.order = FLASH_ON_THEN_OFF;
    if (TokenLength == 4)
        goto exit;

    memset(buffer,0,sizeof (buffer));
    memcpy(buffer,pMulti,TokenLength);
    for (i = 0; i < strlen((char *)buffer); i++)
        buffer[i] = toupper(buffer[i]);

    ch = buffer[3];
    if (ch != 'O' && ch != 'T')
        goto errorExit;
    if (ch == 'O')
        pMultiTag->tag.flash.order = FLASH_OFF_THEN_ON;

    p = buffer + 4;
    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits != 0)
    {
        if (temp > 99)
            goto errorExit;
        if (ch == 'O')
            pMultiTag->tag.flash.on = temp;
        else
            pMultiTag->tag.flash.off = temp;
    }
    p += numDigits;
    ch = *p++;
    if (ch == ']') goto exit;
    if (ch != 'O' && ch != 'T')
        goto errorExit;
    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits != 0)
    {
        if (temp > 99)
            goto errorExit;
        if (ch == 'O')
            pMultiTag->tag.flash.on = temp;
        else
            pMultiTag->tag.flash.off = temp;
    }

exit:
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildCloseFlashTag(uint8_t *, uint8_t TokenLength,
        HNS_MULTI_Tag &, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    if (TokenLength != 5)
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        return 0;
    }
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildFontTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 temp;
    UINT8  numDigits;
    UINT8 *pComma;

    pComma = (UINT8 *)strchr((char *)pMulti,',');
    if (pComma != NULL)
    {
        if(pComma < (pMulti + TokenLength))
        {
            numDigits = GetHexInteger(pComma+1, &temp);

            if (numDigits && temp > 0)
                pMultiTag->tag.font.versionID = temp;
            else
                goto errorExit;


            numDigits = GetDecimalInteger(pMulti+3, &temp);
            if (numDigits && temp>0 && fs_multi_interface->fIsValidFontNumber(temp))
                pMultiTag->tag.font.number = (UINT8)temp;
            else
                goto fontNotDefinedError;
        }
    }
    else
    {
        //look for [fo] tag - set default font
        if((UINT8)pMulti[3]==']')
        {
            pMultiTag->tag.font.number = fs_multi_interface->fGetDefaultFont();
        }
        else {
        numDigits = GetDecimalInteger(pMulti+3, &temp);
        if (numDigits && temp>0 && fs_multi_interface->fIsValidFontNumber(temp))
            pMultiTag->tag.font.number = (UINT8)temp;
        else
            goto fontNotDefinedError;
        }
    }
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
fontNotDefinedError:
    *pError = HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED;
    *pErrorIndex = 3;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildGraphicTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 temp;
    UINT8  buffer[20];
    UINT8  numDigits;
    UINT8 *p;

    if (TokenLength > 19)
    {
        goto errorExit;
    }
    memset(buffer,0,sizeof (buffer));
    memcpy(buffer,pMulti,TokenLength);

    p = pMulti + 2;

    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && (temp > 0 && temp <= 255))
        pMultiTag->tag.graphic.number = (UINT8)temp;
    else goto errorExit;

    p += numDigits;
    if (*p == ']') goto exit;
    if (*p++ != ',')
        goto errorExit;

    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && temp > 0 )
        pMultiTag->tag.graphic.x = (UINT8)temp;
    else goto errorExit;
    p += numDigits;
    if (*p++ != ',')
        goto errorExit;
    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && temp > 0 )
        pMultiTag->tag.graphic.y = (UINT8)temp;
    else goto errorExit;

    p += numDigits;
    if (*p == ']') goto exit;
    if (*p++ != ',')
        goto errorExit;

    numDigits = GetHexInteger((UINT8*)p, &temp);
    if (numDigits)
            pMultiTag->tag.graphic.versionID = temp;
    else
        goto errorExit;

exit:
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildHexCharTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */

    INT16 hexChar;
    UINT8 numDigits;
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    if (TokenLength <= 8)
    {
        numDigits = GetHexInteger(pMulti+3, &hexChar);
        if (numDigits && hexChar > 0)
            pMultiTag->tag.hexCharacter = hexChar;
        else
            goto errorExit;
    }
    else
        goto errorExit;

    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildLineJustificationTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{

    /* return 1 if no error,
        0 otherwise

    */
    UINT8 val = *(pMulti+3)-'0';
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;

    if (TokenLength != 5 || val < LINE_JUSTIFICATION_LEFT || val > LINE_JUSTIFICATION_FULL)
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        *pErrorIndex = 3;
        return 0;
    }
    pMultiTag->tag.lineJustification = val;
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildPageJustificationTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 val = *(pMulti+3)-'0';
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;

    //look for [jp] tag - set to default page justification
    if ((UINT8) pMulti[3] == ']') {
        pMultiTag->tag.pageJustification = fs_multi_interface->fGetDefaultPageJustification();
        return 1;
    }

    if (TokenLength != 5 || val < PAGE_JUSTIFICATION_TOP || val > PAGE_JUSTIFICATION_BOTTOM)
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        *pErrorIndex = 3;
        return 0;
    }
    pMultiTag->tag.pageJustification = val;
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildManufacturerTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 temp;


    UINT8  numDigits,i;
    UINT8 *pComma1 = NULL;
    UINT8 *p= pMulti;

    for (i=0; i < TokenLength; i++, p++)
    {
        if (*p == ',')
        {
            pComma1 = p;
            break;
        }

    }

    if (pComma1 != NULL)
    {
        pMultiTag->tag.manufacturerTag.pStartOfText = ++pComma1;
        pMultiTag->tag.manufacturerTag.textLength = TokenLength-2 - i;
    }
    else
    {
        pMultiTag->tag.manufacturerTag.pStartOfText = NULL;
        pMultiTag->tag.manufacturerTag.textLength = 0;
    }


    numDigits = GetDecimalInteger(pMulti+3, &temp);
    if (!numDigits || temp > 0)
        pMultiTag->tag.manufacturerTag.x = (UINT8)temp;
    else goto errorExit;

    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildCloseManufacturerTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 temp;
    UINT8  numDigits,i;
    UINT8 *pComma1 = NULL;
    UINT8 *p= pMulti;

    for (i=0; i < TokenLength; i++, p++)
    {
        if (*p == ',')
        {
            pComma1 = p;
            break;
        }

    }

    if (pComma1 != NULL)
    {
        pMultiTag->tag.manufacturerTag.pStartOfText = ++pComma1;
        pMultiTag->tag.manufacturerTag.textLength = TokenLength-2 - i;
    }
    else
    {
        pMultiTag->tag.manufacturerTag.pStartOfText = NULL;
        pMultiTag->tag.manufacturerTag.textLength = 0;
    }

    numDigits = GetDecimalInteger(pMulti+4, &temp);
    if (!numDigits || temp > 0)
        pMultiTag->tag.manufacturerTag.x = (UINT8)temp;
    else goto errorExit;

    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildMovingTextTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 *p = pMulti+3;
    UINT8 ch,numDigits;
    INT16 temp;

    ch = toupper(*p++);
    switch (ch) {
        case 'C':
            pMultiTag->tag.movingText.movingInfo.c = 1;
            break;
        case 'L':
            pMultiTag->tag.movingText.movingInfo.c = 0;
            if (isdigit(*p))
            {
                numDigits = GetDecimalInteger(p, &temp);
                if (numDigits && temp > 0)
                {
                    pMultiTag->tag.movingText.movingInfo.x = (UINT8)temp;
                    p += numDigits; // p now points to d
                }
                else
                    goto errorExit;

            }
            else
                pMultiTag->tag.movingText.movingInfo.x = 0;
            break;
        default:
            goto errorExit;
    }
    // should be pointing to d
    ch = toupper(*p++);
    if (ch == 'L')
        pMultiTag->tag.movingText.movingInfo.isLeftMoving = 1;
    else if (ch == 'R')
        pMultiTag->tag.movingText.movingInfo.isLeftMoving = 0;
    else
        goto errorExit; // error
    // now w
    numDigits = GetDecimalInteger(p, &temp);
    if (!numDigits || temp <= 0)
        goto errorExit;
    pMultiTag->tag.movingText.movingInfo.w = (UINT8)temp;
    p += numDigits;
    if (*p++ != ',')
        goto errorExit;
    // now s
    numDigits = GetDecimalInteger(p, &temp);
    if (!numDigits || temp <= 0)
        goto errorExit;
    pMultiTag->tag.movingText.movingInfo.s = (UINT8)temp;
    p += numDigits;
    if (*p++ != ',')
        goto errorExit;
    // now r
    numDigits = GetDecimalInteger(p, &temp);
    if (!numDigits || temp <= 0)
        goto errorExit;
    pMultiTag->tag.movingText.movingInfo.r = (UINT8)temp;
    p += numDigits;
    if (*p++ != ',')
        goto errorExit;
    // now text
    pMultiTag->tag.movingText.pStartOfText = p;
    pMultiTag->tag.movingText.textLength = TokenLength -(p-pMulti+1);

    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildNewLineTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 spacing = 0;
    INT16 temp;
    UINT8 numDigits;
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    if (TokenLength == 4)
    {

    }
    else if (TokenLength <= 8)
    {
        numDigits = GetDecimalInteger(pMulti+3, &temp);
        if (numDigits /*&& temp > 0*/)
        {
            spacing = temp;
        }
        else
        {
            *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
            *pErrorIndex = 0;
            return 0;
        }
    }
    else
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        *pErrorIndex = 0;
        return 0;
    }
    pMultiTag->tag.newLine.x = (UINT8)spacing;
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildNewPageTag(uint8_t *, uint8_t TokenLength,
        MULTI_TAG *, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    if (TokenLength != 4)
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        *pErrorIndex = 0;
        return 0;
    }
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildPageTimeTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 buffer[20], i;
    UINT8 *pOnTime,*pOffTime, numDigits;
    INT16 temp;

    if (TokenLength > 19)
        goto errorExit;

    memset((char*)buffer,0,20);
    memcpy((char*)buffer,pMulti,TokenLength);

    pOffTime = (UINT8*)strchr((char*)buffer,'o');
    if (pOffTime == NULL)
        pOffTime = (UINT8*)strchr((char*)buffer,'O');
    pOnTime = (UINT8*)strchr((char*)buffer,'t');
    if (pOnTime == NULL)
        pOnTime = (UINT8*)strchr((char*)buffer,'T');

    pMultiTag->tag.pageTime.on = -1;
    pMultiTag->tag.pageTime.off = -1;

    if (pOnTime != NULL)
        *pOnTime++;
    if (pOffTime != NULL)
        *pOffTime++;

    for (i = 0; i < 20; i++)
        if (!isdigit((UINT16)buffer[i]))
            buffer[i] = 0;

    if (pOnTime != NULL && isdigit(*pOnTime) )
    {
        numDigits = GetDecimalInteger(pOnTime, &temp);
        if (!numDigits ||(temp < 1 || temp > 255))
            goto errorExit;
        pMultiTag->tag.pageTime.on = temp;
    }
    if (pOffTime != NULL && isdigit(*pOffTime))
    {
        numDigits = GetDecimalInteger(pOffTime, &temp);
        if (!numDigits || temp > 255)
            goto errorExit;
        pMultiTag->tag.pageTime.off = temp;
    }
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildSpacingCharTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 spacing;
    UINT8 numDigits;
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    if (TokenLength <= 6)
    {
        numDigits = GetDecimalInteger(pMulti+3, &spacing);
        if (numDigits /*&& spacing */ > 0)   //JGTs
        {
            pMultiTag->tag.spacingChar = (UINT8)spacing;
        }
        else
        {
            *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
            *pErrorIndex = 0;
            return 0;
        }
    }
    else
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        *pErrorIndex = 0;
        return 0;
    }
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildCloseSpacingCharTag(uint8_t *, uint8_t TokenLength,
        MULTI_TAG *, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    if (TokenLength != 5)
    {
        *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
        return 0;
    }
    return 1;
}

int8_t HNS_MESSAGE_V2::HNS_Message::BuildTextRectangleTag(uint8_t *pMulti, uint8_t TokenLength,
        MULTI_TAG *pMultiTag, uint16_t *pError, uint16_t *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    INT16 temp;

    UINT8  buffer[20],i,numDigits;
    UINT8  numberOfCommas = 0;

    UINT8 *pComma1 = NULL,*pComma2 = NULL,*pComma3 = NULL;

    if (TokenLength > 19)
    {
        goto errorExit;
    }
    memset(buffer,0,sizeof (buffer));
    memcpy(buffer,pMulti,TokenLength);

    pComma1 = (UINT8 *)strchr((char *)buffer,',');
    if (pComma1 != NULL)
    {
        numberOfCommas = 1;
        *pComma1++ = 0; // put null
        pComma2 = (UINT8 *)strchr((char *)pComma1,',');
        if (pComma2 != NULL)
        {
            numberOfCommas = 2;
            *pComma2++ = 0; // put null
            pComma3 = (UINT8 *)strchr((char *)pComma2,',');
            if (pComma3 != NULL)
            {
                *pComma3++ = 0;
                numberOfCommas = 3;
            }
        }
    }
    if (numberOfCommas != 3)
        goto errorExit; // error condition

    for (i = 0; i < 20; i++)
        if (!isdigit(buffer[i]))
            buffer[i] = 0;

    numDigits = GetDecimalInteger(buffer+3, &temp);
    if (numDigits && temp > 0)
        pMultiTag->tag.textRectangle.x = (UINT8)temp;
    else goto errorExit;

    numDigits = GetDecimalInteger(pComma1, &temp);
    if (numDigits && temp > 0 )
        pMultiTag->tag.textRectangle.y = (UINT8)temp;
    else goto errorExit;
    numDigits = GetDecimalInteger(pComma2, &temp);
    if (numDigits )
        pMultiTag->tag.textRectangle.w = (UINT8)temp;
    else goto errorExit;
    numDigits = GetDecimalInteger(pComma3, &temp);
    if (numDigits )
        pMultiTag->tag.textRectangle.h = (UINT8)temp;
    else goto errorExit;

    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;
    return 1;
errorExit:
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

uint8_t HNS_MESSAGE_V2::HNS_Message::GetDecimalInteger(uint8_t *p, int16_t *pResult)
{
    UINT8 buffer[8],i;
    INT8 retVal = 0;
    UINT8 numDigits = 0;
    INT32 itemp = 0;


    memset(buffer,0,sizeof(buffer));

    memcpy(buffer,p,5);
    for (i = 0; i < 5; i++)
        if (!isdigit(buffer[i]))
        {
            buffer[i] = 0;
            break;
        }
        else numDigits++;

    if (numDigits && 1 == sscanf((char *)buffer,"%i",&itemp))
        retVal = numDigits;
    *pResult = (INT16)itemp;
    return retVal;
}

UINT8 HNS_MESSAGE_V2::HNS_Message::GetHexInteger(uint8_t *p, int16_t *pResult)
{
    UINT8 buffer[8],i;
    UINT8 retVal = 0;
    UINT8 numDigits = 0;
    INT32 itemp = 0;

    memset((char *)buffer,0,sizeof(buffer));

    memcpy((char *)buffer,p,4);
    for (i = 0; i < 4; i++)
        if (!isxdigit(buffer[i]))
        {
            buffer[i] = 0;
            break;
        }
        else numDigits++;
    if (numDigits && 1 == sscanf((char *)buffer,"%x",&itemp))
        retVal = numDigits;
    *pResult = (INT16)itemp;
    return retVal;
}

/*****************************************************************************
    FUNCTION InitMessageContext

    Description: Initialized a MESSAGE_CONTEXT structure from default
        configuration values

    Entrance Conditions:

    Exit Conditions:

    args:
        MESSAGE_CONTEXT* pContext : points to a MESSAGE_CONTEXT structure to be initialized
    returns:
        no vlue returned

    Notes:

 *****************************************************************************/
void HNS_MESSAGE_V2::HNS_Message::InitMessageContext(MESSAGE_CONTEXT* pContext)
{
    if(fs_multi_interface != nullptr)
    {
        pContext->currentFont = fs_multi_interface->fGetDefaultFont();
        pContext->lineJustification = fs_multi_interface->fGetDefaultLineJustification();
        pContext->pageJustification = fs_multi_interface->fGetDefaultPageJustification();
        pContext->defaultFlashOnTime = fs_multi_interface->fGetDefaultFlashOnTime();
        pContext->defaultFlashOffTime = fs_multi_interface->fGetDefaultFlashOffTime();
        pContext->currentPageOnTime = fs_multi_interface->fGetDefaultPageTimeOn();
        pContext->currentPageOffTime = fs_multi_interface->fGetDefaultPageTimeOff();
    }
}

/*****************************************************************************
    FUNCTION InitializePage

    Description: Intializes a PAGE structure

    Entrance Conditions:

    Exit Conditions:

    args:
        UINT8 isNewPage : true if this is a new page, false otherwise
        PAGE* pPage : points to the PAGE structure to be initialized
        MESSAGE_CONTEXT* pContext : points to a MESSAGE_CONTEXT structure that contains
            the context for the current message that is being displayed
    returns:
        no value returned

    Notes:

 *****************************************************************************/
void HNS_MESSAGE_V2::HNS_Message::InitializePage(uint8_t isNewPage, PAGE* pPage, MESSAGE_CONTEXT* pContext)
{
//    if (isNewPage)
//        memset(pPage,0,sizeof(PAGE));
//    else
//    {
//        memset(pPage->textWindow,0,sizeof pPage->textWindow);

//        pPage->numberGraphics = 0;
//        memset(pPage->graphics,0, sizeof pPage->graphics);

//        pPage->numberChars = 0;
//        memset(pPage->chars,0, sizeof pPage->chars);
//    }
//    pPage->numberTextWindows = 1;

//    InitializeTextWindow(
//            &(pPage->textWindow[0]),
//            0, //TopLeftY relative to sign face
//            0, // TopLeftX relative to sign face
//            fs_sign_info_interface->fGetSignBrdInfo().fGetWidth(), // width in pixels
//            fs_sign_info_interface->fGetSignBrdInfo().fGetHeight(), // height in pixels
//            pContext->lineJustification,
//            pContext->pageJustification
//            );
}

int16_t HNS_MESSAGE_V2::HNS_Message::BuildTokens(vector<HNS_Token> &tokens,
                  uint16_t maxNumberOfTokens,
                  uint8_t *pMultiString,
                  int16_t *pError,
                  int16_t *pSyntaxError,
                  uint16_t *pMultiIndexOfError)
{
    // returns number of tokens built
    UINT8 multiIndex = 0, tokenIndex = 0,tokenValueIndex = 0;
    UINT8 tokenLength = 0;
    UINT16 multiLen = strlen((char *)pMultiString);
    UINT8 *pMulti = pMultiString;
    UINT8 ch,nextCh;
    INT8 result;
    type_multi_tag tag;
    UINT16 syntaxError, multiIndexOfError;
    UINT8 i,orderInMultiString;
    UINT8 lastPageJustificationTag = 0;

    tokens.clear();
    tokens.push_back(HNS_Token());

    *pError = 0;
    *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pMultiIndexOfError = 0;

    tokens.back().fTokenType() = TOK_TEXT; // at least one text token for
                                             // a null multi string

    while (*pMulti  && tokens.size() < maxNumberOfTokens)
    {
        ch = *pMulti;
        nextCh = *(pMulti+1);
        if ((ch == '[' && nextCh == '[')
            || (ch == ']' && nextCh == ']')
            || (ch != '[' && ch != ']'))
        {
            tokenLength++;
            if ( (ch == ']' || ch == '['))
            {
                pMulti++;
                tokenLength++;
            }
        }
        else if (ch == '[') // must be opening bracket of tag
        {
            if (tokenLength) // last token had some length
            {
                tokens.back().fTokenLength() = (UINT8)tokenLength;
                tokenIndex++;
                tokens.push_back(HNS_Token());
                tokens.back().fMultiStringIndex() = pMulti-pMultiString;
            }
            tag = GetMultiTag(pMulti,&tokenLength);
            if (tag == 0)
            { // error
                *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                *pMultiIndexOfError = pMulti - pMultiString;
                return 0; // number of tokens
            }
            // add something here?
//            result = BuildMultiTag(tag, pMulti, tokenLength,
//                &tokens[tokenIndex].multiTag, &syntaxError, &multiIndexOfError);

//            if (!result)
//            {
//                *pSyntaxError = syntaxError;
//                *pMultiIndexOfError = pMulti - pMultiString + multiIndexOfError;
//                return tokenIndex+1; // number of tokens
//            }

//            if (tag == MULTAG_NEW_PAGE)
//                lastPageJustificationTag = 0;

//            if (tag == MULTAG_PAGE_JUSTIFICATION)
//            {
//                UINT8 pageJustification = tokens[tokenIndex].multiTag.tag.pageJustification;
//                if (pageJustification < lastPageJustificationTag)
//                {
//                    *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_TAGCONFLICT;
//                    *pMultiIndexOfError = pMulti - pMultiString + multiIndexOfError;
//                    return tokenIndex+1; // number of tokens
//                }
//                else
//                    lastPageJustificationTag = pageJustification;
//            }

//            tokens[tokenIndex].tokenType = TOK_MULTI_TAG;
//            tokens[tokenIndex].multiStringIndex = pMulti-pMultiString;
//                tokens[tokenIndex].tokenLength = tokenLength;
//            tokens[tokenIndex].tagValue = tag;
//            pMulti += tokenLength-1; // last char in tag

//            tokenIndex++;
//            tokenLength = 0;
//            tokens[tokenIndex].tokenType = TOK_TEXT;
//            tokens[tokenIndex].multiStringIndex = pMulti+1-pMultiString;
        }
//        else if (ch == ']') //standalone right bracket
//        {
//            *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
//            *pMultiIndexOfError = pMulti - pMultiString;
//            return 0; // number of tokens
//        }
//        else
//            tokenLength++;
//        pMulti++;
        break;
    }// while *pMulti

//    if (tokenLength) // then there is a last token
//    {
//        tokens[tokenIndex].tokenLength = tokenLength;
//    }

//    while(tokenIndex)
//    {
//        if (tokens[tokenIndex].tokenLength > 0)
//            break;
//        tokenIndex--;
//    }

//    // for field tags, place their order in the mulistring into the field tag
//    orderInMultiString = 1;
//    for (i=0; i<tokenIndex+1; i++)
//    {
//        if (tokens[i].multiTag.tagId == MULTAG_FIELD)
//            tokens[i].multiTag.tag.field.orderInMultiString = orderInMultiString++;
//    }

    return tokenIndex + 1; // number of tokens
}

int16_t HNS_MESSAGE_V2::HNS_Message::ValidateMultiString(uint8_t *pMultiString,
                          int16_t *pError,
                          int16_t *pSyntaxError,
                          int16_t *pMultiIndexOfError)
{
//    TOKEN tokens[MAX_TOKENS];
//    UINT16 numTokens;
//    INT16 error;
//    INT16 syntaxError;
//    UINT16 multiIndexOfError;
//    UINT16 index;
//    UINT16 ticks = 0;
//    MESSAGE_CONTEXT messageContext;
//    PAGE   page;

//    numTokens = BuildTokens(tokens,
//                  MAX_TOKENS,
//                  pMultiString,
//                  &error,
//                  &syntaxError,
//                  &multiIndexOfError);



//    if (syntaxError != HNS_MULTI_SYNTAX_ERROR_NONE)
//    {
//        *pError = error;
//        *pSyntaxError = syntaxError;
//        *pMultiIndexOfError = multiIndexOfError;
//        return 0;
//    }
//    index = 0;
//    InitMessageContext(&messageContext);
//    InitializePage(1,&page,&messageContext); // is new page

//    do {
//        if (index != 0)
//            index++; // skip new page tag
//        UpdatePage(
//                1, // assume is new page
//                &page,
//                pMultiString, // input - pointer to multistring
//                tokens, // input - array of tokens
//                numTokens, // input- number of tokens
//                index, // index of first token in page
//                ticks, // input - number of 0.1 sec ticks since page first painted
//                &messageContext,// input,output
//                (UINT16 *)&syntaxError, // output - error
//                &multiIndexOfError, // output - index in multistring of error
//                (UINT8 *)NULL // output - bit map
//                );

//        if (syntaxError != HNS_MULTI_SYNTAX_ERROR_NONE)
//        {
//            *pError = error;
//            *pSyntaxError = syntaxError;
//            *pMultiIndexOfError = multiIndexOfError;
//            return 0;
//        }

//        index = IndexOfNextNewPageToken(index, // starting index
//                  tokens, // input tokens
//                  numTokens // input max number of tokens
//                  );


//    } while (index != 0);


//    *pError = 0;
//    *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_NONE;
//    *pMultiIndexOfError = 0;
//    return (INT16)numTokens;
    return 0;
}

void HNS_MESSAGE_V2::HNS_Message::fSetMultiInfoInterface(HNS_Multi_Info_Interface *interface)
{
    fs_multi_interface = interface;
}

void HNS_MESSAGE_V2::HNS_Message::fSetSignInfoInterface(HNS_Sign_Info_Interface *interface)
{
    fs_sign_info_interface = interface;
}


HNS_MESSAGE_V2::HNS_Message_Moving_Text_Window::HNS_Message_Moving_Text_Window()
{

}
