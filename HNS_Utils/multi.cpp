#include "multi.h"

#include <ctype.h>
#include <stdio.h>
#include <cstring>

#include <message.h>

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;

//TODO: Link this in to my NTCIP system for the CPU-7M somehow.
UINT8 isValidFontNumber(UINT16 fontNumber);
UINT8 getDefaultFont(void);
UINT8 defaultJustificationLine(void);
UINT8 defaultJustificationLineActivate(void);
UINT8 defaultJustificationPage(void);
UINT8 defaultJustificationPageActivate(void);

INT8 GetMultiTag(UINT8 *pMulti,UINT8 *pTokenLength);

INT8 BuildMultiTag(UINT8 tag, UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);

INT8 BuildColorBackgroundTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildColorPageBackgroundTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildColorForegroundTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildColorRectangleTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildFieldTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildFlashTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildCloseFlashTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildFontTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildGraphicTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildHexCharTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildLineJustificationTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildPageJustificationTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildManufacturerTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildCloseManufacturerTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildMovingTextTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildNewLineTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildNewPageTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildPageTimeTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildSpacingCharTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildCloseSpacingCharTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);
INT8 BuildTextRectangleTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex);

void SetMULTIInterface(HNS_Multi_Info_Interface *interface)
{

}

//TODO Link this to the CPU-7M's NTCIP system.  For now, just return 1
UINT8 isValidFontNumber(UINT16 /*fontNumber*/)
{
//    UINT8 index;

//    for (index = 0; index < MaxNumFonts(); index++)
//        if (fontTable[index].fontNumber == fontNumber)
//        {
//            return 1; //success

//        }
    return 1; // failure
}

UINT8 getDefaultFont(void)
{
    return 1;
}

// ===============================
UINT8 defaultJustificationLine(void)
{
    return LINE_JUSTIFICATION_CENTER;
}
// ===============================
UINT8 defaultJustificationLineActivate(void)
{
    return LINE_JUSTIFICATION_CENTER;
}
// ===============================
UINT8 defaultJustificationPage(void)
{
    return PAGE_JUSTIFICATION_MIDDLE;
}
// ===============================
UINT8 defaultJustificationPageActivate(void)
{
    return PAGE_JUSTIFICATION_MIDDLE;
}

UINT8 GetDecimalInteger(UINT8 *p, INT16 *pResult)
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


// -----------------------------------------
void GetFieldString(UINT8 *p, UINT8 id, UINT8)
{
    UINT8	sField[MAX_FIELD_DESC_LENGTH];
    memset(sField,0,sizeof sField);

    switch (id) {
        case 1: // local time 12 hr format, no AM/PM
            strcpy((char *)sField," 9:00");
            break;
        case 2: // local time 24 hr format
            strcpy((char *)sField,"13:00");
            break;
        case 3: // ambient temp deg C
            strcpy((char *)sField,"40");
            break;
        case 4: // ambient temp deg F
            strcpy((char *)sField,"72");
            break;
        case 5: // speed Km/h
            sprintf((char *)sField,"%i",55);
            //strcpy((char *)sField,"90");
            break;
        case 6: // speed mph
            sprintf((char *)sField,"%i",55);

            //strcpy((char *)sField,"65");
            break;
        case 7: // day of week
            strcpy((char *)sField,"MON");
            break;
        case 8: // day of month
            strcpy((char *)sField,"05");
            break;
        case 9: // month
            strcpy((char *)sField,"05");
            break;
        case 10: // Year 2 digits
            strcpy((char *)sField,"14");
            break;
        case 11: // Year 4 digits
            strcpy((char *)sField,"2014");
            break;
        case 12: // Local Time 12 hr format - upper case AM/PM
            strcpy((char *)sField," 9:00 AM");
            break;
        case 13: // Local Time 12 hr format -lower case am/pm
            strcpy((char *)sField,"11:59 pm");
            break;


    }
    strcpy((char *)p,(char *)sField);
}


// -----------------------------------------
UINT8 GetHexInteger(UINT8 *p, INT16 *pResult)
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


// -----------------------------------------
INT16 BuildTokens(TOKEN tokens[],
                  UINT16 maxNumberOfTokens, // input max number of tokens
                  UINT8 *pMultiString,
                  INT16 *pError,
                  INT16 *pSyntaxError,
                  UINT16 *pMultiIndexOfError)
{
    // returns number of tokens built
    UINT8 multiIndex = 0, tokenIndex = 0,tokenValueIndex = 0;
    UINT8 tokenLength = 0;
    UINT16 multiLen = strlen((char *)pMultiString);
    UINT8 *pMulti = pMultiString;
    UINT8 ch,nextCh;
    INT8 result, tag;
    UINT16 syntaxError, multiIndexOfError;
    UINT8 i,orderInMultiString;
    UINT8 lastPageJustificationTag = 0;

    memset(tokens,0, maxNumberOfTokens *sizeof (TOKEN)); // initialize

    *pError = 0;
    *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pMultiIndexOfError = 0;

    tokens[tokenIndex].tokenType = TOK_TEXT; // at least one text token for
                                             // a null multi string

    while (*pMulti  && tokenIndex < maxNumberOfTokens)
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
                tokens[tokenIndex].tokenLength = (UINT8)tokenLength;
                tokenIndex++;
                tokens[tokenIndex].multiStringIndex = pMulti-pMultiString;
            }
            tag = GetMultiTag(pMulti,&tokenLength);
            if (tag == 0)
            { // error
                *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
                *pMultiIndexOfError = pMulti - pMultiString;
                return 0; // number of tokens
            }
            // add something here?
            result = BuildMultiTag(tag, pMulti, tokenLength,
                &tokens[tokenIndex].multiTag, &syntaxError, &multiIndexOfError);

            if (!result)
            {
                *pSyntaxError = syntaxError;
                *pMultiIndexOfError = pMulti - pMultiString + multiIndexOfError;
                return tokenIndex+1; // number of tokens
            }

            if (tag == MULTAG_NEW_PAGE)
                lastPageJustificationTag = 0;

            if (tag == MULTAG_PAGE_JUSTIFICATION)
            {
                UINT8 pageJustification = tokens[tokenIndex].multiTag.tag.pageJustification;
                if (pageJustification < lastPageJustificationTag)
                {
                    *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_TAGCONFLICT;
                    *pMultiIndexOfError = pMulti - pMultiString + multiIndexOfError;
                    return tokenIndex+1; // number of tokens
                }
                else
                    lastPageJustificationTag = pageJustification;
            }

            tokens[tokenIndex].tokenType = TOK_MULTI_TAG;
            tokens[tokenIndex].multiStringIndex = pMulti-pMultiString;
                tokens[tokenIndex].tokenLength = tokenLength;
            tokens[tokenIndex].tagValue = tag;
            pMulti += tokenLength-1; // last char in tag

            tokenIndex++;
            tokenLength = 0;
            tokens[tokenIndex].tokenType = TOK_TEXT;
            tokens[tokenIndex].multiStringIndex = pMulti+1-pMultiString;
        }
        else if (ch == ']') //standalone right bracket
        {
            *pSyntaxError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
            *pMultiIndexOfError = pMulti - pMultiString;
            return 0; // number of tokens
        }
        else
            tokenLength++;
        pMulti++;
    }// while *pMulti

    if (tokenLength) // then there is a last token
    {
        tokens[tokenIndex].tokenLength = tokenLength;
    }

    while(tokenIndex)
    {
        if (tokens[tokenIndex].tokenLength > 0)
            break;
        tokenIndex--;
    }

    // for field tags, place their order in the mulistring into the field tag
    orderInMultiString = 1;
    for (i=0; i<tokenIndex+1; i++)
    {
        if (tokens[i].multiTag.tagId == MULTAG_FIELD)
            tokens[i].multiTag.tag.field.orderInMultiString = orderInMultiString++;
    }

    return tokenIndex + 1; // number of tokens
}

// =================================================
INT8 BuildMultiTag(UINT8 tag, UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 result = 0;

    // initialize the MULTI_TAG
    memset(pMultiTag,0, sizeof (MULTI_TAG));

    pMultiTag->tagId = tag;
    switch (tag) {
        case MULTAG_NONE:
            break;
        case MULTAG_COLOR_BACKGROUND:
            result = BuildColorBackgroundTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_COLOR_PAGE_BACKGROUND:
            result = BuildColorPageBackgroundTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_COLOR_FOREGROUND:
            result = BuildColorForegroundTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_COLOR_RECTANGLE:
            result = BuildColorRectangleTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_FIELD:
            result = BuildFieldTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_FLASH:
            result = BuildFlashTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_CLOSE_FLASH:
            result = BuildCloseFlashTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_FONT:
            result = BuildFontTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_GRAPHIC:
            result = BuildGraphicTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_HEXCHAR:
            result = BuildHexCharTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_LINE_JUSTIFICATION:
            result = BuildLineJustificationTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_PAGE_JUSTIFICATION:
            result = BuildPageJustificationTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_MANUFACTURE:
            result = BuildManufacturerTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_CLOSE_MANUFACTURE:
            result = BuildCloseManufacturerTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_MOVING_TEXT:
            result = BuildMovingTextTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_NEW_LINE:
            result = BuildNewLineTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_NEW_PAGE:
            result = BuildNewPageTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_PAGE_TIME:
            result = BuildPageTimeTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_SPACING_CHAR:
            result = BuildSpacingCharTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_CLOSE_SPACE_CHAR:
            result = BuildCloseSpacingCharTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
        case MULTAG_TEXT_RECTANGLE:
            result = BuildTextRectangleTag(pMulti, TokenLength,
                pMultiTag, pError, pErrorIndex);
            break;
    }
    return result;
}

// =================================================
INT8 GetMultiTag(UINT8 *pMulti,UINT8 *pTokenLength)
{
    // pMulti - points to start of multi string
    // pTokenLength - points to location to receive length of
    //				  the multitag
    // returns the MULTITAG
    INT8 result = MULTAG_NONE;

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

// ========================================================
INT8 BuildColorBackgroundTag(UINT8 *, UINT8 ,
        MULTI_TAG *, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

// ========================================================
INT8 BuildColorPageBackgroundTag(UINT8 *, UINT8 ,
        MULTI_TAG *, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

// ========================================================
INT8 BuildColorForegroundTag(UINT8 *, UINT8 ,
        MULTI_TAG *, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

// ========================================================
INT8 BuildColorRectangleTag(UINT8 *, UINT8 ,
        MULTI_TAG *, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    *pError = HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG;
    *pErrorIndex = 0;
    return 0;
}

// ========================================================
INT8 BuildFieldTag(UINT8 *pMulti, UINT8 ,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */

    INT16 temp;
    UINT8 numDigits;
    UINT8 *p = pMulti+2;
    UINT8 id;

    numDigits = GetDecimalInteger(p, &temp);
    if (numDigits && (temp > 0 && temp < 100))
        pMultiTag->tag.field.x = (UINT8)temp;
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
        pMultiTag->tag.field.y = defaultWidth;
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
        pMultiTag->tag.field.y = (UINT8)temp;
    }
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

// ========================================================
INT8 BuildFlashTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildCloseFlashTag(UINT8 *, UINT8 TokenLength,
        MULTI_TAG *, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildFontTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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
            if (numDigits && temp>0 && isValidFontNumber(temp))
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
            pMultiTag->tag.font.number = getDefaultFont();
        }
        else {
        numDigits = GetDecimalInteger(pMulti+3, &temp);
        if (numDigits && temp>0 && isValidFontNumber(temp))
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

// ========================================================
INT8 BuildGraphicTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildHexCharTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildLineJustificationTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildPageJustificationTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
{
    /* return 1 if no error,
        0 otherwise

    */
    UINT8 val = *(pMulti+3)-'0';
    *pError = HNS_MULTI_SYNTAX_ERROR_NONE;
    *pErrorIndex = 0;

    //look for [jp] tag - set to default page justification
    if ((UINT8) pMulti[3] == ']') {
        pMultiTag->tag.pageJustification = defaultJustificationPage();
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

// ========================================================
INT8 BuildManufacturerTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

// ========================================================
INT8 BuildCloseManufacturerTag(UINT8 *pMulti, UINT8 TokenLength,
        MULTI_TAG *pMultiTag, UINT16 *pError, UINT16 *pErrorIndex)
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

