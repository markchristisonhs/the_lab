#include "HNS_NTCIP_Utilities.h"
#include "HNS_NTCIP_MIB_Strings.h"

#include <vector>
#include <string>
#include <sstream>
#include <CRC.h>

using namespace std;
using namespace NTCIP_MESSAGE;
using namespace NTCIP_GRAPHICS;

int CountMessages(NTCIP_Node *ntcip, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type)
{
    int result = 0;
    stringstream ss;
    int itemp;

    size_t max_messages = MaxMessages(ntcip,memory_type);

    switch(memory_type)
    {
    case MEM_TYPE_BLANK:
    case MEM_TYPE_SCHEDULE:
    case MEM_TYPE_CURRENT:
    case MEM_TYPE_PERMANENT:
        result = max_messages;
        break;
    case MEM_TYPE_CHANGEABLE:
    case MEM_TYPE_VOLATILE:
        break;
    }

    if((memory_type == MEM_TYPE_CHANGEABLE)
    || (memory_type == MEM_TYPE_VOLATILE))
    {
        for(size_t i=0;i<max_messages;i++)
        {
            ss.str(std::string());
            ss << gk_dmsMessageStatus << "." << memory_type << "." << i+1;
            itemp = ntcip->fGetIntData(ss.str());
            if(itemp == 4)
            {
                result++;
            }
        }
    }

    return result;
}

int MaxMessages(NTCIP_Node *ntcip, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type)
{
    int result = 0;

    switch(memory_type)
    {
    case MEM_TYPE_PERMANENT:
        result = ntcip->fGetIntData(gk_dmsNumPermanentMsg);
        break;
    case MEM_TYPE_CHANGEABLE:
        result = ntcip->fGetIntData(gk_dmsMaxChangeableMsg);
        break;
    case MEM_TYPE_VOLATILE:
        result = ntcip->fGetIntData(gk_dmsMaxVolatileMsg);
        break;
    case MEM_TYPE_CURRENT:
        result = 1;
        break;
    case MEM_TYPE_SCHEDULE:
        result = 1;
        break;
    case MEM_TYPE_BLANK:
        result = 255;
        break;
    }

    return result;
}

int CountGraphics(NTCIP_Node *ntcip)
{
    int result = 0;
    stringstream ss;
    int itemp;

    size_t max_graphics = 0;

    max_graphics = ntcip->fGetIntData(gk_dmsGraphicMaxEntries);

    for(size_t i=0;i<max_graphics;i++)
    {
        ss.str(string());
        ss << gk_dmsGraphicStatus << "." << i+1;
        itemp = ntcip->fGetIntData(ss.str());
        if((itemp == GRAPHIC_STATUS_READY_FOR_USE)
        || (itemp == GRAPHIC_STATUS_IN_USE)
        || (itemp == GRAPHIC_STATUS_PERMANENT))
        {
            result++;
        }
    }

    return result;
}

bool IsSecurity(const HNS_NTCIP_OID &oid)
{
    return oid.fIsBranchOf(gk_security);
}

uint16_t CalculateMessageCRC(const std::string &message, const uint8_t &beacon, const uint8_t &pixel)
{
    vector<unsigned char> temp_vec;
    for(size_t i=0;i<message.size();i++)
    {
        temp_vec.push_back(static_cast<unsigned char>(message[i]));
    }
    //beacon and pixel should only be 1 or 0.  However, assume that this has been enforced higher up and take them at face value
    //The standard may be revised later
    temp_vec.push_back(beacon);
    temp_vec.push_back(pixel);

    return CalcCRC(temp_vec);
}

vector<unsigned char> GetBitmapDataStream(const HNS_Bitmap &bitmap, const type_ntcip_graphic_type &graphicType)
{
    vector<unsigned char> result;
    HNS_Color temp_color;

    unsigned char byte = 0;
    size_t bit_count = 0;
    int itemp;

    for(size_t y = 0; y < bitmap.fGetHeight();y++)
    {
        for(size_t x = 0; x < bitmap.fGetWidth();x++)
        {
            temp_color = bitmap.fGetPixel(x,y);

            if(graphicType == graphictype_monochrome8bit)
            {
                //take the average of colors to form a greyscale.
                itemp = (temp_color.fR() + temp_color.fG() + temp_color.fG())/3;
                result.push_back(static_cast<unsigned char>(itemp));
            }
            else if(graphicType == graphictype_colorclassic)
            {
                if(temp_color == gk_classic_black)
                {
                    result.push_back(0);
                }
                else if(temp_color == gk_classic_red)
                {
                    result.push_back(1);
                }
                else if(temp_color == gk_classic_yellow)
                {
                    result.push_back(2);
                }
                else if(temp_color == gk_classic_green)
                {
                    result.push_back(3);
                }
                else if(temp_color == gk_classic_cyan)
                {
                    result.push_back(4);
                }
                else if(temp_color == gk_classic_blue)
                {
                    result.push_back(5);
                }
                else if(temp_color == gk_classic_magenta)
                {
                    result.push_back(6);
                }
                else if(temp_color == gk_classic_white)
                {
                    result.push_back(7);
                }
                else if(temp_color == gk_classic_orange)
                {
                    result.push_back(8);
                }
                //assume amber if it's not one of the other colors.
                else
                {
                    result.push_back(9);
                }
            }
            else if(graphicType == graphictype_color24bit)
            {
                //as per NTCIP1203, the bytes are ordered bgr
                result.push_back(temp_color.fB());
                result.push_back(temp_color.fG());
                result.push_back(temp_color.fR());
            }
            //MSC20210527 Assume a default case of monochrome 1 bit.
            else
            {
                byte = byte | (temp_color == HNS_Color(0,0,0) ? 0x0 : 0x1);
                bit_count++;

                if(bit_count == 8)
                {
                    bit_count = 0;
                    result.push_back(byte);
                    byte = 0;
                }
                else
                {
                    byte = byte << 1;
                }
            }
        }
    }
    return result;
}
