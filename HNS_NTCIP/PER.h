#ifndef PER_H
#define PER_H

#include <vector>
#include <stdint.h>

//This simply enforces the rules as laid out in ITU X.691 section 11.3
//Basically, it just lops off any leading bytes that are 0, thus giving a "minimum octet" as specified in 11.3.6
//input is a non negative unrestricted integer such that index 0 is the most significant byte and index size-1 is the least significant byte
//Also known as big endian
std::vector<__uint8_t> Encode_NonNegativeBinaryInteger(const std::vector<__uint8_t> &input);

#endif // PER_H
