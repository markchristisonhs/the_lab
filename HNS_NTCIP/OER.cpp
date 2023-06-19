#include "OER.h"
#include <utilities.h>
#include <PER.h>

using namespace std;

vector<__uint8_t> Encode_NonNegativeUnrestrictedInteger(const std::vector<__uint8_t> &input)
{
    vector<__uint8_t> result, content, length;

    content = Encode_NonNegativeBinaryInteger(input);
    length = ConvSizeT_to_Legnth(content.size());

    result = length;
    result.insert(result.end(),content.begin(),content.end());

    return result;
}

vector<__uint8_t> Encode_NonNegativeUnrestrictedInteger(const __uint32_t &input)
{
    //input turned into a big endian vector
    vector<__uint8_t> temp_vec = HNS_intToByteArray(input);

    return Encode_NonNegativeUnrestrictedInteger(temp_vec);
}

vector<__uint8_t> Encode_Unrestricted_OctetString(const vector<__uint8_t> &input)
{
    vector<__uint8_t> result;

    result = ConvSizeT_to_Legnth(input.size());
    result.insert(result.end(),input.begin(),input.end());

    return result;
}
