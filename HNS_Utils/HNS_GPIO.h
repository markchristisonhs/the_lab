#ifndef HNS_GPIO_H
#define HNS_GPIO_H

#include <string>
#include <vector>

typedef enum
{
    HNS_GPIO_DIR_UNDEFINED = -1,
    HNS_GPIO_OUTPUT,
    HNS_GPIO_INPUT
} type_gpio_direction;

class HNS_GPIO_Port
{
public:
    HNS_GPIO_Port();
    HNS_GPIO_Port(const type_gpio_direction &direction, const int &port_num, const int &val);

    type_gpio_direction &fDirection() {return f_direction;}
    type_gpio_direction fGetDirection() const {return f_direction;}
    void fSetDirection(const type_gpio_direction &direction) {f_direction = direction;}

    int &fVal() {return f_val;}
    int fGetVal() const {return f_val;}
    void fSetVal(const int &val) {f_val = val;}

    int &fPortNum() {return f_port_num;}
    int fGetPortNum() const {return f_port_num;}
    void fSetPortNum(const int &port_num){f_port_num = port_num;}

    bool operator ==(const HNS_GPIO_Port &rhs) const;
    bool operator !=(const HNS_GPIO_Port &rhs) const;
private:
    type_gpio_direction f_direction;
    int f_val;
    int f_port_num;
};

class HNS_GPIO_Interface
{
public:
    virtual HNS_GPIO_Port fGetGPIO_Port(const int &port) = 0;
    virtual std::vector<HNS_GPIO_Port> fGetGPIO_Ports() = 0;
    virtual int fGetGPIO_Input(const int &port) = 0;
    virtual void fSetGPIO_Output(const int &port, const int &val) = 0;
    virtual ~HNS_GPIO_Interface() {}
};

#endif // HNS_GPIO_H
