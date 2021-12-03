#include "loadntcip_worker.h"
#include <QFileInfo>
#include <QDir>
#include <hns_ntcip_xml.h>

LoadNTCIP_Worker::LoadNTCIP_Worker()
{

}

NTCIP_Node LoadNTCIP_Worker::fGetResult()
{
    return f_ntcip;
}

void LoadNTCIP_Worker::doWork(const QString &file_path, const QString &backup_resource)
{
    QString tempstring;
    QString load_path = file_path;
    bool generate_file = false;

    QFileInfo fi(file_path);
    if(!QFileInfo::exists(file_path) || (fi.size() == 0))
    {
        load_path = backup_resource;
        generate_file = true;
    }

    HNS_NTCIP_XML xml_file(load_path);

    xml_file.fParseNTCIPFile(&f_ntcip);

    if(generate_file)
    {
        tempstring = file_path.left(file_path.lastIndexOf(QDir::separator())-1);
        if(!QFileInfo::exists(tempstring))
        {
            QDir dir = QDir(tempstring);
            dir.mkpath(tempstring);
        }
        xml_file = HNS_NTCIP_XML(file_path);
        xml_file.fWriteNTCIPFile(&f_ntcip);
    }

    emit resultReady();
}
