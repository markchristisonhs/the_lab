#include "HNS_NTCIP_1203.h"
#include <utilities.h>
#include <vector>
#include <OER.h>
#include <CRC.h>

using namespace std;

vector<__uint8_t> Encode_CharacterInformation(const HNS_Character &character);
vector<__uint8_t> Encode_CharacterInfoList(const vector<HNS_Character> &characters);
vector<__uint8_t> Encode_FontVersionByteStream(const HNS_Font &font);
vector<__uint8_t> Encode_FontInformation(const HNS_Font &font);

vector<__uint8_t> Encode_GraphicInfoList(const HNS_Graphic &graphic);

__uint16_t Calculate_fontVersionID(const HNS_Font &font)
{
    __uint16_t result = 0;

    vector<__uint8_t> temp_vec = Encode_FontVersionByteStream(font);

    result = CalcCRC(temp_vec);
    result = SwapCRCEndianness(result);

    return result;
}

__uint16_t Calculate_graphicID(const HNS_Graphic &graphic)
{
    __uint16_t result = 0;

    vector<__uint8_t> temp_vec = Encode_GraphicInfoList(graphic);

    result = CalcCRC(temp_vec);
    result = SwapCRCEndianness(result);

    return result;
}

vector<__uint8_t> Encode_FontVersionByteStream(const HNS_Font &font)
{
    vector<__uint8_t> result, temp_vec;

    result = Encode_FontInformation(font);
    temp_vec = Encode_CharacterInfoList(font.fGetCharacters());
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    return result;
}

vector<__uint8_t> Encode_FontInformation(const HNS_Font &font)
{
    vector<__uint8_t> result;

    result.push_back((font.fGetFontNumber() & 0xFF));
    result.push_back((font.fGetFontHeight() & 0xFF));
    result.push_back((font.fGetCharSpacing() & 0xFF));
    result.push_back((font.fGetLineSpacing() & 0xFF));

    return result;
}

vector<__uint8_t> Encode_CharacterInformation(const HNS_Character &character)
{
    vector<__uint8_t> result, temp_vec;

    temp_vec = HNS_intToByteArray(character.fGetCharacterNumber());
    //trim it down to two bytes
    while(temp_vec.size() > 2)
    {
        temp_vec.erase(temp_vec.begin());
    }
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    //character width
    result.push_back(static_cast<unsigned char>(character.fGetWidth()));

    //character bitmap
    //Remember that character bitmap in CharacterInformation is defined as an octet string of unrestricted size!
    temp_vec = character.fGetNTCIPData();
    temp_vec = Encode_Unrestricted_OctetString(temp_vec);
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    return result;
}

vector<__uint8_t> Encode_CharacterInfoList(const vector<HNS_Character> &characters)
{
    vector<__uint8_t> result, temp_vec;

    result = Encode_NonNegativeUnrestrictedInteger(characters.size());
    for(size_t ui=0;ui<characters.size();ui++)
    {
        temp_vec = Encode_CharacterInformation(characters[ui]);
        result.insert(result.end(),temp_vec.begin(),temp_vec.end());
    }

    return result;
}

vector<__uint8_t> Encode_GraphicInfoList(const HNS_Graphic &graphic)
{
    vector<__uint8_t> result;
    int temp;
    HNS_Bitmap temp_bitmap = graphic.fGetBitmap();
    vector<unsigned char> temp_bitmap_bytes = temp_bitmap.fGetByteArray();

    result.push_back(graphic.fGetGraphicNumber() & 0xFF);
    temp = temp_bitmap.fGetHeight();
    result.push_back((temp >> 8) & 0xFF);
    result.push_back(temp & 0xFF);

    temp = temp_bitmap.fGetWidth();
    result.push_back((temp >> 8) & 0xFF);
    result.push_back(temp & 0xFF);

    //type is monochrome1bit
    result.push_back(0x1);
    //no transparent
    result.push_back(0x0);
    //transparent color is 0x00 00 00
    result.push_back(0x0);
    result.push_back(0x0);
    result.push_back(0x0);

    result.insert(result.end(),temp_bitmap_bytes.begin(),temp_bitmap_bytes.end());

    return result;
}
