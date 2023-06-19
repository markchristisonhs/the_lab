#ifndef OER_H
#define OER_H

#include <vector>
#include <stdint.h>

//This needs a lot of explanation.  This encodes a "non negative unrestricted integer" as per the OER rules found in NTCIP 1102 2.3.2.1.4
//As per the definition laid out within, the integer is technically unrestricted and thus may be arbitrarily large.  There are limits to the
//size but it's effectively infinite and allows value far greater than we would ever want.
//Such an encoding consists of length octets as defined by 2.2.3 followed by content octets as defined by 2.4.1
std::vector<__uint8_t> Encode_NonNegativeUnrestrictedInteger(const std::vector<__uint8_t> &input);
//Same as above, but allows for a more typical case of an input restricted to a 32 byte unsigned int
std::vector<__uint8_t> Encode_NonNegativeUnrestrictedInteger(const __uint32_t &input);

//Implements the encoding of NTCIP 1102 2.3.6 in the case of an unrestricted length octet string which means that the contents must be
//prefeaced with length octets.
std::vector<__uint8_t> Encode_Unrestricted_OctetString(const std::vector<__uint8_t> &input);

#endif // OER_H
