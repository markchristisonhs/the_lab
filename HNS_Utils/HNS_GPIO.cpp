#include "HNS_GPIO.h"

#include <sstream>

using namespace std;

HNS_GPIO_Port::HNS_GPIO_Port() :
    f_direction(HNS_GPIO_DIR_UNDEFINED)
  , f_val(0)
  , f_port_num(0)
{

}

HNS_GPIO_Port::HNS_GPIO_Port(const type_gpio_direction &direction, const int &port_num, const int &val):
    f_direction(direction)
  , f_val(val)
  , f_port_num(port_num)
{

}

bool HNS_GPIO_Port::operator ==(const HNS_GPIO_Port &rhs) const
{
    bool result = false;

    result = (f_direction == rhs.fGetDirection())
            && (f_val == rhs.fGetVal())
            && (f_port_num == rhs.fGetPortNum());

    return result;
}

bool HNS_GPIO_Port::operator !=(const HNS_GPIO_Port &rhs) const
{
    return !(*this == rhs);
}
