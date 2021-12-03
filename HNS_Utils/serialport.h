/*
 * serialport.h
 *
 *  Created on: Jul 31, 2019
 *      Author: mchristiso
 */

#ifndef SRC_SERIALPORT_H_
#define SRC_SERIALPORT_H_

#include <string>
#include "HNS_CommLink.h"

#ifdef WIN32
#include <Windows.h>

typedef int ssize_t;
typedef HANDLE HNS_type_port;
#elif _UNIX
typedef int HNS_type_port;
#endif

#include "serial_info.h"

class HNS_SerialPort : public HNS_CommLink
{
public:
    HNS_SerialPort();
    HNS_SerialPort(const std::string &device, const type_hns_baud_rate &baud = HNS_SER_BAUD9600);

    ~HNS_SerialPort();

    int fOpen(const std::string &device, const type_hns_baud_rate &baud = HNS_SER_BAUD9600);
    bool fIsOpened();

    bool fWrite(const unsigned char *data, const size_t &datasize);
    bool fRead(unsigned char *data, const size_t &datasize, ssize_t &received);

    int fGetError();
private:
	HNS_type_port f_serialport;
    int f_error;
};





#endif /* SRC_SERIALPORT_H_ */
