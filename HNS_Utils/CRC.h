#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <vector>

uint16_t CalcCRC(std::vector<unsigned char>input);

uint16_t SwapCRCEndianness(const uint16_t &input);

#endif // CRC_H
