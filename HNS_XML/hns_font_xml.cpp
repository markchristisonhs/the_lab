#include "hns_font_xml.h"

#include <QFile>
#include <QImage>
#include <hns_qt_toolkit.h>

using namespace std;

HNS_Font_XML::HNS_Font_XML()
{

}

HNS_Font_XML::HNS_Font_XML(const QString &file)
{
    fSetFile(file);
}

void HNS_Font_XML::fSetFile(const QString &file)
{
    f_file = file;
}

vector<HNS_Font> HNS_Font_XML::fGetFontsFromFile()
{
    QStringRef tempstring_ref;
    QXmlStreamReader xml;
    QFile file(f_file);
    QXmlStreamReader::TokenType token_type;
    bool at_end = false;
    vector<HNS_Font> result;

    if(file.open(QIODevice::ReadWrite))
    {
        xml.setDevice(&file);
        do
        {
            token_type = xml.readNext();
            if(token_type == QXmlStreamReader::Invalid || token_type == QXmlStreamReader::EndDocument)
            {
                at_end = true;
            }
            else if(token_type == QXmlStreamReader::StartDocument)
            {
                tempstring_ref = xml.documentVersion();
            }
            else if(token_type == QXmlStreamReader::DTD)
            {
                tempstring_ref = xml.dtdName();
            }
            else if(token_type == QXmlStreamReader::StartElement)
            {
                tempstring_ref = xml.name();
                if(xml.name() == "HNS_Font")
                {
                    fLoadFont(&xml,result);
                }
                else
                {
                    at_end = true;
                }
            }
            else if(token_type == QXmlStreamReader::EndElement)
            {

            }
        }while(!at_end);
    }

    return result;
}

void HNS_Font_XML::fLoadFont(QXmlStreamReader *xml, vector <HNS_Font> &font_output)
{
    bool found_end = false;
    QXmlStreamReader::TokenType token_type;
    QStringRef tempstring_ref;
    QString font_name, tempstring;
    int font_number;
    HNS_Font current_font;
    QImage font_image;

    QString image_root = f_file;
    int temp = image_root.lastIndexOf("/");
    image_root = image_root.left(temp);

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(xml->name().contains(QString("font"),Qt::CaseInsensitive))
            {
                font_name = tempstring = xml->name().toString();
                font_number = tempstring.remove(QString("font"), Qt::CaseInsensitive).toInt();
                font_name = image_root + QString("/") + font_name + QString(".bmp");
/*#ifdef HNS_UNIX
                font_name = getenv("HOME") + QString("/HNS/fonts/") + font_name + QString(".bmp");
#elif HNS_WIN32
                font_name = font_name + QString(".bmp");
#endif*/

                current_font.fSetFontNumber(font_number);

                font_image = QImage(font_name,"bmp");

                if(fLoadCharacters(xml, font_image, current_font))
                {
                    font_output.push_back(current_font);
                    current_font = HNS_Font();
                }
                else
                {
                    found_end = true;
                }
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            found_end = true;
        }
        else if(token_type == QXmlStreamReader::Characters)
        {
            tempstring_ref = xml->text();
        }
        else
        {
            found_end = true;
        }
    }while(!found_end);
}

bool HNS_Font_XML::fLoadCharacters(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font)
{
    bool success = true;
    bool found_end = false;
    QXmlStreamReader::TokenType token_type;

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(xml->name() == QString("character"))
            {
                fLoadCharacter(xml,font_image,font);
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            found_end = true;
        }
        else if(token_type == QXmlStreamReader::Characters)
        {

        }
        else
        {
            found_end = true;
            success = false;
        }
    }while(!found_end);

    return success;
}

bool HNS_Font_XML::fLoadCharacter(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font)
{
    bool success = true;
    bool found_end = false;
    int character_number;
    int x=0,y=0,height=0,width=0;
    QXmlStreamReader::TokenType token_type;
    int state = -1; // -1 is looking for a tag, 0 is character number, 1 is x, 2 is y, 3 is height, and 4 is width
    HNS_Character current_character;
    QImage temp_image;

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(state == -1)
            {
                if(xml->name() == QString("char_number"))
                {
                    state = 0;
                }
                else if(xml->name() == QString("x"))
                {
                    state = 1;
                }
                else if(xml->name() == QString("y"))
                {
                    state = 2;
                }
                else if(xml->name() == QString("height"))
                {
                    state = 3;
                }
                else if(xml->name() == QString("width"))
                {
                    state = 4;
                }
            }
            else
            {
                found_end = true;
                success = false;
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            if(state == -1)
            {
                found_end = true;
            }
            else
            {
                state = -1;
            }
        }
        else if(token_type == QXmlStreamReader::Characters)
        {
            if(!xml->isWhitespace())
            {
                switch(state)
                {
                case 0:
                    character_number = xml->text().toInt();
                    break;
                case 1:
                    x = xml->text().toInt();
                    break;
                case 2:
                    y = xml->text().toInt();
                    break;
                case 3:
                    height = xml->text().toInt();
                    break;
                case 4:
                    width = xml->text().toInt();
                    break;
                }
            }
        }
        else
        {
            found_end = true;
            success = false;
        }
    }while(!found_end);

    if(success)
    {
        bool *data = nullptr;
        temp_image = font_image.copy(x,y,width,height);
        data = new bool[temp_image.width() * temp_image.height()];
        //f_characters[f_characters.size()-1].push_back(temp_image);
        //f_character_numbers[f_character_numbers.size()-1].push_back(character_number);
        ConvertQImageToBoolArray(temp_image,Qt::white,data,temp_image.width() * temp_image.height());
        current_character.fSetData(data,temp_image.width() * temp_image.height(),width,height);
        current_character.fSetCharacterNumber(character_number);
        font.fAddCharacter(current_character);
        delete [] data;
    }

    return success;
}
