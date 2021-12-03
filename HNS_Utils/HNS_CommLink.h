#ifndef HNS_COMMLINK_H
#define HNS_COMMLINK_H

#include <sys/types.h>
#include <stddef.h>

class HNS_CommLink
{
public:
    virtual bool fIsOpened() = 0;
    virtual bool fWrite(const unsigned char *data, const size_t &datasize) = 0;
    virtual bool fRead(unsigned char *data, const size_t &datasize, ssize_t &received) = 0;
    virtual int fGetError() = 0;

    virtual ~HNS_CommLink() = 0;
};

#endif // HNS_COMMLINK_H
