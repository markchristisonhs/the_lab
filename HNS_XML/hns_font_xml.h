#ifndef HNS_FONT_XML_H
#define HNS_FONT_XML_H

#include <vector>
#include <fonts.h>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class HNS_Font_XML
{
public:
    HNS_Font_XML();
    HNS_Font_XML(const QString &file);

    void fSetFile(const QString &file);

    std::vector<HNS_Font> fGetFontsFromFile();
private:
    QString f_file;

    void fLoadFont(QXmlStreamReader *xml, std::vector<HNS_Font> &font_output);
    bool fLoadCharacters(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font);
    bool fLoadCharacter(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font);
};

#endif // HNS_FONT_XML_H
