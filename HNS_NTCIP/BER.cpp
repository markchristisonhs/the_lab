#include "BER.h"

using namespace std;

vector<unsigned char> BER_Encode_Int(const int &input)
{
    int i;
    std::vector<unsigned char> byteArray(4);
    bool done;
    unsigned char uitemp,uitemp2;

    for(i=0;i<4;i++)
    {
        byteArray[3 - i] = (input >> (i*8));
    }

    done = false;
    do
    {
        if(byteArray.size() > 1)
        {
            uitemp = *byteArray.begin();
            uitemp2 = *(byteArray.begin()+1);

            if((uitemp == 0x00) && ((uitemp2 & 0x80) == 0))
            {
                byteArray.erase(byteArray.begin());
            }
            else if((uitemp == 0xFF) && ((uitemp2 & 0x80) != 0))
            {
                byteArray.erase(byteArray.begin());
            }
            else
            {
                done = true;
            }
        }
        else
        {
            done = true;
        }
    } while(!done);

    return byteArray;
}

vector<unsigned char> BER_Encode_Int64(const int64_t &input)
{
    int i;
    std::vector<unsigned char> byteArray(8);
    bool done;
    unsigned char uitemp,uitemp2;

    for(i=0;i<8;i++)
    {
        byteArray[7 - i] = (input >> (i*8));
    }

    done = false;
    do
    {
        if(byteArray.size() > 1)
        {
            uitemp = *byteArray.begin();
            uitemp2 = *(byteArray.begin()+1);

            if((uitemp == 0x00) && ((uitemp2 & 0x80) == 0))
            {
                byteArray.erase(byteArray.begin());
            }
            else if((uitemp == 0xFF) && ((uitemp2 & 0x80) != 0))
            {
                byteArray.erase(byteArray.begin());
            }
            else
            {
                done = true;
            }
        }
        else
        {
            done = true;
        }
    } while(!done);

    return byteArray;
}
