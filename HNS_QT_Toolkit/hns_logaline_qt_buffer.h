#ifndef HNS_LOGALINE_QT_BUFFER_H
#define HNS_LOGALINE_QT_BUFFER_H

#include <QObject>
#include <logaline.h>
#include <QFile>
#include <QList>
#include <QString>

class HNS_LogALine_QT_Buffer : public QObject, public HNS_LogALine2
{
    Q_OBJECT
public:
    explicit HNS_LogALine_QT_Buffer(QObject *parent = nullptr);
    void fLogALine(const std::string &line);
    void fLogALine(const std::string &line, const std::string &caller);
    void fLogBuffer(const std::vector<unsigned char> &buffer);
    void fLogCurrentTime();
    ~HNS_LogALine_QT_Buffer();

    void fSetLogfileLocation(const QString &path);
    void fSetTimestamp(const bool &timestamp);
private:
    QFile *fGetOutputFile();

    QList <QString> f_lines;
    QString f_file_path, f_current_file_path;
    int f_file_index;
    //if true, prepend an ISO standard time stamp
    bool f_timestamp;
signals:
    void fNewLine(QString line);

};

#endif // HNS_LOGALINE_QT_BUFFER_H
