#include "hns_ntcip_xml.h"

#include <QFile>

HNS_NTCIP_XML::HNS_NTCIP_XML()
{
}

HNS_NTCIP_XML::HNS_NTCIP_XML(const QString &file)
{
    f_file = file;
}

void HNS_NTCIP_XML::fSetFile(const QString &file)
{
    f_file = file;
}

bool HNS_NTCIP_XML::fParseNTCIPFile(NTCIP_Node *tree)
{
    bool success = true;
    QStringRef tempstring_ref;
    QFile file(f_file);
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    bool at_end = false;
    QXmlStreamAttributes test;
    QString version,ntcip_version;

    if(file.open(QIODevice::ReadOnly))
    {
        xml.setDevice(&file);
        do
        {
            token = xml.readNext();
            if(token == QXmlStreamReader::Invalid || token == QXmlStreamReader::EndDocument)
            {
                at_end = true;
            }
            else if(token == QXmlStreamReader::DTD)
            {
                tempstring_ref = xml.dtdName();
            }
            else if(token == QXmlStreamReader::StartElement)
            {
                tempstring_ref = xml.name();
                if(xml.name() == "NTCIP")
                {
                    test = xml.attributes();
                    version = xml.attributes().value("","HNS_version").toString();
                    ntcip_version = xml.attributes().value("","version").toString();
                    success = success && fGetNTCIPTree(&xml,tree,"");
                }
            }
        }while(!at_end);
    }

    return success;
}

void HNS_NTCIP_XML::fWriteNTCIPFile(NTCIP_Node *tree)
{
    QFile file(f_file);
    QXmlStreamWriter xml;

    if(file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        xml.setDevice(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument("1.0");
        xml.writeStartElement("","NTCIP");
        xml.writeAttribute("","HNS_Version","1.0");
        xml.writeAttribute("","version","9001 04.06");

        fWriteNTCIPBranch(&xml,tree);

        xml.writeEndElement();
        xml.writeEndDocument();
    }
}

bool HNS_NTCIP_XML::fGetNTCIPTree(QXmlStreamReader *xml, NTCIP_Node *tree, const QString &parent_oid)
{
    bool success = true;
    bool at_end = false;
    QString tempstring;
    QString end_tag;
    QString new_oid;
    QString datatype_string,access_string,data_string,binary_mode_string;

    type_ntcip_data_access access;
    bool binary_mode = false;

    if(parent_oid.isEmpty())
    {
        end_tag = "NTCIP";
    }
    else
    {
        end_tag = "Node";
    }

    do
    {
        QXmlStreamReader::TokenType next_token = xml->readNext();
        //found a child
        if(xml->isStartElement() && xml->name() == "Node")
        {
            if(parent_oid.isEmpty())
            {
                new_oid = xml->attributes().value("","OID").toString();
            }
            else
            {
                new_oid = parent_oid + "." + xml->attributes().value("","OID").toString();
            }
            tempstring = xml->attributes().value("","name").toString();
            tree->fAddChild(new_oid.toStdString(),xml->attributes().value("","name").toString().toStdString());
            fGetNTCIPTree(xml,tree,new_oid);
        }
        //found a data bearing leaf
        else if(xml->isStartElement() && xml->name() == "Data")
        {
            datatype_string = xml->attributes().value("","datatype").toString();
            access_string = xml->attributes().value("","access").toString();
            binary_mode_string = xml->attributes().value("","binarymode").toString();

            while(!(xml->isCharacters() || xml->isEndElement()))
            {
                xml->readNext();
            }
            if(xml->isCharacters())
            {
                //found the data payload
                data_string = xml->text().toString();
                if(access_string == "rw")
                {
                    access = HNS_NTCIP_READWRITE;
                }
                else
                {
                    access = HNS_NTCIP_READ;
                }
                binary_mode = (binary_mode_string == "1");
            }
            else
            {
                //data field is blank
                if(datatype_string == "INTEGER")
                {
                    data_string = "0";
                }
                binary_mode = false;
                //for OCTETSTRING, leave data_string blank

                //don't forget access in this case.
                if(access_string == "rw")
                {
                    access = HNS_NTCIP_READWRITE;
                }
                else
                {
                    access = HNS_NTCIP_READ;
                }
            }

            if(datatype_string == "OCTETSTRING")
            {
                tree->fSetData(parent_oid.toStdString(),data_string.toStdString(),access,HNS_NTCIP_OCTETSTRING,binary_mode);
            }
            else if(datatype_string == "INTEGER" || datatype_string == "COUNTER" || datatype_string == "TIMETICKS")
            {
                tree->fSetData(parent_oid.toStdString(),data_string.toInt(),access,HNS_NTCIP_INTEGER);
            }
            else if(datatype_string == "COUNTER")
            {
                tree->fSetData(parent_oid.toStdString(),data_string.toInt(),access,HNS_NTCIP_COUNTER);
            }
            else if(datatype_string == "TIMETICKS")
            {
                tree->fSetData(parent_oid.toStdString(),data_string.toInt(),access,HNS_NTCIP_TIMETICKS);
            }
            else
            {
                tree->fSetData(parent_oid.toStdString(),data_string.toStdString(),access,HNS_NTCIP_OCTETSTRING,binary_mode);
            }

            while(!(xml->isEndElement() && xml->name() == "Data"))
            {
                xml->readNext();
            }
        }
        //look for closing node.  Recursion should take care of children closing tags.
        //if this is no parent oid, then this is the root and we should look for the NTCIP closing tag.
        //if parent oid is not empty, then this is a child
        else if(xml->isEndElement() && xml->name() == end_tag)
        {
            at_end = true;
        }
        else if(next_token == QXmlStreamReader::Invalid)
        {
            //error condition
            at_end = true;
            success = false;
        }
    }while(!at_end);

    return success;
}

void HNS_NTCIP_XML::fWriteNTCIPBranch(QXmlStreamWriter *xml, NTCIP_Node *tree)
{
    xml->writeStartElement("","Node");
    xml->writeAttribute("","name",QString::fromStdString(tree->fGetName()));
    xml->writeAttribute("","OID",QString::number(tree->fGetOidNumber()));
    //found a leaf, check for data
    if(tree->fGetNumChildren() == 0)
    {
        if(tree->fGetDataSize() > 0)
        {
            xml->writeStartElement("","Data");
            if(tree->fGetDataType() == HNS_NTCIP_INTEGER)
            {
                xml->writeAttribute("","datatype","INTEGER");
            }
            else if(tree->fGetDataType() == HNS_NTCIP_COUNTER)
            {
                xml->writeAttribute("","datatype","COUNTER");
            }
            else if(tree->fGetDataType() == HNS_NTCIP_TIMETICKS)
            {
                xml->writeAttribute("","datatype","TIMETICKS");
            }
            else
            {
                xml->writeAttribute("","datatype","OCTETSTRING");
            }

            if(tree->fGetDataAccess() == HNS_NTCIP_READWRITE)
            {
                xml->writeAttribute("","access","rw");
            }
            else
            {
                xml->writeAttribute("","access","r");
            }

            if(tree->fGetBinaryMode())
            {
                xml->writeAttribute("","binarymode","1");
            }

            xml->writeCharacters(QString::fromStdString(tree->fGetRawDataAsString()));

            xml->writeEndElement();
        }
        else
        {
            xml->writeStartElement("","Data");
            if(tree->fGetDataType() == HNS_NTCIP_INTEGER)
            {
                xml->writeAttribute("","datatype","INTEGER");
            }
            else if(tree->fGetDataType() == HNS_NTCIP_COUNTER)
            {
                xml->writeAttribute("","datatype","COUNTER");
            }
            else if(tree->fGetDataType() == HNS_NTCIP_TIMETICKS)
            {
                xml->writeAttribute("","datatype","TIMETICKS");
            }
            else
            {
                xml->writeAttribute("","datatype","OCTETSTRING");
            }

            if(tree->fGetDataAccess() == HNS_NTCIP_READWRITE)
            {
                xml->writeAttribute("","access","rw");
            }
            else
            {
                xml->writeAttribute("","access","rw");
            }

            if(tree->fGetBinaryMode())
            {
                xml->writeAttribute("","binarymode","1");
            }

            xml->writeEndElement();
        }
    }
    //has branches
    else
    {
        for(size_t i=0;i<tree->fGetNumChildren();i++)
        {
            fWriteNTCIPBranch(xml,tree->fGetChild(i));
        }
    }
    xml->writeEndElement();
}
