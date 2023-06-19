#include "hns_logaline_qt_buffer.h"
#include <QDateTime>
#include <QTextStream>

HNS_LogALine_QT_Buffer::HNS_LogALine_QT_Buffer(QObject *parent) : QObject(parent),
  f_file_index(1),
  f_timestamp(false)
{

}

void HNS_LogALine_QT_Buffer::fLogALine(const std::string &line)
{
    QFile *output_file = fGetOutputFile();
    QTextStream output_stream(output_file);
    QDateTime current = QDateTime::currentDateTime();
    QString formatted_line = (f_timestamp ? (current.toString("yyyy.MM.dd hh:mm:ss.zzz") + " : ") : "") + QString::fromStdString(line);

    if(output_file->isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    output_file->close();

    delete output_file;
    output_file = nullptr;

    emit fNewLine(formatted_line);
}

void HNS_LogALine_QT_Buffer::fLogALine(const std::string &line, const std::string &caller)
{
    QFile *output_file = fGetOutputFile();
    QDateTime current = QDateTime::currentDateTime();
    QTextStream output_stream(output_file);

    QString formatted_line = (f_timestamp ? (current.toString("yyyy.MM.dd hh:mm:ss.zzz") + " : ") : "") + QString("%1 : %2").arg(QString::fromStdString(caller)).arg(QString::fromStdString(line));

    if(output_file->isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    output_file->close();

    delete output_file;
    output_file = nullptr;

    emit fNewLine(formatted_line);
}

void HNS_LogALine_QT_Buffer::fLogBuffer(const std::vector<unsigned char> &buffer)
{
    QFile *output_file = fGetOutputFile();
    QTextStream output_stream(output_file);
    QString formatted_line;

    for(size_t ui=0;ui<buffer.size();ui++)
    {
        formatted_line += QString::number(buffer[ui],16).rightJustified(2,'0');
        if((ui % 16) != 15)
        {
            formatted_line += " ";
        }
        else
        {
            if(output_file->isOpen())
            {
                output_stream << formatted_line << "\n";
            }
            emit fNewLine(formatted_line);
            formatted_line.clear();
        }
    }
    if(!formatted_line.isEmpty())
    {
        if(output_file->isOpen())
        {
            output_stream << formatted_line << "\n";
        }
        emit fNewLine(formatted_line);
        formatted_line.clear();
    }

    output_file->close();

    delete output_file;
    output_file = nullptr;
}

void HNS_LogALine_QT_Buffer::fLogCurrentTime()
{
    QFile *output_file = fGetOutputFile();
    QTextStream output_stream(output_file);
    QDateTime current = QDateTime::currentDateTime();
    QString formatted_line = current.toString("yyyy.MM.dd hh:mm:ss.zzz");

    if(output_file->isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    emit fNewLine(formatted_line);

    output_file->close();

    delete output_file;
    output_file = nullptr;
}

HNS_LogALine_QT_Buffer::~HNS_LogALine_QT_Buffer()
{

}

void HNS_LogALine_QT_Buffer::fSetLogfileLocation(const QString &path)
{
    f_file_index = 1;
    f_file_path = path;
    f_current_file_path = f_file_path + QString(".%1").arg(f_file_index,2,10,QLatin1Char('0'));
}

void HNS_LogALine_QT_Buffer::fSetTimestamp(const bool &timestamp)
{
    f_timestamp = timestamp;
}

QFile *HNS_LogALine_QT_Buffer::fGetOutputFile()
{
    QFile *file = new QFile(f_current_file_path);

    if(file->size() > 1048575)
    {
        f_file_index++;
        if(f_file_index > 5)
        {
            f_file_index = 1;
        }
        fSetLogfileLocation(f_file_path);
        file->setFileName(f_current_file_path);
        file->open(QIODevice::WriteOnly);
    }
    else
    {
        file->open(QIODevice::WriteOnly|QIODevice::Append);
    }

    return file;
}
