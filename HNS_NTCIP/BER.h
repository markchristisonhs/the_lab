#ifndef BER_H
#define BER_H

#include <vector>
#include <cstdint>

std::vector<unsigned char> BER_Encode_Int(const int &input);
std::vector<unsigned char> BER_Encode_Int64(const int64_t &input);

#endif // BER_H
