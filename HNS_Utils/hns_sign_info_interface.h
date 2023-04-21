#ifndef HNS_SIGN_INFO_INTERFACE_H
#define HNS_SIGN_INFO_INTERFACE_H

#include <MessageBoard.h>

class HNS_Sign_Info_Interface
{
public:
    virtual HNS_SignBoard_Info fGetSignBrdInfo() = 0;
};

#endif // HNS_SIGN_INFO_INTERFACE_H
