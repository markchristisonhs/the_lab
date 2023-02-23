/*
 * serialport.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: mchristiso
 */

#include "stdafx.h"

#include "serialport.h"

#include <stdio.h>
#include <fcntl.h>
#ifdef _UNIX
#include <termios.h>
#include <unistd.h>
#endif
#include <stddef.h>
#include <cstring>
#include <errno.h>

#include <string>
#include <sstream>
#include <vector>

using namespace std;

HNS_SerialPort::HNS_SerialPort()
    :f_serialport(0),
      f_error(HNS_ERR_SER_NOERR),
      f_logaline(nullptr)
{

}

HNS_SerialPort::HNS_SerialPort(const string &device, const type_hns_baud_rate &baud)
    :f_serialport(0),
      f_error(HNS_ERR_SER_NOERR),
      f_logaline(nullptr)
{
    fOpen(device,baud);
}

HNS_SerialPort::~HNS_SerialPort()
{
#ifdef WIN32
	CloseHandle(f_serialport);
#endif
}

int HNS_SerialPort::fOpen(const string &device, const type_hns_baud_rate &baud)
{
    int temp_error;
    if(!fIsOpened())
    {
#ifdef _UNIX
        struct termios tty;

        f_error = HNS_ERR_SER_NOERR;

        f_serialport = open(device.c_str(), O_RDWR);

        if(f_serialport < 0)
        {
            temp_error = errno;
            if (errno == 2)
            {
                f_error = HNS_ERR_SER_NOFILE;
            }
            else if(errno == 13)
            {
                f_error = HNS_ERR_SER_NOPERM;
            }
            return f_error;
        }
        memset (&tty,0, sizeof (tty));
        if(tcgetattr(f_serialport,&tty) != 0)
        {
            f_error = HNS_ERR_SER_NOATTR;
        }

        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;

        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;

        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;

        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;

        if(baud == HNS_SER_BAUD115200)
        {
            cfsetispeed(&tty,B115200);
            cfsetospeed(&tty,B115200);
        }
        else
        {
            cfsetispeed(&tty,B9600);
            cfsetospeed(&tty,B9600);
        }

        if(tcsetattr(f_serialport, TCSANOW, &tty) != 0)
        {
            f_error = HNS_ERR_SER_ATTRWR;
        }
#elif WIN32
		wchar_t wtext[256];
		size_t itemp;
		mbstowcs_s(&itemp,wtext,device.c_str(),256);
		f_serialport = CreateFile(wtext,
								  GENERIC_READ|GENERIC_WRITE,
								  0,
								  0,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL,
								  0);
		if(f_serialport == INVALID_HANDLE_VALUE)
		{
			if(GetLastError() == ERROR_FILE_NOT_FOUND)
			{
				f_error = HNS_ERR_SER_NOFILE;
			}
		}
		else
		{
			DCB dcbSerialParams = {0};

			dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
			if (!GetCommState(f_serialport, &dcbSerialParams)) 
			{
				f_error = HNS_ERR_SER_NOATTR;
			}
			dcbSerialParams.BaudRate=CBR_19200;
			dcbSerialParams.ByteSize=8;
			dcbSerialParams.StopBits=ONESTOPBIT;
			dcbSerialParams.Parity=NOPARITY;
			if(!SetCommState(f_serialport, &dcbSerialParams))
			{
				f_error = HNS_ERR_SER_ATTRWR;
			}

			COMMTIMEOUTS timeouts={0};
			timeouts.ReadIntervalTimeout=50;
			timeouts.ReadTotalTimeoutConstant=50;
			timeouts.ReadTotalTimeoutMultiplier=10;
			timeouts.WriteTotalTimeoutConstant=50;
			timeouts.WriteTotalTimeoutMultiplier=10;
			if(!SetCommTimeouts(f_serialport, &timeouts))
			{
				f_error = HNS_ERR_SER_ATTRWR;
			}
		}
#endif
        return f_error;
    }
    return HNS_ERR_SER_ALREADY_OPEN;
}

bool HNS_SerialPort::fIsOpened()
{
    return (f_serialport > 0);
}

bool HNS_SerialPort::fWrite(const unsigned char *data, const size_t &datasize)
{
    string tempstring;
    stringstream ss;
    vector<unsigned char> temp_vec;

    if(fIsOpened())
    {
#ifdef _UNIX
        //ioctl(f_serialport,TCFLSH,0);
        if(f_logaline != nullptr)
        {
            ss << "Sending " << datasize << " bytes of data";
            tempstring = ss.str();
            f_logaline->fLogALine(tempstring);

            temp_vec = vector<unsigned char>(data, data+datasize);
            f_logaline->fLogBuffer(temp_vec);
        }
        write(f_serialport,data,datasize);
#elif WIN32
		DWORD written;
		if(!WriteFile(f_serialport,data,datasize,&written,NULL))
		{
			f_error = HNS_ERR_SER_READERR;
		}
#endif
        return true;
    }
	else
    {
        f_error = HNS_ERR_SER_NOT_OPENED;
        return false;
    }
}

bool HNS_SerialPort::fRead(unsigned char *data, const size_t &datasize, ssize_t &received)
{
    string tempstring;
    stringstream ss;
    vector<unsigned char> temp_vec;

    if(fIsOpened())
    {
#ifdef _UNIX
        ssize_t n;

        n = read(f_serialport,data,datasize);

        if(n < 0)
        {
            received = 0;
            f_error = HNS_ERR_SER_READERR;
            return false;
        }
        if(n > 0 && f_logaline != nullptr)
        {
            ss << "Received " << n << " bytes of data";
            tempstring = ss.str();
            f_logaline->fLogALine(tempstring);

            temp_vec = vector<unsigned char>(data, data+n);
            f_logaline->fLogBuffer(temp_vec);
        }
        received = n;
        f_error = HNS_ERR_SER_NOERR;
#elif WIN32
		DWORD itemp;
		if(!ReadFile(f_serialport,data,datasize,&itemp,NULL))
		{
			received = 0;
            f_error = HNS_ERR_SER_READERR;
		}
		else
		{
			received = itemp;
		}
#endif
        return true;
    }
    else
    {
        f_error = HNS_ERR_SER_NOT_OPENED;
        return false;
    }
}

int HNS_SerialPort::fGetError()
{
    return f_error;
}

void HNS_SerialPort::fSetLog(HNS_LogALine2 *logaline)
{
    f_logaline = logaline;
}
