#include "hns_messages_xml.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QTime>
#include <sstream>
#include <hns_qt_toolkit.h>
#include <QFileInfo>
#include <QDir>

using namespace std;

bool IsScheduleVersionValid(const double &version)
{
    return version == 1.0 || version == 1.1;
}

HNS_Messages_XML::HNS_Messages_XML()
{

}

HNS_Messages_XML::HNS_Messages_XML(const QString &file)
{
    f_file = file;
}

void HNS_Messages_XML::fSetFile(const QString &file)
{
    f_file = file;
}

vector<string> HNS_Messages_XML::fGetMessagesFromFile()
{
    QStringRef tempstring_ref;
    vector <string> messages;
    QFile file(f_file);
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    bool at_end = false;
    QXmlStreamAttributes test;
    bool message_tag_open = false;

    if(file.open(QIODevice::ReadWrite))
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
                if(xml.name() == "HNS_Message")
                {
                    test = xml.attributes();
                    if(test.value("","version").toDouble() != 1.0)
                    {
                        at_end = true;
                    }
                }
                else if(xml.name() == "Message")
                {
                    message_tag_open = true;
                }
            }
            else if(token == QXmlStreamReader::EndElement)
            {
                if(xml.name() == "Message")
                {
                    message_tag_open = false;
                }
            }
            else if(token == QXmlStreamReader::Characters)
            {
                if(message_tag_open)
                {
                    tempstring_ref = xml.text();
                    messages.push_back(tempstring_ref.toString().toStdString());
                }
            }
        }while(!at_end);
    }

    return messages;
}

QVector<QVector<int> > LoadAnimationXML(const QString &path)
{
    QStringRef tempstring_ref;
    QVector<QVector<int> > animations;
    QVector<int> animation;
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    QFile file(path);
    bool at_end = false;
    QXmlStreamAttributes test;

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
                if(xml.name() == "HNS_Animation")
                {
                    test = xml.attributes();
                    if(test.value("","version").toDouble() != 1.0)
                    {
                        at_end = true;
                    }
                }
                else if(xml.name() == "Animation")
                {
                    //start of animation tag
                }
                else if(xml.name() == "Page")
                {
                    animation.push_back(xml.attributes().value("","graphic").toInt());
                }
                else
                {
                    at_end = true;
                }
            }
            else if(token == QXmlStreamReader::EndElement)
            {
                if(xml.name() == "Animation")
                {
                    animations.push_back(animation);
                    animation.clear();
                }
            }
        }while(!at_end);
    }

    return animations;
}

QVector<HNS_Schedule> LoadSchedulesXML(const QString &path, const std::vector<HNS_Font> *fonts, const std::vector<HNS_Graphic> *graphics, const HNS_Field_Data *field_data, const int &max_num_pages)
{
    QVector<HNS_Schedule> result;
    QFile file(path);
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    bool at_end = false;
    QXmlStreamAttributes test;
    QTime start,stop;
    QDate date_start,date_stop;
//    type_message_type message_type;
//    int message_no;
    HNS_Schedule schedule;
    QString tempstring;
    QStringRef tempstring_ref;
    int i;
    string multi_string;
    HNS_Message2 temp_message;
    double version;
    //string caller = "";
    //ostringstream s;

    if(file.open(QIODevice::ReadOnly))
    {
        //s << "Schedule file " << path.toStdString() << " opened";
        //gLog->fLogALine(s.str(),caller);
        //s.str("");
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

            }
            else if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "HNS_Schedule")
                {
                    test = xml.attributes();
                    version = 0.0;
                    version = test.value("","version").toDouble();
                    if(!IsScheduleVersionValid(version))
                    {
                        at_end = true;
                    }
                }
                if(xml.name() == "Schedule")
                {
                    schedule.fSetActive(xml.attributes().value("Active").toInt() == 1);
                    tempstring_ref = xml.attributes().value("StartDate");
                    tempstring = tempstring_ref.toString();
                    //date_start = QDate::fromString(*xml.attributes().value("StartDate").string(),"yy.M.d");
                    date_start = QDate::fromString(tempstring,"yyyy.M.d");
                    tempstring_ref = xml.attributes().value("StopDate");
                    tempstring = tempstring_ref.toString();
                    date_stop = QDate::fromString(tempstring,"yyyy.M.d");
                    schedule.fSetStartDate(ConvertQDatetoSTLDate(date_start));
                    schedule.fSetEndDate(ConvertQDatetoSTLDate(date_stop));
                    tempstring_ref = xml.attributes().value("Week");
                    tempstring = tempstring_ref.string()->mid(tempstring_ref.position(),tempstring_ref.size());
                    for(i=0;i<tempstring.size();i++)
                    {
                        switch(tempstring.at(i).toLatin1())
                        {
                        case 'U':
                            schedule.fSetDay(HNS_SCHED_SUNDAY);
                            break;
                        case 'M':
                            schedule.fSetDay(HNS_SCHED_MONDAY);
                            break;
                        case 'T':
                            schedule.fSetDay(HNS_SCHED_TUESDAY);
                            break;
                        case 'W':
                            schedule.fSetDay(HNS_SCHED_WEDNESDAY);
                            break;
                        case 'R':
                            schedule.fSetDay(HNS_SCHED_THURSDAY);
                            break;
                        case 'F':
                            schedule.fSetDay(HNS_SCHED_FRIDAY);
                            break;
                        case 'S':
                            schedule.fSetDay(HNS_SCHED_SATURDAY);
                            break;
                        }
                    }

                    tempstring_ref = xml.attributes().value("Start");
                    tempstring = tempstring_ref.string()->mid(tempstring_ref.position(),tempstring_ref.size());
                    //s << "Found start time: " << tempstring.toStdString();
                    //gLog->fLogALine(s.str(),caller);
                    //s.str("");
                    start = QTime::fromString(tempstring,"H:mm:ss");

                    tempstring_ref = xml.attributes().value("Stop");
                    tempstring = tempstring_ref.string()->mid(tempstring_ref.position(),tempstring_ref.size());
                    //s << "Found end time: " << tempstring.toStdString();
                    //gLog->fLogALine(s.str(),caller);
                    //s.str("");
                    stop = QTime::fromString(tempstring,"H:mm:ss");

                    schedule.fSetStartTime(ConvertQTimetoSTLTime(start));
                    schedule.fSetStopTime(ConvertQTimetoSTLTime(stop));
                    multi_string = xml.attributes().value("Message").toString().toStdString();
                    temp_message.fSetMULTI(multi_string,fonts,graphics,field_data,nullptr,nullptr,max_num_pages);
                    schedule.fSetMessage(temp_message);

                    tempstring = "";
                    tempstring = xml.attributes().value("Title").toString();
                    schedule.fSetTitle(tempstring.toStdString());

                    if(version == 1.1)
                    {
                        tempstring = xml.attributes().value("StartNow").toString();
                        schedule.fSetStartNow(tempstring == "TRUE");

                        tempstring = xml.attributes().value("NeverEnd").toString();
                        schedule.fSetNeverEnd(tempstring == "TRUE");
                    }

                    result.push_back(schedule);
                }
            }
            else if(token == QXmlStreamReader::EndElement)
            {
            }
            else if(token == QXmlStreamReader::Characters)
            {
            }
        }while(!at_end);
    }

    //s << "Loading schedules done.  There are " << result.size() << " schedules present";
    //gLog->fLogALine(s.str(),caller);
    //s.str("");

    return result;
}

void SaveSchedulesXML(const QString &path, const QVector<HNS_Schedule> &schedules)
{
    int i,j;
    QXmlStreamWriter xml;
    QFile file(path);
    QTime start,stop;
    QDate date_start,date_stop;
    QString tempstring;
    HNS_Message2 temp_message;

    if(file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        xml.setAutoFormatting(true);
        xml.setDevice(&file);

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE xml>");
        xml.writeStartElement(QStringLiteral("HNS_Schedule"));
        xml.writeAttribute(QStringLiteral("version"),QStringLiteral("1.1"));
    }

    for(i=0;i<schedules.size();i++)
    {
        date_start = ConvertSTLDatetoQDate(schedules[i].fGetStartDate());
        date_stop = ConvertSTLDatetoQDate(schedules[i].fGetEndDate());
        start = ConvertSTLTimetoQTime(schedules[i].fGetStartTime());
        stop = ConvertSTLTimetoQTime(schedules[i].fGetStopTime());
        schedules[i].fGetMessage(temp_message);
        xml.writeStartElement("Schedule");
        xml.writeAttribute("ID",QString("%1").arg(i));
        xml.writeAttribute("Active",QString("%1").arg(schedules[i].fIsActive()));
        xml.writeAttribute("StartDate",date_start.toString("yyyy.M.d"));
        xml.writeAttribute("StopDate",date_stop.toString("yyyy.M.d"));
        xml.writeAttribute("Title",QString::fromStdString(schedules[i].fGetTitle()));
        tempstring = "";
        for(j=0;j<7;j++)
        {
            if(schedules[i].fGetDay(j))
            {
                tempstring += schedules[i].fGetDayChar(j);
            }
        }
        xml.writeAttribute("Week",tempstring);
        xml.writeAttribute("Start",start.toString("H:mm:ss"));
        xml.writeAttribute("Stop",stop.toString("H:mm:ss"));
        xml.writeAttribute("Message",QString::fromStdString(temp_message.fGetMULTI()));
        xml.writeAttribute("StartNow",schedules[i].fStartNow() ? "TRUE" : "FALSE");
        xml.writeAttribute("NeverEnd",schedules[i].fNeverEnd() ? "TRUE" : "FALSE");
//        xml.writeAttribute("MessageType",QString::number(message_type));
//        xml.writeAttribute("MessageNo",QString::number(message_no));
        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();
}

vector<HNS_Message_Info> LoadMessageLogXML(const QString &path)
{
    vector<HNS_Message_Info> result, temp_vec;
    vector<size_t> temp_vec_positions;

    QFile file(path);
    QXmlStreamReader xml;
    QXmlStreamReader::TokenType token;
    bool at_end = false;
    QXmlStreamAttributes test;
    HNS_Message_Info entry;
    double version;
    size_t size_temp;

    size_t index;
    bool num_messages_found = false;

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

            }
            else if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "HNS_Message_Log")
                {
                    test = xml.attributes();
                    version = 0.0;
                    version = test.value("","version").toDouble();
                    if(version != 1.0)
                    {
                        at_end = true;
                    }
                }
                else if(xml.name() == "Num_Messages")
                {
                    num_messages_found = true;

                    size_temp = 0;

                    result.resize(xml.attributes().value("size").toUInt());

                    //Num messages should have been first and this list should be empty,
                    //but if not, copy over the temporary, possibly unordered, list
                    if(!temp_vec.empty())
                    {
                        for(size_t ui=0;ui<temp_vec.size();ui++)
                        {
                            //should always be true since the vectors were created together.  but just in case.
                            if(ui<temp_vec_positions.size())
                            {
                                result[temp_vec_positions[ui]] = temp_vec[ui];
                            }
                        }
                    }
                }
                else if(xml.name() == "Message")
                {
                    index = xml.attributes().value("ID").toUInt();
                    entry.fSetTime(xml.attributes().value("Time").toLongLong());

                    //Ideally, this should have been the first element
                    if(num_messages_found)
                    {
                        if(index < result.size())
                        {
                            result[index] = entry;
                        }
                    }
                    //Oh no!  Num messages did not come first.  Make a temporary list
                    else
                    {
                        temp_vec.push_back(entry);
                        temp_vec_positions.push_back(index);
                    }
                }
            }
            else if(token == QXmlStreamReader::EndElement)
            {
            }
            else if(token == QXmlStreamReader::Characters)
            {
            }
        }while(!at_end);
    }

    //s << "Loading schedules done.  There are " << result.size() << " schedules present";
    //gLog->fLogALine(s.str(),caller);
    //s.str("");

    return result;
}

void SaveMessageLogXML(const QString &path, const vector<HNS_Message_Info> &message_log)
{
    size_t ui;
    QXmlStreamWriter xml;
    QFile file(path);
    QString tempstring = path.left(path.lastIndexOf(QDir::separator()));

    QDir dir(tempstring);
    if(!dir.exists(tempstring))
    {
        dir.mkdir(tempstring);
    }

    if(file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        xml.setAutoFormatting(true);
        xml.setDevice(&file);

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE xml>");
        xml.writeStartElement(QStringLiteral("HNS_Message_Log"));
        xml.writeAttribute(QStringLiteral("version"),QStringLiteral("1.0"));
    }

    xml.writeStartElement("Num_Messages");
    xml.writeAttribute("size",QString("%1").arg(message_log.size()));
    xml.writeEndElement();

    for(ui=0;ui<message_log.size();ui++)
    {
        xml.writeStartElement("Message");
        xml.writeAttribute("ID",QString("%1").arg(ui));
        xml.writeAttribute("Time",QString("%1").arg(message_log[ui].fGetTime()));
        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();
}
