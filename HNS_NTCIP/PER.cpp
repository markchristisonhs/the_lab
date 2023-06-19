#include "PER.h"

using namespace std;

vector<__uint8_t> Encode_NonNegativeBinaryInteger(const vector<__uint8_t> &input)
{
    vector<__uint8_t> result = input;

    bool done = false;
    while((result.size() > 0) && !done)
    {
        if(result[0] == 0)
        {
            result.erase(result.begin());
        }
        else
        {
            done = true;
        }
    }

    return result;
}
