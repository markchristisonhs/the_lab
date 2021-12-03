#ifndef HNS_NTCIP_XML_H
#define HNS_NTCIP_XML_H

#include <NTCIP.h>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class HNS_NTCIP_XML
{
public:
    HNS_NTCIP_XML();
    HNS_NTCIP_XML(const QString &file);

    void fSetFile(const QString &file);

    void fParseNTCIPFile(NTCIP_Node *tree);

    void fWriteNTCIPFile(NTCIP_Node *tree);
private:

    void fGetNTCIPTree(QXmlStreamReader *xml, NTCIP_Node *tree, const QString &parent_oid);

    void fWriteNTCIPBranch(QXmlStreamWriter *xml, NTCIP_Node *tree);
    QString f_file;
};

#endif // HNS_NTCIP_XML_H
