#ifndef HNS_MESSAGES_XML_H
#define HNS_MESSAGES_XML_H

#include <QVector>
#include <QString>
#include <string>
#include <vector>
#include <HNS_schedule.h>
#include <hns_message_info.h>

class HNS_Messages_XML
{
public:
    HNS_Messages_XML();
    HNS_Messages_XML(const QString &file);

    void fSetFile(const QString &file);

    std::vector<std::string> fGetMessagesFromFile();
private:
    QString f_file;
};

QVector<QVector<int> > LoadAnimationXML(const QString &path);
QVector<HNS_Schedule> LoadSchedulesXML(const QString &path, const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const HNS_Field_Data *field_data, const int &max_num_pages = 3);
void SaveSchedulesXML(const QString &path, const QVector<HNS_Schedule> &schedules);

std::vector<HNS_Message_Info> LoadMessageLogXML(const QString &path);
void SaveMessageLogXML(const QString &path, const std::vector<HNS_Message_Info> &message_log);

#endif // HNS_MESSAGES_XML_H
