#include "LocalHostLink.h"
#include <string>

using namespace std;

LocalHostLink::LocalHostLink(QObject *parent):
    QObject(parent),
    socket(new QLocalSocket(this)),
    f_logaline(nullptr)
{
    int tries = 5;

    connect(socket,SIGNAL(readyRead()),this,SLOT(DataReady()));

    while(socket->state() != QLocalSocket::ConnectedState && tries > 0)
    {
        socket->abort();
        socket->connectToServer("Trailer");
        tries--;
    }
}

void LocalHostLink::fSetLog(HNS_LogALine2 *logaline)
{
    f_logaline = logaline;
}

bool LocalHostLink::fIsOpened()
{
    return socket->isValid();
}

bool LocalHostLink::fWrite(const unsigned char *data, const size_t &datasize)
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
        socket->write(block);
        socket->flush();
        return true;
    }
    return false;
}

bool LocalHostLink::fRead(unsigned char *data, const size_t &datasize, ssize_t &received)
{
    string caller = "LocalHostLink";
    QString s;

    s = QString("Running read, looking for %1 bytes").arg(datasize);
    if(f_logaline != nullptr) f_logaline->fLogALine(s.toStdString(),caller);
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

int LocalHostLink::fGetError()
{
    return 0;
}

LocalHostLink::~LocalHostLink()
{

}

void LocalHostLink::DataReady()
{
    QDataStream in;
    quint8 tempchar;
    int blockSize = 0;
    QByteArray temparray;
    string caller = "LocalHostLine::DataReady";
    vector<unsigned char> temp_vec;
    QString s;

    if(f_logaline != nullptr) f_logaline->fLogCurrentTime();
    if(f_logaline != nullptr) f_logaline->fLogALine("Receiving Data",caller);

    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_5_13);

    if(socket->bytesAvailable() < (int)sizeof(quint32))
        return;

    in >> blockSize;

    if (socket->bytesAvailable() < blockSize || in.atEnd())
        return;

    for(int i=0;i<blockSize;i++)
    {
        in >> tempchar;
        temparray.append(tempchar);
    }

    f_buffer.append(temparray);
    if(f_logaline != nullptr)
    {
        temp_vec = vector<unsigned char> (temparray.begin(),temparray.end());
        f_logaline->fLogBuffer(temp_vec);

        s = QString("Buffer is now %1 bytes").arg(f_buffer.size());
        f_logaline->fLogALine(s.toStdString(),caller);
    }
}
