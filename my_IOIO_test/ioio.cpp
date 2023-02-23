#include "ioio.h"

using namespace std;

IOIO_Port::IOIO_Port():
    f_port_number(-1)
  , f_port_direction(IOIO_DIRECTION_UNDEFINED)
  , f_port_value(IOIO_VALUE_UNDEFINED)
  , f_port_output_mode(IOIO_OUTPUT_MODE_UNDEFINED)
{

}

IOIO_Port::IOIO_Port(const int &port_number, const int &port_direction, const int &port_value, const int &port_output_mode):
    f_port_number(port_number)
  , f_port_direction(port_direction)
  , f_port_value(port_value)
  , f_port_output_mode(port_output_mode)
{
    //Output mode is not defined if a port is declared to be an input.
    if(f_port_direction == IOIO_DIRECTION_INPUT)
    {
        f_port_output_mode = IOIO_OUTPUT_MODE_UNDEFINED;
    }
}

void IOIO_Port::fSetDirection(const int &dir)
{
    if(dir == IOIO_DIRECTION_INPUT || dir == IOIO_DIRECTION_OUTPUT)
    {
        f_port_direction = dir;
    }
    else
    {
        f_port_direction = IOIO_DIRECTION_UNDEFINED;
    }
}

void IOIO_Port::fSetValue(const int &value)
{
    if(value == IOIO_VALUE_FALSE || value == IOIO_VALUE_TRUE)
    {
        f_port_value = value;
    }
    else
    {
        f_port_value = IOIO_VALUE_UNDEFINED;
    }
}

void IOIO_Port::fSetOutputMode(const int &output_mode)
{
    if(output_mode == IOIO_OUTPUT_MODE_NORMAL || output_mode == IOIO_OUTPUT_MODE_OPEN_SOURCE)
    {
        f_port_output_mode = output_mode;
    }
    else
    {
        f_port_output_mode = IOIO_OUTPUT_MODE_UNDEFINED;
    }
}

IOIO::IOIO()
{

}

void IOIO::fConnectComms(const std::function<void(void *data, const int &size)> send_data)
{
    f_send_data = send_data;
}

void IOIO::fAddPort(const int &pin_number)
{
    if(fFindPin(pin_number) == -1)
    {
        f_ports.push_back(IOIO_Port(pin_number));
    }
}

vector<unsigned char> IOIO::fSetDigitalOut(const int &pin_number, const int &start_value, const int &output_mode)
{
    vector<unsigned char> buffer_to_send;
    unsigned char byte = 0;

    int index = fFindPin(pin_number);

    if(index >= 0 && IOIO_Port::fIsValueValid(start_value) && IOIO_Port::fIsOutputModeValid(output_mode))
    {
        buffer_to_send.push_back(0x03);
        byte = byte | pin_number << 2;
        byte = byte | (start_value == IOIO_VALUE_TRUE) ? 0x20 : 0x00;
        byte = byte | (output_mode == IOIO_OUTPUT_MODE_OPEN_SOURCE) ? 0x01 : 0x00;
        buffer_to_send.push_back(byte);

        fSendData(buffer_to_send);

        f_ports[index].fSetDirection(IOIO_DIRECTION_OUTPUT);
        f_ports[index].fSetOutputMode(output_mode);
        f_ports[index].fSetValue(start_value);
    }

    return buffer_to_send;
}

vector<unsigned char> IOIO::fSetDigitalIn(const int &pin_number)
{
    vector<unsigned char> buffer_to_send;
    unsigned char byte = 0;

    int index = fFindPin(pin_number);

    if(index >= 0)
    {
        buffer_to_send.push_back(0x05);
        byte = byte | pin_number << 2;
        //for now just do pull up always
        byte = byte | 0x1;
        buffer_to_send.push_back(byte);

        fSendData(buffer_to_send);

        f_ports[index].fSetDirection(IOIO_DIRECTION_INPUT);
    }

    return buffer_to_send;
}

std::vector<unsigned char> IOIO::fSetValue(const int &pin_number, const int &value)
{
    vector<unsigned char> buffer_to_send;
    unsigned char byte = 0;

    int index = fFindPin(pin_number);

    if(index >= 0 && IOIO_Port::fIsValueValid(value))
    {
        buffer_to_send.push_back(0x04);
        byte = byte | pin_number << 2;
        byte = byte | (value == IOIO_VALUE_TRUE) ? 0x01 : 0x00;
        buffer_to_send.push_back(byte);

        fSendData(buffer_to_send);

        f_ports[index].fSetValue(value);
    }

    return buffer_to_send;
}

void IOIO::fReceiveData(const vector<unsigned char> &/*data*/)
{

}

int IOIO::fFindPin(const int &pin_number)
{
    int result = -1;

    for(size_t ui=0;ui<f_ports.size();ui++)
    {
        if(f_ports[ui].fGetPortNumber() == pin_number)
        {
            result = ui;
            break;
        }
    }

    return result;
}

void IOIO::fSendData(vector <unsigned char> data)
{
    if(f_send_data && (data.size() > 0))
    {
        f_send_data(static_cast<void*>(data.data()),data.size());
    }
}
