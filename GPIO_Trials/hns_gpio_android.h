#ifndef HNS_GPIO_ANDROID_H
#define HNS_GPIO_ANDROID_H

#include <HNS_GPIO.h>
#include "hns_gpio_service_interface.h"
#include <QVector>
#include <QObject>


class HNS_GPIO_Android : public QObject, public HNS_GPIO_Interface, public HNS_GPIO_Service_Interface
{
    Q_OBJECT
public:
    HNS_GPIO_Android(QObject *parent = nullptr);

    void fRetrieveNewInput(const int &port, const int &val);
    HNS_GPIO_Port fGetGPIO_Port(const int &port);
    std::vector<HNS_GPIO_Port> fGetGPIO_Ports();
    int fGetGPIO_Input(const int &port);
    void fSetGPIO_Output(const int &port, const int &val);
private:
    void fStartService();

    QVector<HNS_GPIO_Port> f_ports;
signals:
    void SIG_Input_Changed();
};

#endif // HNS_GPIO_ANDROID_H
