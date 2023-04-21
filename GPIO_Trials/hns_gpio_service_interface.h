#ifndef HNS_GPIO_SERVICE_INTERFACE_H
#define HNS_GPIO_SERVICE_INTERFACE_H

#include <string>

class HNS_GPIO_Service_Interface
{
public:
    virtual void fRetrieveNewInput(const int &port, const int &val) = 0;
    virtual ~HNS_GPIO_Service_Interface() {}
};

#endif // HNS_GPIO_SERVICE_INTERFACE_H
