#ifndef IOIO_H
#define IOIO_H

#include <vector>

enum
{
    IOIO_DIRECTION_UNDEFINED = -1,
    IOIO_DIRECTION_INPUT,
    IOIO_DIRECTION_OUTPUT
};

enum
{
    IOIO_VALUE_UNDEFINED = -1,
    IOIO_VALUE_FALSE,
    IOIO_VALUE_TRUE
};

enum
{
    IOIO_OUTPUT_MODE_UNDEFINED = -1,
    IOIO_OUTPUT_MODE_NORMAL,
    IOIO_OUTPUT_MODE_OPEN_SOURCE
};

class IOIO_Port
{
public:
    IOIO_Port();
    IOIO_Port(const int &port_number = -1, const int &port_direction = IOIO_DIRECTION_UNDEFINED, const int &port_value = IOIO_VALUE_UNDEFINED, const int &port_output_mode = IOIO_OUTPUT_MODE_UNDEFINED);

    int fGetPortNumber() const {return f_port_number;}
    int fGetPortDirection() const {return f_port_direction;}
    int fGetPortValue() const {return f_port_value;}
    int fGetPortOutputMode() const {return f_port_output_mode;}

    void fSetDirection(const int &dir);
    void fSetValue(const int &value);
    void fSetOutputMode(const int &output_mode);

    static bool fIsValueValid(const int &value) {return (value == IOIO_VALUE_FALSE || value == IOIO_VALUE_TRUE);}
    static bool fIsDirectionValid(const int &dir) {return (dir == IOIO_DIRECTION_INPUT || dir == IOIO_DIRECTION_OUTPUT);}
    static bool fIsOutputModeValid(const int &mode) {return (mode == IOIO_OUTPUT_MODE_NORMAL || mode == IOIO_OUTPUT_MODE_OPEN_SOURCE);}
private:
    int f_port_number;
    int f_port_direction;
    int f_port_value;
    int f_port_output_mode;
};

class IOIO
{
public:
    IOIO();

    void fAddPort(const int &pin_number);

    std::vector<unsigned char> fSetDigitalOut(const int &pin_number, const int &start_value = IOIO_VALUE_FALSE, const int &output_mode = IOIO_OUTPUT_MODE_NORMAL);
    std::vector<unsigned char> fSetDigitalIn(const int &pin_number);
    std::vector<unsigned char> fSetValue(const int &pin_number, const int &value);
private:
    int fFindPin(const int &pin_number);

    std::vector<IOIO_Port> f_ports;
};

#endif // IOIO_H
