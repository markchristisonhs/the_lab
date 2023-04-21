#ifndef LOGALINE_QT_H
#define LOGALINE_QT_H

#include <logaline.h>
#include <QList>
#include <QString>
#include <QObject>
#include <QFile>

class HNS_LogALine_QT : public QObject, public HNS_LogALine2
{
    Q_OBJECT
public:
    HNS_LogALine_QT();
    void fLogALine(const std::string &line);
    void fLogALine(const std::string &line, const std::string &caller);
    void fLogBuffer(const std::vector<unsigned char> &buffer);
    void fLogCurrentTime();
    ~HNS_LogALine_QT();

    void fSetLogfileLocation(const QString &path);
    void fSetTimestamp(const bool &timestamp);
private:
    QList <QString> f_lines;
    QString f_file_path;
    //if true, prepend an ISO standard time stamp
    bool f_timestamp;
signals:
    void fNewLine(QString line);
};

#endif // LOGALINE_QT_H
