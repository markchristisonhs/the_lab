#include "HNS_QT_Serial.h"
#include <QCoreApplication>
#include <QDataStream>

using namespace std;

HNS_QT_SerialPort::HNS_QT_SerialPort():
    f_port(nullptr)
  , f_last_error(0)
{

}

HNS_QT_SerialPort::HNS_QT_SerialPort(const string &device, const type_hns_baud_rate &baud):
    f_port(nullptr)
  , f_last_error(0)
{
    fOpen(device,baud);
}

HNS_QT_SerialPort::~HNS_QT_SerialPort()
{
    delete f_port;
    f_port = nullptr;
}

int HNS_QT_SerialPort::fOpen(const string &device, const type_hns_baud_rate &baud)
{
    int error = 0;

    f_port = new QSerialPort(this);
    f_port->setPortName(QString::fromStdString(device));
    if(baud == HNS_SER_BAUD9600)
    {
        f_port->setBaudRate(QSerialPort::Baud9600);
    }
    else if(baud == HNS_SER_BAUD115200)
    {
        f_port->setBaudRate(QSerialPort::Baud115200);
    }
    else
    {
        f_port->setBaudRate(QSerialPort::Baud9600);
    }
    f_port->setDataBits(QSerialPort::Data8);
    f_port->setParity(QSerialPort::NoParity);
    f_port->setStopBits(QSerialPort::OneStop);
    f_port->setFlowControl(QSerialPort::NoFlowControl);
    if (f_port->open(QIODevice::ReadWrite))
    {
        connect(f_port, &QSerialPort::readyRead, this, &HNS_QT_SerialPort::fReadData);
    }
    else
    {
        delete f_port;
        f_port = nullptr;
        error = 1;
    }

    return error;
}

bool HNS_QT_SerialPort::HNS_QT_SerialPort::fIsOpened()
{
    return (f_port != nullptr);
}

bool HNS_QT_SerialPort::fWrite(const unsigned char *data, const size_t &datasize)
{
    if(fIsOpened())
    {
        QByteArray block;
        int packet_size;
        for(size_t i = 0;i<datasize;i++)
        {
            block.append(data[i]);
        }

        packet_size = block.size();
        for(size_t i=0;i<sizeof(packet_size);i++)
        {
            block.prepend( static_cast<unsigned char>(packet_size & 0xFF));
            packet_size = packet_size >> 8;
        }
        f_port->write(block);
        return true;
    }
    return false;
}

bool HNS_QT_SerialPort::fRead(unsigned char *data, const size_t &datasize, ssize_t &received)
{
    if(fIsOpened())
    {
        QCoreApplication::processEvents();

        size_t index = 0;
        while(f_buffer.size() > 0)
        {
            if(index < datasize)
            {
                data[index++] = f_buffer.front();
                f_buffer.remove(0,1);
            }
            else
            {
                break;
            }
        }
        received = index;
        return true;
    }
    return false;
}

int HNS_QT_SerialPort::fGetError()
{
    return 0;
}

void HNS_QT_SerialPort::fReadData()
{
    QDataStream in;
    quint8 tempchar;
    int blockSize = 0;
    QByteArray temparray;

    in.setDevice(f_port);
    in.setVersion(QDataStream::Qt_5_13);

    blockSize = f_port->bytesAvailable();

    if (f_port->bytesAvailable() < blockSize || in.atEnd())
        return;

    for(int i=0;i<blockSize;i++)
    {
        in >> tempchar;
        temparray.append(tempchar);
    }

    f_buffer.append(temparray);
}
