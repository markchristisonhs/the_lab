#ifndef LOCALHOSTLINK_H
#define LOCALHOSTLINK_H

#include <HNS_CommLink.h>
#include <QtNetwork>
#include <logaline.h>

class LocalHostLink : public QObject, public HNS_CommLink
{
    Q_OBJECT
public:
    LocalHostLink(QObject *parent = nullptr);
    void fSetLog(HNS_LogALine2 *logaline);

    bool fIsOpened();
    bool fWrite(const unsigned char *data, const size_t &datasize);
    bool fRead(unsigned char *data, const size_t &datasize, ssize_t &received);
    int fGetError();

    ~LocalHostLink();
private:
    QLocalSocket *socket;
    QByteArray f_buffer;

    HNS_LogALine2 *f_logaline;

private slots:

    void DataReady();
};

#endif // LOCALHOSTLINK_H
