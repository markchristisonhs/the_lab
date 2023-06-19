#include "HNS_NTCIP_Datatypes.h"
#include "HNS_NTCIP_Utilities.h"

#include <structures.h>
#include <sstream>

using namespace std;
using namespace NTCIP_MESSAGE;

HNS_NTCIP_MessageIDCode::HNS_NTCIP_MessageIDCode():
    f_memorytype(MEM_TYPE_PERMANENT)
  , f_messagenumber(1)
  , f_crc(2,0)
{

}

HNS_NTCIP_MessageIDCode::HNS_NTCIP_MessageIDCode(const std::vector<uint8_t> &octet_string)
{
    fSetOctetString(octet_string);
}

HNS_NTCIP_MessageIDCode::HNS_NTCIP_MessageIDCode(const type_ntcip_message_memory_type &memory_type, const uint16_t &message_no, const std::vector<uint8_t> &crc)
{
    f_memorytype = memory_type;
    f_messagenumber = message_no;
    fSetCRC(crc);
}

HNS_NTCIP_MessageIDCode::HNS_NTCIP_MessageIDCode(const type_ntcip_message_memory_type &memory_type, const uint16_t &message_no, const uint16_t &crc)
{
    f_memorytype = memory_type;
    f_messagenumber = message_no;
    fSetCRC(crc);
}

void HNS_NTCIP_MessageIDCode::fSetOctetString(const std::vector<uint8_t> &octet_string)
{
    uint16_t itemp = 0;

    if(octet_string.size() >= 5)
    {
        f_memorytype = static_cast<type_ntcip_message_memory_type>(octet_string[0]);

        itemp = 0;
        itemp = octet_string[1];
        itemp = itemp << 8;
        itemp = itemp | octet_string[2];
        f_messagenumber = itemp;

        f_crc.clear();
        f_crc.push_back(octet_string[3]);
        f_crc.push_back(octet_string[4]);
    }
}

vector<uint8_t> HNS_NTCIP_MessageIDCode::fGetOctetString() const
{
    vector<uint8_t> result;

    result.push_back(static_cast<uint8_t>(f_memorytype));

    result.push_back(static_cast<uint8_t>(f_messagenumber >> 8));
    result.push_back(static_cast<uint8_t>(f_messagenumber & 0xFF));

    result.insert(result.end(),f_crc.begin(),f_crc.end());

    return result;
}

void HNS_NTCIP_MessageIDCode::fSetMemoryType(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type)
{
    f_memorytype = memory_type;
}

type_ntcip_message_memory_type HNS_NTCIP_MessageIDCode::fGetMemoryType() const
{
    return f_memorytype;
}

void HNS_NTCIP_MessageIDCode::fSetMessageNumber(const uint16_t &message_no)
{
    f_messagenumber = message_no;
}

uint16_t HNS_NTCIP_MessageIDCode::fGetMessageNumber() const
{
    return f_messagenumber;
}

void HNS_NTCIP_MessageIDCode::fSetCRC(const std::vector<uint8_t> &crc)
{
    f_crc = crc;

    //force to 2 bytes
    if(f_crc.size() > 2)
    {
        f_crc.resize(2);
    }
    else
    {
        while(f_crc.size() < 2)
        {
            f_crc.push_back(0);
        }
    }
}

void HNS_NTCIP_MessageIDCode::fSetCRC(const uint16_t &crc)
{
    vector<unsigned char> temp_vec;

    temp_vec.push_back(static_cast<uint8_t>(crc >> 8));
    temp_vec.push_back(static_cast<uint8_t>(crc & 0xFF));

    fSetCRC(temp_vec);
}

vector<uint8_t> HNS_NTCIP_MessageIDCode::fGetCRCAsVector() const
{
    return f_crc;
}

uint16_t HNS_NTCIP_MessageIDCode::fGetCRCAsInt() const
{
    uint16_t result = 0;

    if(f_crc.size() >= 2)
    {
        result = f_crc[0];
        result = result << 8;
        result = result + f_crc[1];
    }

    return result;
}

bool HNS_NTCIP_MessageIDCode::operator==(const HNS_NTCIP_MessageIDCode &rhs)
{
    if(f_memorytype != rhs.f_memorytype)
    {
        return false;
    }
    if(f_messagenumber != rhs.f_messagenumber)
    {
        return false;
    }
    if(f_crc.size() != rhs.f_crc.size())
    {
        return false;
    }
    for(size_t ui=0;ui<f_crc.size();ui++)
    {
        if(f_crc[ui] != rhs.f_crc[ui])
        {
            return false;
        }
    }

    return true;
}

bool HNS_NTCIP_MessageIDCode::operator!=(const HNS_NTCIP_MessageIDCode &rhs)
{
    return !(*this == rhs);
}

HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode():
    f_duration(0xFFFF)
  , f_priority(1)
  , f_memory_type(MEM_TYPE_PERMANENT)
  , f_message_no(1)
  , f_crc(2,0)
  , f_src_address(4,0)
{

}

HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const std::vector<uint8_t> &octet_string)
{
    fSetOctetString(octet_string);
}

//Constructor with CRC presented as a vector, only the first 2 bytes are used.  And with the source address as a 4 byte vector
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const std::vector<uint8_t> &crc, const std::vector<uint8_t> &src_address):
    f_duration(duration)
  , f_priority(priority)
  , f_memory_type(memory_type)
  , f_message_no(message_no)
  , f_crc(crc)
  , f_src_address(src_address)
{
    //make sure CRC and Source address are formatted correctly.

    //well, not really.  Just trim to size 2 if it's more than 2 bytes, or if less than 2 bytes then pad with 0's until it is 2 bytes
    //can't completely predict what the user wants from bad data.
    if(f_crc.size() > 2)
    {
        f_crc.resize(2);
    }
    else
    {
        while(f_crc.size() < 2)
        {
            f_crc.push_back(0);
        }
    }

    //same for source address at 4 bytes
    if(f_src_address.size() > 4)
    {
        f_src_address.resize(4);
    }
    else
    {
        while(f_src_address.size() < 4)
        {
            f_src_address.push_back(0);
        }
    }
}

//Constructor with CRC presented as a 16 bit int, and the source address as a 4 byte vector
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const uint16_t &crc, const std::vector<uint8_t> &src_address)
{
//store CRC as big endian
    vector<uint8_t> temp_vec;
    temp_vec.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
    temp_vec.push_back(static_cast<uint8_t>(crc & 0xFF));

    *this = HNS_NTCIP_MessageActivationCode(duration,priority,memory_type,message_no,temp_vec,src_address);
}

//constructor with CRC calculated from message, beacon, and pixel service values.  Source address as a 4 byte vector
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const std::string &message, const std::vector<uint8_t> &src_address,
                                const uint8_t &beacon, const uint8_t &pixel_svc)
{
    uint16_t crc = CalculateMessageCRC(message,beacon,pixel_svc);

    *this = HNS_NTCIP_MessageActivationCode(duration,priority,memory_type,message_no,crc,src_address);
}

//Constructor with CRC presented as a vector, only the first 2 bytes are used.  And with the source address as a string in normal IP address format
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const std::vector<uint8_t> &crc, const std::string &src_address)
{
    vector<uint8_t> ip_vec = HNS_IP_Settings::fIPStringtoByteArray(src_address);

    *this = HNS_NTCIP_MessageActivationCode(duration,priority,memory_type,message_no,crc,ip_vec);
}

//Constructor with CRC presented as a 16 bit int, and the source address as a string in normal IP address format
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const uint16_t &crc, const std::string &src_address)
{
    vector<uint8_t> ip_vec = HNS_IP_Settings::fIPStringtoByteArray(src_address);

    *this = HNS_NTCIP_MessageActivationCode(duration,priority,memory_type,message_no,crc,ip_vec);
}

//constructor with CRC calculated from message, beacon, and pixel service values.  Source address as a string in normal IP address format
HNS_NTCIP_MessageActivationCode::HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                const uint16_t &message_no, const std::string &message, const std::string &src_address,
                                const uint8_t &beacon, const uint8_t &pixel_svc)
{
    vector<uint8_t> ip_vec = HNS_IP_Settings::fIPStringtoByteArray(src_address);

    *this = HNS_NTCIP_MessageActivationCode(duration,priority,memory_type,message_no,message,ip_vec,beacon,pixel_svc);
}

void HNS_NTCIP_MessageActivationCode::fSetOctetString(const std::vector<uint8_t> &octet_string)
{
    uint16_t itemp = 0;

    if(octet_string.size() >= 12)
    {
        //assume big endian;
        itemp = octet_string[0];
        itemp = itemp << 8;
        itemp = itemp | octet_string[1];
        f_duration = itemp;

        f_priority = octet_string[2];
        f_memory_type = static_cast<type_ntcip_message_memory_type>(octet_string[3]);

        itemp = 0;
        itemp = octet_string[4];
        itemp = itemp << 8;
        itemp = itemp | octet_string[5];
        f_message_no = itemp;

        f_crc.clear();
        f_crc.push_back(octet_string[6]);
        f_crc.push_back(octet_string[7]);

        f_src_address.clear();
        f_src_address.push_back(octet_string[8]);
        f_src_address.push_back(octet_string[9]);
        f_src_address.push_back(octet_string[10]);
        f_src_address.push_back(octet_string[11]);
    }
}

vector<uint8_t> HNS_NTCIP_MessageActivationCode::fGetOctetString() const
{
    vector<uint8_t> result;

    result.push_back(static_cast<uint8_t>(f_duration >> 8));
    result.push_back(static_cast<uint8_t>(f_duration & 0xFF));

    result.push_back(f_priority);

    result.push_back(static_cast<uint8_t>(f_memory_type));

    result.push_back(static_cast<uint8_t>(f_message_no >> 8));
    result.push_back(static_cast<uint8_t>(f_message_no & 0xFF));

    result.insert(result.end(),f_crc.begin(),f_crc.end());
    result.insert(result.end(),f_src_address.begin(),f_src_address.end());

    return result;
}

void HNS_NTCIP_MessageActivationCode::fSetDuration(const uint16_t &duration)
{
    f_duration = duration;
}

uint16_t HNS_NTCIP_MessageActivationCode::fGetDuration() const
{
    return f_duration;
}

void HNS_NTCIP_MessageActivationCode::fSetPriority(const uint16_t &priority)
{
    f_priority = priority;
}

uint8_t HNS_NTCIP_MessageActivationCode::fGetPriority() const
{
    return f_priority;
}

void HNS_NTCIP_MessageActivationCode::fSetMemoryType(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type)
{
    f_memory_type = memory_type;
}

NTCIP_MESSAGE::type_ntcip_message_memory_type HNS_NTCIP_MessageActivationCode::fGetMemoryType() const
{
    return f_memory_type;
}

void HNS_NTCIP_MessageActivationCode::fSetMessageNo(const uint16_t &message_no)
{
    f_message_no = message_no;
}

uint16_t HNS_NTCIP_MessageActivationCode::fGetMessageNo() const
{
    return f_message_no;
}

void HNS_NTCIP_MessageActivationCode::fSetCRC(const std::vector<uint8_t> &crc)
{
    f_crc = crc;

    //force to 2 bytes
    if(f_crc.size() > 2)
    {
        f_crc.resize(2);
    }
    else
    {
        while(f_crc.size() < 2)
        {
            f_crc.push_back(0);
        }
    }
}

void HNS_NTCIP_MessageActivationCode::fSetCRC(const uint16_t &crc)
{
    vector<unsigned char> temp_vec;

    temp_vec.push_back(static_cast<uint8_t>(crc >> 8));
    temp_vec.push_back(static_cast<uint8_t>(crc & 0xFF));

    fSetCRC(temp_vec);
}

void HNS_NTCIP_MessageActivationCode::fSetCRC(const std::string &message, const uint8_t &beacon, const uint8_t &pixel_svc)
{
    uint16_t crc = CalculateMessageCRC(message,beacon,pixel_svc);

    fSetCRC(crc);
}

std::vector<uint8_t> HNS_NTCIP_MessageActivationCode::fGetCRCAsVector() const
{
    return f_crc;
}

uint16_t HNS_NTCIP_MessageActivationCode::fGetCRCAsInt() const
{
    uint16_t result = 0;

    if(f_crc.size() >= 2)
    {
        result = f_crc[0];
        result = result << 8;
        result = result + f_crc[1];
    }

    return result;
}

void HNS_NTCIP_MessageActivationCode::fSetSrcAddr(const std::vector<uint8_t> &src_addr)
{
    f_src_address = src_addr;

    //fix to 4 bytes
    if(f_src_address.size() > 4)
    {
        f_src_address.resize(4);
    }
    else
    {
        while(f_src_address.size() < 4)
        {
            f_src_address.push_back(0);
        }
    }
}

void HNS_NTCIP_MessageActivationCode::fSetSrcAddr(const std::string &src_addr)
{
    f_src_address = HNS_IP_Settings::fIPStringtoByteArray(src_addr);
}

std::vector<uint8_t> HNS_NTCIP_MessageActivationCode::fGetSrcAddrAsVec() const
{
    return f_src_address;
}

std::string HNS_NTCIP_MessageActivationCode::fGetSrcAddrAsString() const
{
    return HNS_IP_Settings::fIPByteArraytoString(f_src_address);
}

bool HNS_NTCIP_MessageActivationCode::operator==(const HNS_NTCIP_MessageActivationCode &rhs)
{
    if(f_duration != rhs.f_duration)
    {
        return false;
    }
    if(f_memory_type != rhs.f_memory_type)
    {
        return false;
    }
    if(f_message_no != rhs.f_message_no)
    {
        return false;
    }
    if(f_priority != rhs.f_priority)
    {
        return false;
    }
    if(f_crc.size() != rhs.f_crc.size())
    {
        return false;
    }
    for(size_t ui=0;ui<f_crc.size();ui++)
    {
        if(f_crc[ui] != rhs.f_crc[ui])
        {
            return false;
        }
    }
    if(f_src_address.size() != rhs.f_src_address.size())
    {
        return false;
    }
    for(size_t ui=0;ui<f_src_address.size();ui++)
    {
        if(f_src_address[ui] != rhs.f_src_address[ui])
        {
            return false;
        }
    }
    return true;
}

bool HNS_NTCIP_MessageActivationCode::operator!=(const HNS_NTCIP_MessageActivationCode &rhs)
{
    return !(*this == rhs);
}

HNS_NTCIP_OID::HNS_NTCIP_OID()
{

}

HNS_NTCIP_OID::HNS_NTCIP_OID(const string &oid_string)
{
    fSetData(oid_string);
}

HNS_NTCIP_OID::HNS_NTCIP_OID(const vector<uint8_t> &oid_octet_string):
    f_oid_octet_string(oid_octet_string)
{

}

void HNS_NTCIP_OID::fSetData(const string &oid_string)
{
    istringstream iss;
    string token;
    uint8_t temp;
    vector<int> oid_vec;
    vector<uint8_t> temp_vec;
    int itemp,j;

    iss.str(oid_string);

    while(getline(iss,token,'.'))
    {
        oid_vec.push_back(stoi(token));
    }

    temp = 0x00;
    f_oid_octet_string.clear();
    for(size_t i=0;i<oid_vec.size();i++)
    {
        if(i==0)
        {
            temp = oid_vec[i] * 40;
        }
        else if(i==1)
        {
            temp += oid_vec[i];
            f_oid_octet_string.push_back(temp);
        }
        else
        {
            if(oid_vec[i] <= 0x7F)
            {
                f_oid_octet_string.push_back(oid_vec[i]);
            }
            else
            {
                temp_vec.clear();
                j = 0;
                itemp = oid_vec[i];
                while(itemp != 0)
                {
                    temp = static_cast<unsigned char>(itemp & 0x7F);
                    if(j > 0)
                    {
                        temp = temp | 0x80;
                    }
                    temp_vec.insert(temp_vec.begin(),temp);
                    itemp = itemp >> 7;
                    j++;
                }
                f_oid_octet_string.insert(f_oid_octet_string.end(),temp_vec.begin(),temp_vec.end());
            }
        }
    }
}

void HNS_NTCIP_OID::fSetData(const vector<uint8_t> &oid_octet_string)
{
    f_oid_octet_string = oid_octet_string;
}

string HNS_NTCIP_OID::fGetOIDString() const
{
    string result;
    stringstream ss;
    int itemp;
    uint8_t ctemp;
    string tempstring;

    for(size_t i=0;i<f_oid_octet_string.size();i++)
    {
        if(i == 0)
        {
            ctemp = f_oid_octet_string[i];
            ss << (ctemp / 40) << "." << (ctemp%40);
            tempstring = ss.str();
        }
        else
        {
            if(f_oid_octet_string[i] > 0x80)
            {
                itemp = f_oid_octet_string[i] & 0x7F;
                do
                {
                    itemp = itemp << 7;
                    i++;
                    if(i >= f_oid_octet_string.size())
                    {
                        break;
                    }
                    else
                    {
                        itemp = itemp | (f_oid_octet_string[i] & 0x7F);
                    }
                } while(f_oid_octet_string[i] > 0x80);
                ss << "." << itemp;
                tempstring = ss.str();
            }
            else
            {
                ss << "." << static_cast<unsigned int>(f_oid_octet_string[i]);
                tempstring = ss.str();
            }
        }
    }
    result = ss.str();

    return result;
}

vector<uint8_t> HNS_NTCIP_OID::fGetOIDOctetString() const
{
    return f_oid_octet_string;
}

bool HNS_NTCIP_OID::fIsBranchOf(const HNS_NTCIP_OID &oid) const
{
    vector<int> input_oid = oid.fGetOIDIntString(),current_oid = fGetOIDIntString();

    if(current_oid.size() >= input_oid.size())
    {
        for(size_t i=0;i<input_oid.size();i++)
        {
            if(current_oid[i] != input_oid[i])
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

vector<int> HNS_NTCIP_OID::fGetOIDIntString() const
{
    vector<int> result;
    int itemp;
    uint8_t ctemp;

    for(size_t i=0;i<f_oid_octet_string.size();i++)
    {
        if(i == 0)
        {
            ctemp = f_oid_octet_string[i];
            result.push_back(ctemp/40);
            result.push_back(ctemp%40);
        }
        else
        {
            if(f_oid_octet_string[i] > 0x80)
            {
                itemp = f_oid_octet_string[i] & 0x7F;
                do
                {
                    itemp = itemp << 7;
                    i++;
                    if(i >= f_oid_octet_string.size())
                    {
                        break;
                    }
                    else
                    {
                        itemp = itemp | (f_oid_octet_string[i] & 0x7F);
                    }
                } while(f_oid_octet_string[i] > 0x80);
                result.push_back(itemp);
            }
            else
            {
                result.push_back(static_cast<int>(f_oid_octet_string[i]));
            }
        }
    }

    return result;
}
