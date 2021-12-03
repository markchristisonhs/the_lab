/*
 * utilites.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: mchristiso
 */

#include "stdafx.h"

#include "utilities.h"

#include <stdlib.h>
#include <cmath>
#include <errno.h>

using namespace std;

vector<int> HNS_string_to_int_vector(const string &input, const char &delimiter, int &error)
{
	vector<int> result;
	string temp_string = input,temp_string2;
	size_t index = 0;
	long ltemp;
	char *endptr = nullptr;

	errno = 0;

	error = 0;
	do
	{
	    index = temp_string.find_first_of(delimiter);
		temp_string2 = temp_string.substr(0,index);
		ltemp = strtol(temp_string2.c_str(),&endptr,10);
		if(errno != 0)
		{
			//some error occurred
			error = 1;
			break;
		}
		else if(endptr == temp_string2.c_str())
		{
			//invalid entry
			error = 1;
			break;
		}
		else if(*endptr != 0)
		{
			//valid number but offending non-number characters are left over
			error = 1;
			break;
		}
		else
		{
			result.push_back(ltemp);
		}


		if(index == string::npos) break;
		temp_string = temp_string.substr(index+1);
        //}while((index = temp_string.find_first_of(delimiter)) != string::npos);
    }while(1);

	return result;
}

double HNS_CtoF(const double &temperature)
{
    return (temperature * (9.0/5.0)) + 32.0;
}

double HNS_FtoC(const double &temperature)
{
    return (temperature - 32) * (5.0/9.0);
}

vector<unsigned char> HNS_intToByteArray(const int &input)
{
    int i;
    std::vector<unsigned char> byteArray(4);

    for(i=0;i<4;i++)
    {
        byteArray[3 - i] = (input >> (i*8));
    }

    return byteArray;
}

vector<unsigned char> HNS_int64ToByteArray(const int64_t &input)
{
    int i;
    std::vector<unsigned char> byteArray(8);

    for(i=0;i<8;i++)
    {
        byteArray[7 - i] = (input >> (i*8));
    }

    return byteArray;
}

int HNS_ByteArrayToint(const std::vector <unsigned char> &input)
{
    int result = 0;
    size_t ilimit = min(size_t(4),input.size());

    for(size_t i=0;i<ilimit;i++)
    {
        result = result | (input[i] << ((ilimit - 1 - i)*8));
    }

    return result;
}

int64_t HNS_ByteArrayToint64(const std::vector <unsigned char> &input)
{
    int64_t result = 0;
    size_t ilimit = min(size_t(8),input.size());

    for(size_t i=0;i<ilimit;i++)
    {
        result = result | (input[i] << ((ilimit - 1 - i)*8));
    }

    return result;
}

std::vector<unsigned char> ConvSizeT_to_Legnth(const size_t &length)
{
    size_t temp = length;
    unsigned char legnth_of_length;
    std::vector<unsigned char> result;

    if(length <= 0x7F)
    {
        result.push_back(static_cast<unsigned char>(length));
    }
    else
    {
        while(temp != 0)
        {
            result.insert(result.begin(),static_cast<unsigned char>(temp & 0xFF));
            temp = temp >> 8;
        }
        legnth_of_length = static_cast<unsigned char> (result.size()) | 0x80;
        result.insert(result.begin(),legnth_of_length);
    }

    return result;
}

vector<unsigned char> HexStringToByteArray(const string &input)
{
    vector<unsigned char> result;

    for(size_t i=0;i<input.length();i+=2)
    {
        string byteString = input.substr(i,2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(),NULL,16));
        result.push_back(byte);
    }

    return result;
}

int Conv24Hto12HTime(const int &time_24, bool *am_pm)
{
    int result = time_24;

    if(time_24 > 12)
    {
        if(am_pm != nullptr)
        {
            *am_pm = true;
        }
        result = time_24 - 12;
    }
    else if(time_24 == 0)
    {
        if(am_pm != nullptr)
        {
            *am_pm = false;
        }
        result = 12;
    }
    else
    {
        if(am_pm != nullptr)
        {
            *am_pm = false;
        }
    }

    return result;
}

int CompareDates(const tm &end_date, const tm &start_date)
{
    if(end_date.tm_year > start_date.tm_year)
    {
        return 1;
    }
    else if(end_date.tm_year == start_date.tm_year)
    {
        if(end_date.tm_mon > start_date.tm_mon)
        {
            return 1;
        }
        else if(end_date.tm_mon == start_date.tm_mon)
        {
            if(end_date.tm_mday > start_date.tm_mday)
            {
                return 1;
            }
            else if(end_date.tm_mday == start_date.tm_mday)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

int CompareTimes(const tm &end_time, const tm &start_time)
{
    if(end_time.tm_hour > start_time.tm_hour)
    {
        return 1;
    }
    else if(end_time.tm_hour == start_time.tm_hour)
    {
        if(end_time.tm_min > start_time.tm_min)
        {
            return 1;
        }
        else if(end_time.tm_min == start_time.tm_min)
        {
            if(end_time.tm_sec > start_time.tm_sec)
            {
                return 1;
            }
            if(end_time.tm_sec == start_time.tm_sec)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

double Round(const double &input, const int &places)
{
    double result = input;
    result = result * pow(10,places);
    result = floor(result + 0.5);
    result = result / pow(10,places);

    return result;
}
