#ifndef HNS_QT_SERIAL_H
#define HNS_QT_SERIAL_H

#include <serial_info.h>
#include <HNS_CommLink.h>
#include <string>
#include <QSerialPort>

class HNS_QT_SerialPort : public QObject, public HNS_CommLink
{
    Q_OBJECT
public:
    HNS_QT_SerialPort();
    HNS_QT_SerialPort(const std::string &device, const type_hns_baud_rate &baud = HNS_SER_BAUD9600);

    ~HNS_QT_SerialPort();

    int fOpen(const std::string &device, const type_hns_baud_rate &baud = HNS_SER_BAUD9600);
    bool fIsOpened();

    bool fWrite(const unsigned char *data, const size_t &datasize);
    bool fRead(unsigned char *data, const size_t &datasize, ssize_t &received);

    int fGetError();
private:
    QSerialPort *f_port;

    int f_last_error;
    QByteArray f_buffer;

private slots:

    void fReadData();
};

#endif // HNS_QT_SERIAL_H
