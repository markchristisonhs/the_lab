#include "logaline_QT.h"
#include <QDateTime>
#include <QTextStream>

HNS_LogALine_QT::HNS_LogALine_QT():
    f_timestamp(false)
{

}

void HNS_LogALine_QT::fLogALine(const std::string &line)
{
    QFile output_file(f_file_path);
    output_file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream output_stream(&output_file);
    QDateTime current = QDateTime::currentDateTime();
    QString formatted_line = (f_timestamp ? (current.toString("yyyy.MM.dd hh:mm:ss.zzz") + " : ") : "") + QString::fromStdString(line);

    if(output_file.isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    output_file.close();

    emit fNewLine(formatted_line);
}

void HNS_LogALine_QT::fLogALine(const std::string &line, const std::string &caller)
{
    QFile output_file(f_file_path);
    output_file.open(QIODevice::WriteOnly|QIODevice::Append);
    QDateTime current = QDateTime::currentDateTime();
    QTextStream output_stream(&output_file);

    QString formatted_line = (f_timestamp ? (current.toString("yyyy.MM.dd hh:mm:ss.zzz") + " : ") : "") + QString("%1 : %2").arg(QString::fromStdString(caller)).arg(QString::fromStdString(line));

    if(output_file.isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    output_file.close();

    emit fNewLine(formatted_line);
}

void HNS_LogALine_QT::fLogBuffer(const std::vector<unsigned char> &buffer)
{
    QFile output_file(f_file_path);
    output_file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream output_stream(&output_file);
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
            if(output_file.isOpen())
            {
                output_stream << formatted_line << "\n";
            }
            emit fNewLine(formatted_line);
            formatted_line.clear();
        }
    }
    if(!formatted_line.isEmpty())
    {
        if(output_file.isOpen())
        {
            output_stream << formatted_line << "\n";
        }
        emit fNewLine(formatted_line);
        formatted_line.clear();
    }

    output_file.close();
}

void HNS_LogALine_QT::fLogCurrentTime()
{
    QFile output_file(f_file_path);
    output_file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream output_stream(&output_file);
    QDateTime current = QDateTime::currentDateTime();
    QString formatted_line = current.toString("yyyy.MM.dd hh:mm:ss.zzz");

    if(output_file.isOpen())
    {
        output_stream << formatted_line << "\n";
    }
    emit fNewLine(formatted_line);

    output_file.close();
}

HNS_LogALine_QT::~HNS_LogALine_QT()
{

}

void HNS_LogALine_QT::fSetLogfileLocation(const QString &path)
{
    f_file_path = path;
}

void HNS_LogALine_QT::fSetTimestamp(const bool &timestamp)
{
    f_timestamp = timestamp;
}
