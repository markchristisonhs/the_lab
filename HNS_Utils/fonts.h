/*
 * fonts.h
 *
 *  Created on: Aug 26, 2019
 *      Author: mchristiso
 */

#ifndef SRC_FONTS_H_
#define SRC_FONTS_H_

#include <vector>
#include <string>
#include "graphics.h"

static const size_t MAX_NUM_CHARACTERS = 128;
static const size_t MAX_NUM_FONTS = 8;

class HNS_Character;
class HNS_Font;
class HNS_Bitmap;

class HNS_Font
{
public:
	HNS_Font();
	~HNS_Font();

    HNS_Character fGetCharacter(const int &character_number) const;
    std::vector<HNS_Character> fGetCharacters() const;
    size_t fGetNumCharacters() const;
    HNS_Character fGetCharacterByIndex (const size_t &index) const;
	void fAddCharacter(const HNS_Character &character);
    void fChangeCharacter(const HNS_Character &character, const int &character_number);
	void fRemoveCharacter(const int &character_number);

    bool fIsCharValid(const int &character_number) const;

	void fSetFontNumber(const int &font_number);
	int fGetFontNumber() const;
	bool fCharacterExists(const int &character_number) const;

	int fGetFontHeight() const;
    void fSetFontHeight(const int &height);
	int fGetMaxFontWidth() const;

    void fSetLineSpacing(const int &spacing);
    int fGetLineSpacing() const;
    void fSetCharSpacing(const int &spacing);
    int fGetCharSpacing() const;

    void fSetFontName(const std::string &name);
    std::string fGetFontName() const;

    uint16_t fGetVersionID() const;
    static HNS_Character f_null_character;
private:
	std::vector <HNS_Character> f_characters;
    std::string f_fontname;
	int f_font_number;
	int f_maxheight,f_maxwidth;
    int f_char_spacing,f_line_spacing;

};

class HNS_Character
{
public:
	HNS_Character();
	HNS_Character(const HNS_Character &copy);
    HNS_Character(const bool *data, const int &width, const int &height, const int &character_number);
	~HNS_Character();

	HNS_Character &operator=(const HNS_Character &rhs);

    //Sets data using a byte array
    void fSetData(const HNS_Bitmap &data);
    void fSetData(const std::vector<unsigned char> &data, const int &width, const int &height);
    void fSetData(const bool *data, const long long &datasize, const int &width, const int &height);
    bool *fGetData() const;
    HNS_Bitmap fGetBitmap() const;
    //gets the data formatted in an octetstring for NTCIP
    std::vector<unsigned char> fGetNTCIPData() const;

	void fSetCharacterNumber(const int &character_number);
	int fGetCharacterNumber() const;

	int fGetHeight() const;
	int fGetWidth() const;

    bool operator==(const HNS_Character &rhs);
    bool operator!=(const HNS_Character &rhs);
private:
	int f_character_number;
	int f_width,f_height;
    bool *f_data;
	long long f_datasize;
};



#endif /* SRC_FONTS_H_ */
