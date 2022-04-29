#ifndef SNMP_H
#define SNMP_H

#include <vector>
#include <string>
#include <logaline.h>

typedef enum
{
    PMPP_PACKET = 0,
    PMPP_PACKET_INCOMPLETE,
    SNMP_PACKET,
    SNMP_PACKET_INCOMPLETE,
    INVALID_PACKET,
    UNKNOWN_PACKET
} type_packet;

typedef enum
{
    PDU_GETREQUEST = 0xA0,
    PDU_GETNEXTREQUEST = 0xA1,
    PDU_GETRESPONSE = 0xA2,
    PDU_SETREQUEST = 0xA3,
    PDU_TRAP = 0xA4
} type_pdu;

//it's confusing, but the PDU ID's are also data types.
typedef enum
{
    SNMP_INTEGER = 0x02,
    SNMP_OCTET_STRING = 0x04,
    SNMP_NULL_DATA = 0x05,
    SNMP_OID = 0x06,
    SNMP_SEQUENCE = 0x30,
    SNMP_IP_ADDRESS = 0x40,
    SNMP_COUNTER = 0x41,
    SNMP_GAUGE = 0x42,
    SNMP_TIMETICKS = 0x43,
    SNMP_OPAQUE = 0x44,
    SNMP_COUNTER64 = 0x46,
    SNMP_PDU_GETREQUEST = 0xA0,
    SNMP_PDU_GETNEXTREQUEST = 0xA1,
    SNMN_PDU_GETRESPONSE = 0xA2,
    SNMP_PDU_SETREQUEST = 0xA3,
    SNMP_PDU_TRAP = 0xA4
} type_snmp_data_types;

class PMPP_Packet;
class SNMP_Packet;
class SNMP_Data;
class SNMP_PDU_Data;
class SNMP_Variable_Bindings;

//class representing an input buffer. Could be PMPP, could be UDP, who knows?
class HNS_Buffer
{
public:
    HNS_Buffer();
    ~HNS_Buffer();

    void fAppendData(const std::vector<unsigned char> &input);
    void fClear();
    type_packet fGetPacketType() const;
    PMPP_Packet *fGetPacket();
    SNMP_Packet *fGetPacketAsSNMP();

private:
    void fAnalyzeBuffer();
    std::vector<unsigned char> f_data;
    PMPP_Packet *f_packet;
    SNMP_Packet *f_snmp_packet;
    type_packet f_packet_type;
};

class PMPP_Packet
{
public:
    PMPP_Packet();
    PMPP_Packet(const std::vector<unsigned char> &data, const int &address, const bool &address_group, const unsigned char &control, const unsigned char &ipi);
    PMPP_Packet(const std::vector<unsigned char> &data);

    //This returns the packet as a vector of unsigned chars
    std::vector<unsigned char> fGetRawData() const;

    void fSetData(const std::vector<unsigned char> &data);
    void fSetAddress(const int &address);
    void fSetAddressGroup(const bool &address_group);
    void fSetControl(const unsigned char &control);
    void fSetIPI(const unsigned char &ipi);

    int fGetAddress() const;
    bool fGetAddressGroup() const;
    unsigned char fGetControl() const;
    unsigned char fGetIPI() const;

    bool fIsComplete() const;
    bool fIsCorrupt() const;
    bool fIsSNMP() const;
    size_t fGetLastPosition() const;
    std::vector<unsigned char> fGetData();
private:
    void fValidatePacket(const std::vector<unsigned char> &input);
    std::vector<unsigned char> f_data;

    bool f_is_complete;
    bool f_is_corrupt;
    size_t f_last_position;

    int f_address;
    bool f_address_group;
    unsigned char f_control;
    unsigned char f_ipi;
};

//class representing a complete SNMP packet
class SNMP_Packet
{
public:
    SNMP_Packet();
    SNMP_Packet(const std::vector <unsigned char> &input_data, int *error = nullptr);

    //Returns the SNMP packet as a vector of bytes.  Mostly used when you want to send a packet
    std::vector<unsigned char> fGetRawData();

    void fSetPDUs(const std::vector<SNMP_PDU_Data> &input);
    void fSetCommunity(const std::string &community);
    void fSetVersion(const int &version);

    int fGetVersion() const;
    std::string fGetCommunity() const;
    size_t fGetNumPDUs() const;
    SNMP_PDU_Data fGetPDU(const size_t &index) const;

    void fLogPacket(HNS_LogALine2 *logaline);
private:
    int fParseInputData(const std::vector<unsigned char> &input_data);

    int f_version;
    std::string f_community_name;
    std::vector <SNMP_PDU_Data> f_data;
};

//basic SNMP TLV Structure
class SNMP_Data
{
public:
    SNMP_Data();
    SNMP_Data(const std::vector<unsigned char> &input, int &error_code);

    std::vector<unsigned char> fGetRawData() const;

    void fSetOctetData(const std::vector<unsigned char> &octetstring);
    void fSetOctetData(const std::string &octetstring);
    void fSetIntData(const int &input, const type_snmp_data_types &int_type = SNMP_INTEGER);
    void fSetNullData();
    void fSetOID(const std::string &oid);
    void fSetData(const std::vector<unsigned char> &input, const type_snmp_data_types &datatype);
    int fSetData(const std::vector<unsigned char> &input);

    type_snmp_data_types fGetDataType() const;
    std::vector<unsigned char> fGetData() const;
    size_t fGetDataSize() const;

    //This will turn this data chunk into a sequence.  It will erase the data and start a sequence
    void fPushSequenceItemToBack(const SNMP_Data &data);

    size_t fGetNumSequenceItems() const;
    long fGetInt() const;

    std::string fGetString() const;

    SNMP_Data fGetSequenceItem(const size_t &index) const;

    bool fIsPDU() const;

    void fLogData(HNS_LogALine2 *logaline);
private:

    std::string fGetDataTypeAsString() const;
    type_snmp_data_types f_datatype;
    std::vector<unsigned char> f_data;
    //if the datatype is a sequence, then this stores the items in the sequence.  This may contain further sequences, but those can
    //be retrieved as further SNMP_Data structures.
    //if this data block is not a sequence, this will always be empty
    std::vector<SNMP_Data> f_sequence_items;

    size_t fGetLength(const std::vector<unsigned char> &input, size_t &turtle, int &error, std::vector<unsigned char> *vector_output = nullptr);

    bool fIsSequence(const type_snmp_data_types &datatype);
};

class SNMP_PDU_Data
{
public:
    SNMP_PDU_Data();
    SNMP_PDU_Data(const std::vector<unsigned char> &input, int &error_code);
    SNMP_PDU_Data(const SNMP_Data &input, int &error_code);

    std::vector<unsigned char> fGetRawData() const;

    void fAddVarBinding(const SNMP_Variable_Bindings &input);
    void fSetPDUType(const type_pdu &pdu_type);
    void fSetRequestID(const int &id);
    void fSetErrorStatus(const int &error_state);
    void fSetErrorIndex(const int &error_index);

    type_pdu fGetPDUType() const;
    int fGetRequestID() const;
    int fGetErrorStatus() const;
    int fGetErrorIndex() const;
    size_t fGetNumVarBindings() const;
    SNMP_Variable_Bindings fGetVarBinding(const size_t &index) const;

    void fClearVarBindings();

    void fLogData(HNS_LogALine2 *logaline);
private:
    type_pdu f_pdu_type;
    int f_request_id;
    int f_error_status;
    int f_error_index;
    std::vector<SNMP_Variable_Bindings> f_data;
    std::string fGetPDUTypeAsString() const;
};

class SNMP_Variable_Bindings
{
public:
    SNMP_Variable_Bindings();
    SNMP_Variable_Bindings(const std::vector<unsigned char> &input, int &error_code);
    SNMP_Variable_Bindings(const SNMP_Data &input, int &error_code);
    SNMP_Variable_Bindings(const std::string &oid, const SNMP_Data &data);

    std::vector<unsigned char> fGetRawData() const;

    void fSetOID(const std::string &oid);
    void fSetData(const SNMP_Data &data);

    std::string fGetOID() const;
    SNMP_Data fGetData() const;

    void fLogBinding(HNS_LogALine2 *logaline);
private:
    std::string f_oid;
    SNMP_Data f_data;
};

#endif // SNMP_H
