/*
 * utilities.h
 *
 *  Created on: Aug 23, 2019
 *      Author: mchristiso
 */

#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#include <string>
#include <vector>
#include <ctime>

//This function takes a string of numbers seperated by a delimiting character, and returns a vector of
//numbers.  It assumes that there are only numbers between delimiters and gives an error if there are
//non-numbers in the sequence.  At this time only supports bases 10 and 16.  That should be all that's
//needed though.
std::vector<int> HNS_string_to_int_vector(const std::string &input, const char &delimiter, int &error, const bool &is_hex = false);

//This function is a simple Celsius to Fahrenheit converter
double HNS_CtoF(const double &temperature);
//This function is a simple Fahrenheit to Celsius converter
double HNS_FtoC(const double &temperature);

//Splits an int into a big endian byte array (assuming that index 0 is MSB and 0 to size-1 is read left to right).  Assumes that int is 4 bytes
std::vector<unsigned char> HNS_intToByteArray(const int &input);
//Same as above for 8 bytes
std::vector<unsigned char> HNS_int64ToByteArray(const int64_t &input);

int HNS_ByteArrayToint(const std::vector <unsigned char> &input);
int64_t HNS_ByteArrayToint64(const std::vector <unsigned char> &input);

//convert a size_t to an array of bytes as dictated by OER and BER
std::vector<unsigned char> ConvSizeT_to_Legnth(const size_t &length);

std::vector<unsigned char> HexStringToByteArray(const std::string &input);

double Round(const double &input, const int &places = 0);

int Conv24Hto12HTime(const int &time_24, bool *am_pm = nullptr);

//if end is after start, return 1.
//if the dates are equal return 0.
//if end is earlier than start, return -1.
//ignore time component
int CompareDates(const tm &end_date, const tm &start_date);

//same return as Compare Dates, but ignores date component
int CompareTimes(const tm &end_time, const tm &start_time);

//since vector<bool> doesn't always behave the same way across compilers, I have opted to use vector<unsigned char> with zero as false and non zero as true
//I hope this gives more predictable behavior across compilers
std::vector<unsigned char> BoolArray_to_ByteArray(const std::vector<unsigned char> &input);

#endif /* SRC_UTILITIES_H_ */
