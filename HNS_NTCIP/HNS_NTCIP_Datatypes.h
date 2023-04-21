#ifndef HNS_NTCIP_DATATYPES_H
#define HNS_NTCIP_DATATYPES_H

#include "HNS_NTCIP_Enums.h"
#include <stdint.h>
#include <vector>
#include <string>

class HNS_NTCIP_MessageIDCode
{
public:
    HNS_NTCIP_MessageIDCode();
    HNS_NTCIP_MessageIDCode(const std::vector<uint8_t> &octet_string);
    HNS_NTCIP_MessageIDCode(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const uint16_t &message_no, const std::vector<uint8_t> &crc);
    HNS_NTCIP_MessageIDCode(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const uint16_t &message_no, const uint16_t &crc);

    void fSetOctetString(const std::vector<uint8_t> &octet_string);
    std::vector<uint8_t> fGetOctetString() const;

    void fSetMemoryType(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type);
    NTCIP_MESSAGE::type_ntcip_message_memory_type fGetMemoryType() const;
    void fSetMessageNumber(const uint16_t &message_no);
    uint16_t fGetMessageNumber() const;

    void fSetCRC(const std::vector<uint8_t> &crc);
    void fSetCRC(const uint16_t &crc);
    std::vector<uint8_t> fGetCRCAsVector() const;
    uint16_t fGetCRCAsInt() const;
private:
    NTCIP_MESSAGE::type_ntcip_message_memory_type f_memorytype;
    uint16_t f_messagenumber;
    std::vector<unsigned char> f_crc;
};

class HNS_NTCIP_MessageActivationCode
{
public:
    HNS_NTCIP_MessageActivationCode();
    //direct input from octet string
    HNS_NTCIP_MessageActivationCode(const std::vector<uint8_t> &octet_string);
    //Constructor with CRC presented as a vector, only the first 2 bytes are used.  And with the source address as a 4 byte vector
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const std::vector<uint8_t> &crc, const std::vector<uint8_t> &src_address);
    //Constructor with CRC presented as a 16 bit int, and the source address as a 4 byte vector
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const uint16_t &crc, const std::vector<uint8_t> &src_address);
    //constructor with CRC calculated from message, beacon, and pixel service values.  Source address as a 4 byte vector
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const std::string &message, const std::vector<uint8_t> &src_address,
                                    const uint8_t &beacon = 0, const uint8_t &pixel_svc = 0);
    //Constructor with CRC presented as a vector, only the first 2 bytes are used.  And with the source address as a string in normal IP address format
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const std::vector<uint8_t> &crc, const std::string &src_address);
    //Constructor with CRC presented as a 16 bit int, and the source address as a string in normal IP address format
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const uint16_t &crc, const std::string &src_address);
    //constructor with CRC calculated from message, beacon, and pixel service values.  Source address as a string in normal IP address format
    HNS_NTCIP_MessageActivationCode(const uint16_t &duration, const uint8_t &priority, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                    const uint16_t &message_no, const std::string &message, const std::string &src_address,
                                    const uint8_t &beacon = 0, const uint8_t &pixel_svc = 0);

    void fSetOctetString(const std::vector<uint8_t> &octet_string);
    std::vector<uint8_t> fGetOctetString() const;

    void fSetDuration(const uint16_t &duration);
    uint16_t fGetDuration() const;

    void fSetPriority(const uint16_t &priority);
    uint8_t fGetPriority() const;

    void fSetMemoryType(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type);
    NTCIP_MESSAGE::type_ntcip_message_memory_type fGetMemoryType() const;

    void fSetMessageNo(const uint16_t &message_no);
    uint16_t fGetMessageNo() const;

    void fSetCRC(const std::vector<uint8_t> &crc);
    void fSetCRC(const uint16_t &crc);
    void fSetCRC(const std::string &message, const uint8_t &beacon, const uint8_t &pixel_svc);
    std::vector<uint8_t> fGetCRCAsVector() const;
    uint16_t fGetCRCAsInt() const;

    void fSetSrcAddr(const std::vector<uint8_t> &src_addr);
    void fSetSrcAddr(const std::string &src_addr);
    std::vector<uint8_t> fGetSrcAddrAsVec() const;
    std::string fGetSrcAddrAsString() const;
private:
    uint16_t f_duration;
    uint8_t f_priority;
    NTCIP_MESSAGE::type_ntcip_message_memory_type f_memory_type;
    uint16_t f_message_no;
    std::vector<uint8_t> f_crc;
    std::vector<uint8_t> f_src_address;
};

//class to store OID's and handle conversion to and from octet strings, and also to and from plain strings
class HNS_NTCIP_OID
{
public:
    HNS_NTCIP_OID();
    HNS_NTCIP_OID(const std::string &oid_string);
    HNS_NTCIP_OID(const std::vector<uint8_t> &oid_octet_string);

    void fSetData(const std::string &oid_string);
    void fSetData(const std::vector<uint8_t> &oid_octet_string);

    std::string fGetOIDString() const;
    std::vector<uint8_t> fGetOIDOctetString() const;

    //Returns true if this OID is a branch of the input OID.  That is to say, the sequence in the input is present at the beginning of the OID sequence contained in this object
    bool fIsBranchOf(const HNS_NTCIP_OID &oid) const;
private:
    //returns the OID as a series of unencoded integers.  Does not return a valid sequence for use in situations that requires OER
    std::vector<int> fGetOIDIntString() const;

    std::vector<uint8_t> f_oid_octet_string;
};

#endif // HNS_NTCIP_DATATYPES_H
