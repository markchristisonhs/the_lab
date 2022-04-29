#include "SNMP.h"
#include <utilities.h>
#include <CRC.h>
#include <HNS_NTCIP_Enums.h>
#include "BER.h"

#include <sstream>
#include <iomanip>

using namespace std;
using namespace NTCIP_SNMP;

string DataToOID(const std::vector<unsigned char> &input);

HNS_Buffer::HNS_Buffer():
    f_packet(nullptr),
    f_snmp_packet(nullptr),
    f_packet_type(UNKNOWN_PACKET)
{

}

HNS_Buffer::~HNS_Buffer()
{
    delete f_packet;
}

void HNS_Buffer::fAppendData(const std::vector<unsigned char> &input)
{
    for(size_t i = 0;i<input.size();i++)
    {
        f_data.push_back(input[i]);
    }

    fAnalyzeBuffer();
}

void HNS_Buffer::fClear()
{
    f_data.clear();
}

type_packet HNS_Buffer::fGetPacketType() const
{
    return f_packet_type;
}

PMPP_Packet *HNS_Buffer::fGetPacket()
{
    return f_packet;
}

SNMP_Packet *HNS_Buffer::fGetPacketAsSNMP()
{
    return f_snmp_packet;
}

void HNS_Buffer::fAnalyzeBuffer()
{
    int error;
    ssize_t packet_start = -1;
    f_packet_type = UNKNOWN_PACKET;
    //After a complete packet is found, it should be removed from the buffer.
    //So here we can assume there is no part of a prior packet left in the buffer
    //So let's look for the start of a buffer here.  For now, only looking for PMPP
    for(size_t i=0;i<f_data.size();i++)
    {
        //found the start
        if(f_data[i] == 0x7E)
        {
            f_packet_type = PMPP_PACKET_INCOMPLETE;
            packet_start = i;
            break;
        }
    }


    //if there is a random 7E somewhere in an SNMP packet it will think that it's a PMPP packet
    if(f_packet_type != PMPP_PACKET_INCOMPLETE)
    {
        for(size_t i=0;i<f_data.size();i++)
        {
            if(f_data[i] == 0x30)
            {
                f_packet_type = SNMP_PACKET_INCOMPLETE;
                packet_start = i;
                break;
            }
        }
    }
    //so if we think we have a PMPP packet check for a possible SNMP packet before the 7E.
    else
    {
        for(ssize_t i=0;i<packet_start;i++)
        {
            if(f_data[i] == 0x30)
            {
                f_packet_type = SNMP_PACKET_INCOMPLETE;
                packet_start = i;
                break;
            }
        }
    }

    //found the packet not at the beginning of the buffer, but somewhere in the middle.  Assume that all preceeding bytes are gibberish and discard
    if(packet_start > 0)
    {
        f_data.erase(f_data.begin(),f_data.begin()+packet_start);
    }
    //from here on, the first element should be 0x7E

    if(f_packet_type == PMPP_PACKET_INCOMPLETE)
    {
        if(f_packet != nullptr)
        {
            delete f_packet;
            f_packet = nullptr;
        }
        f_packet = new PMPP_Packet(f_data);
        if(f_packet->fIsComplete())
        {
            f_packet_type = PMPP_PACKET;
            //remove buffer that has been processed
            f_data.erase(f_data.begin(),f_data.begin()+f_packet->fGetLastPosition()+1);
        }
    }

    if(f_packet_type == SNMP_PACKET_INCOMPLETE)
    {
        if(f_snmp_packet != nullptr)
        {
            delete f_snmp_packet;
            f_snmp_packet = nullptr;
        }
        f_snmp_packet = new SNMP_Packet(f_data,&error);
        if(error == 0)
        {
            f_packet_type = SNMP_PACKET;
            //remove buffer that has been processed
            f_data.clear();
        }
    }
}

PMPP_Packet::PMPP_Packet():
    f_is_complete(false),
    f_is_corrupt(false),
    f_last_position(0),
    f_address(0),
    f_address_group(false),
    f_control(0),
    f_ipi(0)
{

}

PMPP_Packet::PMPP_Packet(const std::vector<unsigned char> &data, const int &address, const bool &address_group, const unsigned char &control, const unsigned char &ipi):
    f_data(data),
    f_is_complete(false),
    f_is_corrupt(false),
    f_last_position(0),
    f_address(address),
    f_address_group(address_group),
    f_control(control),
    f_ipi(ipi)
{

}

PMPP_Packet::PMPP_Packet(const std::vector<unsigned char> &data):
    f_is_complete(false),
    f_is_corrupt(false),
    f_last_position(0),
    f_address(0),
    f_address_group(false),
    f_control(0),
    f_ipi(0)
{
    fValidatePacket(data);
}

std::vector<unsigned char> PMPP_Packet::fGetRawData() const
{
    std::vector<unsigned char> result;
    unsigned char temp;
    uint16_t crc;

    //two byte address required
    if(f_address > 63)
    {
        temp = 0;
        temp = (f_address & 0x3F) << 2;
        if(f_address_group)
        {
            temp = temp | 0x2;
        }
        result.push_back(temp);

        temp = f_address >> 6;
        temp = temp << 1;
        temp = temp | 0x1;

        result.push_back(temp);
    }
    //single byte address
    else
    {
        temp = 0;
        temp = f_address << 2;
        temp = temp | 0x1;
        if(f_address_group)
        {
            temp = temp | 0x2;
        }
        result.push_back(temp);
    }

    result.push_back(f_control);
    result.push_back(f_ipi);
    result.insert(result.end(),f_data.begin(),f_data.end());

    crc = CalcCRC(result);
    result.push_back(static_cast<unsigned char>(crc & 0xFF));
    result.push_back(static_cast<unsigned char>((crc >> 8) & 0xFF));

    //now "decorate" the data.  Replace 0x7E with 0x7D5E and 0x7D with 0x7D5D
    for(size_t i=0;i<result.size();i++)
    {
        if(result[i] == 0x7E)
        {
            result[i] = 0x7D;
            result.emplace(result.begin()+i,0x5E);
        }
        else if(result[i] == 0x7D)
        {
            result.emplace(result.begin()+i,0x5D);
        }
    }

    //now to put in place the 0x7E "bookends"
    result.insert(result.begin(),0x7E);
    result.insert(result.end(),0x7E);

    return result;
}

void PMPP_Packet::fSetData(const std::vector<unsigned char> &data)
{
    f_data = data;
}

void PMPP_Packet::fSetAddress(const int &address)
{
    f_address = address;
}

void PMPP_Packet::fSetAddressGroup(const bool &address_group)
{
    f_address_group = address_group;
}

void PMPP_Packet::fSetControl(const unsigned char &control)
{
    f_control = control;
}

void PMPP_Packet::fSetIPI(const unsigned char &ipi)
{
    f_ipi = ipi;
}

int PMPP_Packet::fGetAddress() const
{
    return f_address;
}

bool PMPP_Packet::fGetAddressGroup() const
{
    return f_address_group;
}

unsigned char PMPP_Packet::fGetControl() const
{
    return f_control;
}

unsigned char PMPP_Packet::fGetIPI() const
{
    return f_ipi;
}

bool PMPP_Packet::fIsComplete() const
{
    return f_is_complete;
}

bool PMPP_Packet::fIsCorrupt() const
{
    return f_is_corrupt;
}

bool PMPP_Packet::fIsSNMP() const
{
    return (f_ipi == 0xC1);
}

size_t PMPP_Packet::fGetLastPosition() const
{
    return f_last_position;
}

std::vector<unsigned char> PMPP_Packet::fGetData()
{
    return f_data;
}

void PMPP_Packet::fValidatePacket(const std::vector<unsigned char> &input)
{
    size_t turtle = 0;
    bool found7d = false;
    unsigned char temp;
    int header_size = 0;
    uint16_t crc_from_input = 0,crc_calced = 0;
    bool found_end = false;

    if(input[turtle++] == 0x7E)
    {
        if(turtle < input.size())
        {
            //remember, data should include the header for now, for CRCC purposes
            temp = input[turtle++];
            f_data.push_back(temp);
            header_size++;
        }
        else
        {
            //not necessarily corrupt, just incomplete
            f_last_position = turtle-1;
            return;
        }

        f_address_group = ((temp & 0x2) == 1);
        if((temp & 0x1) == 0)
        //2 byte address field detected.
        {
            f_address = temp >> 2;
            if(turtle < input.size())
            {
                temp = input[turtle++];
                f_data.push_back(temp);
                header_size++;
            }
            else
            {
                f_last_position = turtle-1;
                return;
            }

            //bit 0 of address field should not be 0 for both LSB and MSB.  See NTCIP 2101 2.2.2 page 2-2
            if((temp & 0x1) == 0)
            {
                f_last_position = turtle;
                f_is_corrupt = true;
                return;
            }
            //shift right and discard the first bit.
            temp = temp >> 1;
            //merge the LSB with the MSB
            f_address = f_address | (temp << 6);
        }
        else
        //1 byte address field
        {
            f_address = temp >> 2;
        }
        //address 0 is invalid and address 63 is reserved for broadcast to all (0xFF)
        if(f_address == 0 || ((f_address == 63 ) && !f_address_group))
        {
            f_last_position = turtle;
            f_is_corrupt = true;
            return;
        }

        if(turtle < input.size())
        {
            f_control = input[turtle++];
            f_data.push_back(f_control);
            header_size++;
        }
        else
        {
            f_last_position = turtle-1;
            return;
        }
        if(turtle < input.size())
        {
            f_ipi = input[turtle++];
            f_data.push_back(f_ipi);
            header_size++;
        }
        else
        {
            f_last_position = turtle-1;
            return;
        }

        for(;turtle<input.size();turtle++)
        {
            if(found7d)
            {
                if(input[turtle] == 0x5E)
                {
                    f_data.push_back(0x7E);
                }
                else if(input[turtle] == 0x5D)
                {
                    f_data.push_back(0x7D);
                }
                else
                {
                    //7D must be followed by 5E or 5D
                    f_last_position = turtle;
                    f_is_corrupt = true;
                    return;
                }
            }
            else
            {
                if(input[turtle] == 0x7D)
                {
                    found7d = true;
                }
                else if(input[turtle] == 0x7E)
                {
                    //found the end
                    found_end = true;
                    f_last_position = turtle;
                    break;
                }
                else
                {
                    f_data.push_back(input[turtle]);
                }
            }
        }

        //verify CRC
        if(found_end)
        {
            crc_from_input = f_data[f_data.size()-2];
            crc_from_input = crc_from_input | (f_data[f_data.size()-1] << 8);

            crc_calced = CalcCRC(std::vector<unsigned char>(f_data.begin(),f_data.end()-2));

            if(crc_from_input == crc_calced)
            {
                //this is a valid PMPP packet.
                //remove the checksum, not really part of the data payload
                f_data.pop_back();
                f_data.pop_back();
                //remove the header, again not really part of the data payload
                f_data.erase(f_data.begin());
                f_data.erase(f_data.begin());
                f_data.erase(f_data.begin());
                f_last_position = turtle;
                f_is_complete = true;
            }
        }
    }
    else
    {
        f_is_corrupt = true;
    }
}

SNMP_Packet::SNMP_Packet()
{

}

SNMP_Packet::SNMP_Packet(const std::vector<unsigned char> &input_data, int *error)
{
    int temp;
    temp = fParseInputData(input_data);

    if(error != nullptr)
    {
        *error = temp;
    }
}

std::vector<unsigned char> SNMP_Packet::fGetRawData()
{
    std::vector <unsigned char> result, temp_vec;
    SNMP_Data temp_data;

    temp_data.fSetIntData(f_version);
    temp_vec = temp_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    temp_data.fSetOctetData(f_community_name);
    temp_vec = temp_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(), temp_vec.end());

    for(size_t i=0;i<f_data.size();i++)
    {
        temp_vec = f_data[i].fGetRawData();
        result.insert(result.end(),temp_vec.begin(), temp_vec.end());
    }

    temp_vec = ConvSizeT_to_Legnth(result.size());
    result.insert(result.begin(),temp_vec.begin(),temp_vec.end());

    result.insert(result.begin(),0x30);

    return result;
}

void SNMP_Packet::fSetPDUs(const std::vector<SNMP_PDU_Data> &input)
{
    f_data = input;
}

void SNMP_Packet::fSetCommunity(const std::string &community)
{
    f_community_name = community;
}

void SNMP_Packet::fSetVersion(const int &version)
{
    f_version = version;
}

int SNMP_Packet::fGetVersion() const
{
    return f_version;
}

std::string SNMP_Packet::fGetCommunity() const
{
    return f_community_name;
}

size_t SNMP_Packet::fGetNumPDUs() const
{
    return f_data.size();
}

SNMP_PDU_Data SNMP_Packet::fGetPDU(const size_t &index) const
{
    if(index < f_data.size())
    {
        return f_data[index];
    }
    else
    {
        return SNMP_PDU_Data();
    }
}

void SNMP_Packet::fLogPacket(HNS_LogALine2 *logaline)
{
    stringstream ss;
    string caller = "SNMP_Packet::fLogPacket";

    if(logaline != nullptr)
    {
        ss.str("");
        ss << "Packet has version of " << fGetVersion() << " and a community string of " << fGetCommunity();
        logaline->fLogALine(ss.str(),caller);

        ss.str("");
        ss << "Packet contains " << fGetNumPDUs() << " PDUs";
        logaline->fLogALine(ss.str(),caller);

        for(size_t ui=0;ui<f_data.size();ui++)
        {
            f_data[ui].fLogData(logaline);
        }
    }
}

int SNMP_Packet::fParseInputData(const std::vector<unsigned char> &input_data)
{
    int error = 0;

    SNMP_Data top_level_sequence(input_data,error);
    SNMP_PDU_Data temp;

    if(error == 0)
    {
        //as defined in RFC 1157 Section 4, the top level MUST be a sequence and MUST contain at least 3 sets of data;
        //the version number, followed by the community name, followed by any supporting data.
        //Data must be in that order
        if((top_level_sequence.fGetDataType() == SNMP_SEQUENCE)
        && (top_level_sequence.fGetNumSequenceItems() >= 3))
        {
            //check for version number at the top
            if(top_level_sequence.fGetSequenceItem(0).fGetDataType() == SNMP_INTEGER)
            {
                f_version = top_level_sequence.fGetSequenceItem(0).fGetInt();
            }
            else
            {
                error = 1;
            }

            //check for community string
            if(top_level_sequence.fGetSequenceItem(1).fGetDataType() == SNMP_OCTET_STRING)
            {
                f_community_name = top_level_sequence.fGetSequenceItem(1).fGetString();
            }
            else
            {
                error = 1;
            }

            //and now for PDUs.  I believe it is possible for there to be multiple PDUs
            for(size_t i =2;i<top_level_sequence.fGetNumSequenceItems();i++)
            {
                if(top_level_sequence.fGetSequenceItem(i).fIsPDU())
                {
                    temp = SNMP_PDU_Data(top_level_sequence.fGetSequenceItem(i), error);
                    f_data.push_back(temp);
                }
                else
                {
                    error = 1;
                    break;
                }
            }
        }
        else
        {
            error = 1;
        }
    }

    return error;
}

SNMP_Data::SNMP_Data():
    f_datatype(SNMP_NULL_DATA)
{

}

SNMP_Data::SNMP_Data(const std::vector<unsigned char> &input, int &error_code)
{
    error_code = fSetData(input);
}

std::vector<unsigned char> SNMP_Data::fGetRawData() const
{
    std::vector<unsigned char> result,temp_vec;

    result.push_back(static_cast<unsigned char>(f_datatype));
    temp_vec = ConvSizeT_to_Legnth(f_data.size());
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());
    for(size_t i=0;i<f_data.size();i++)
    {
        result.push_back(f_data[i]);
    }

    return result;
}

void SNMP_Data::fSetOctetData(const std::vector<unsigned char> &octetstring)
{
    f_sequence_items.clear();
    f_data.clear();
    f_data = octetstring;
    f_datatype = SNMP_OCTET_STRING;
}

void SNMP_Data::fSetOctetData(const std::string &octetstring)
{
    f_sequence_items.clear();
    f_data.clear();
    for(size_t i=0;i<octetstring.size();i++)
    {
        f_data.push_back(octetstring[i]);
    }
    f_datatype = SNMP_OCTET_STRING;
}

void SNMP_Data::fSetIntData(const int &input, const type_snmp_data_types &int_type)
{
    f_sequence_items.clear();
    f_data.clear();

//    f_data = HNS_intToByteArray(input);
//    //lop off all trailing zeroes to minimize the size
//    while(f_data.size() > 1)
//    {
//        if(f_data[0] == 0x00)
//        {
//            f_data.erase(f_data.begin());
//        }
//        else
//        {
//            break;
//        }
//    }
    f_data = BER_Encode_Int(input);
    if((int_type == SNMP_INTEGER)
    || (int_type == SNMP_COUNTER)
    || (int_type == SNMP_GAUGE)
    || (int_type == SNMP_TIMETICKS))
    {
        f_datatype = int_type;
    }
    else
    {
        f_datatype = SNMP_INTEGER;
    }
}

void SNMP_Data::fSetNullData()
{
    f_datatype = SNMP_NULL_DATA;
    f_data.clear();
}

void SNMP_Data::fSetOID(const std::string &oid)
{
    std::istringstream input(oid);
    int n;
    char dummy;
    unsigned char temp;
    std::vector<unsigned char> temp_vec;

    f_data.clear();
    f_sequence_items.clear();
    int i=0,j;
    temp = 0x0;
    f_datatype = SNMP_OID;
    do
    {
        input >> n;

        //not at the end, strip the '.'
        if(input.tellg() != -1)
        {
            input >> dummy;
        }

        if(i == 0)
        {
            temp = n * 40;
        }
        else if(i == 1)
        {
            temp += n;
            f_data.push_back(temp);
        }
        else
        {
            if(n <= 0x7F)
            {
                f_data.push_back(n);
            }
            else
            {
                temp_vec.clear();
                j = 0;
                while(n != 0)
                {
                    temp = static_cast<unsigned char>(n & 0x7F);
                    if(j > 0)
                    {
                        temp = temp | 0x80;
                    }
                    temp_vec.insert(temp_vec.begin(),temp);
                    n = n >> 7;
                    j++;
                }
                f_data.insert(f_data.end(),temp_vec.begin(),temp_vec.end());
            }
        }
        i++;
    }while(input.tellg() != -1);
}

void SNMP_Data::fSetData(const std::vector<unsigned char> &input, const type_snmp_data_types &datatype)
{
    f_data = input;
    f_datatype = datatype;
}

int SNMP_Data::fSetData(const std::vector<unsigned char> &input)
{
    int error = 0;
    size_t turtle = 0;
    size_t length = 0, sub_length = 0;
    if(input.size() > 0)
    {
        f_data.clear();
        f_sequence_items.clear();
        //first byte should be the data type
        f_datatype = static_cast<type_snmp_data_types>(input[turtle++]);

        //then the size
        length = fGetLength(input,turtle,error);

        //then the data
        if(input.size() >= (turtle + length))
        {
            //if the data is a sequence, then store the sequences as a series of SNMP_Data strutctures
            if(fIsSequence(f_datatype))
            {
                do
                {
                    //going to abuse f_data as a temporary buffer for sub units.
                    f_data.push_back(input[turtle++]);
                    sub_length = fGetLength(input,turtle,error,&f_data);
                    for(size_t i = 0; i < sub_length; i++)
                    {
                        f_data.push_back(input[turtle++]);
                    }
                    f_sequence_items.push_back(SNMP_Data(f_data,error));
                    f_data.clear();
                }while(turtle < input.size());
            }
            else
            {
                for(size_t i = 0; i < length; i++)
                {
                    f_data.push_back(input[turtle++]);
                }
            }
        }
        else
        {
            error = 1;
        }
    }

    return error;
}

type_snmp_data_types SNMP_Data::fGetDataType() const
{
    return f_datatype;
}

string SNMP_Data::fGetDataTypeAsString() const
{
    string result = "unkown";

    switch(f_datatype)
    {
    case SNMP_INTEGER:
        result = "Integer";
        break;
    case SNMP_OCTET_STRING:
        result = "Octet String";
        break;
    case SNMP_NULL_DATA:
        result = "Null Data";
        break;
    case SNMP_OID:
        result = "OID";
        break;
    case SNMP_SEQUENCE:
        result = "Sequence";
        break;
    case SNMP_IP_ADDRESS:
        result = "IP Address";
        break;
    case SNMP_COUNTER:
        result = "Counter";
        break;
    case SNMP_GAUGE:
        result = "Gauge";
        break;
    case SNMP_TIMETICKS:
        result = "TimeTicks";
        break;
    case SNMP_OPAQUE:
        result = "Opaque";
        break;
    case SNMP_COUNTER64:
        result = "Counter64";
        break;
    case SNMP_PDU_GETREQUEST:
        result = "unkown";
        break;
    case SNMP_PDU_GETNEXTREQUEST:
        result = "unkown";
        break;
    case SNMN_PDU_GETRESPONSE:
        result = "unkown";
        break;
    case SNMP_PDU_SETREQUEST:
        result = "unkown";
        break;
    case SNMP_PDU_TRAP:
        result = "unkown";
        break;
    }

    return result;
}

std::vector<unsigned char> SNMP_Data::fGetData() const
{
    return f_data;
}

size_t SNMP_Data::fGetDataSize() const
{
    return f_data.size();
}

void SNMP_Data::fPushSequenceItemToBack(const SNMP_Data &data)
{
    f_data.clear();
    f_datatype = SNMP_SEQUENCE;

    f_sequence_items.push_back(data);
}

size_t SNMP_Data::fGetNumSequenceItems() const
{
    return f_sequence_items.size();
}

bool SNMP_Data::fIsPDU() const
{
    return (f_datatype >= SNMP_PDU_GETREQUEST) && (f_datatype <= SNMP_PDU_TRAP);
}

void SNMP_Data::fLogData(HNS_LogALine2 *logaline)
{
    stringstream ss;
    string caller = "SNMP_Data::fLogData";
    bool octet_ok = false;
    bool printable = true;

    if(logaline != nullptr)
    {
        ss.str("");
        ss << "Data type is " << fGetDataTypeAsString();
        logaline->fLogALine(ss.str(),caller);

        if(fGetDataType() != SNMP_NULL_DATA)
        {
            ss.str("");
//            ss << "Data is 0x"<< setfill('0') << setw(2) << hex;
            ss << "Data is ";
            if(fGetDataType() == SNMP_OCTET_STRING)
            {
                for(size_t ui = 0; ui < f_data.size();ui++)
                {
                    if(isprint(f_data[ui]) == 0)
                    {
                        printable = false;
                        break;
                    }
                }
                if(printable)
                {
                    octet_ok = true;

                    ss << string(f_data.begin(),f_data.end());
                }
            }
            if(!octet_ok)
            {
                ss.str("");
                ss << "0x"<< setfill('0') << setw(2) << hex;
                for(size_t ui = 0; ui < f_data.size();ui++)
                {
                    ss << (unsigned)f_data[ui];
                }
            }
        }
        logaline->fLogALine(ss.str(),caller);
    }
}

long SNMP_Data::fGetInt() const
{
    long result = 0;
    size_t i,ilimit;

    if(f_datatype == SNMP_INTEGER)
    {
        if((f_data[0] & 0x80) != 0)
        {
            result = -1;
        }
        //assume the integer is not greater than 64 bit;
        ilimit = std::min(f_data.size(),static_cast<size_t>(8));
        {
            for(i=0;i<ilimit;i++)
            {
                result = result << 8;
                result += f_data[i];
            }
        }
    }

    return result;
}

std::string SNMP_Data::fGetString() const
{
    if(f_datatype == SNMP_OCTET_STRING)
    {
        return std::string(f_data.begin(),f_data.end());
    }
    else
    {
        return std::string("");
    }
}

SNMP_Data SNMP_Data::fGetSequenceItem(const size_t &index) const
{
    if(index < f_sequence_items.size())
    {
        return f_sequence_items[index];
    }
    else
    {
        return SNMP_Data();
    }
}

size_t SNMP_Data::fGetLength(const std::vector<unsigned char> &input, size_t &turtle, int &error, std::vector<unsigned char> *vector_output)
{
    size_t length = 0,length_of_length;

    if(input.size() > turtle)
    {
        //bit 7 is 0, therefore the length is only one byte, encoded in bits 6 to 0
        if(input[turtle] < 0x80)
        {
            if(vector_output != nullptr)
            {
                vector_output->push_back(input[turtle]);
            }
            length = input[turtle++];
        }
        //bit 7 is 1, therefore length is multibyte.  Bits 6 to 0 encode the number of bytes constituting the length.  The following
        //bytes are the length constituted in a big endian fashion.
        else
        {
            //MSC20210626 don't forget to grab the length of length byte!!!
            if(vector_output != nullptr)
            {
                vector_output->push_back(input[turtle]);
            }
            length_of_length = (input[turtle++] & 0x7F);
            if(input.size() >= (turtle + length_of_length))
            {
                for(size_t i = 0; i < length_of_length; i++)
                {
                    if(vector_output != nullptr)
                    {
                        vector_output->push_back(input[turtle]);
                    }
                    length = length << 8;
                    length += input[turtle++];
                }
            }
            else
            {
                error = 1;
            }
        }
    }
    else
    {
        error = 1;
    }

    return length;
}

bool SNMP_Data::fIsSequence(const type_snmp_data_types &datatype)
{
    return (datatype >= SNMP_SEQUENCE);
}

SNMP_PDU_Data::SNMP_PDU_Data():
    f_pdu_type(PDU_GETRESPONSE),
    f_request_id(0),
    f_error_status(SNMP_ERR_noError),
    f_error_index(0)
{

}

SNMP_PDU_Data::SNMP_PDU_Data(const std::vector<unsigned char> &input, int &error_code)
{
    SNMP_PDU_Data(SNMP_Data(input,error_code),error_code);
}

SNMP_PDU_Data::SNMP_PDU_Data(const SNMP_Data &input, int &error_code)
{
    SNMP_Data temp;

    f_pdu_type = (type_pdu)input.fGetDataType();
    //by definition, the PDU must contain 4 fields;
    //request-id,error state, error index, and varbindlist
    if(input.fGetNumSequenceItems() == 4)
    {
        //get request-id
        if(input.fGetSequenceItem(0).fGetDataType() == SNMP_INTEGER)
        {
            f_request_id = input.fGetSequenceItem(0).fGetInt();
        }
        else
        {
            error_code = 1;
        }

        //get error state
        if(input.fGetSequenceItem(1).fGetDataType() == SNMP_INTEGER)
        {
            f_error_status = input.fGetSequenceItem(1).fGetInt();
        }
        else
        {
            error_code = 1;
        }

        //get error index
        if(input.fGetSequenceItem(2).fGetDataType() == SNMP_INTEGER)
        {
            f_error_index = input.fGetSequenceItem(2).fGetInt();
        }
        else
        {
            error_code = 1;
        }

        //now to grab the variable bindings.  According to RFC 1157, there is only one variable bindings list, which is
        //a sequence (0x30) of variable bindings, which are in turn sequences.
        if(input.fGetSequenceItem(3).fGetDataType() == SNMP_SEQUENCE)
        {
            for(size_t i=0;i<input.fGetSequenceItem(3).fGetNumSequenceItems();i++)
            {
                temp = input.fGetSequenceItem(3).fGetSequenceItem(i);
                f_data.push_back(SNMP_Variable_Bindings(temp,error_code));
            }
        }
        else
        {
            error_code = 1;
        }
    }
    else
    {
        error_code = 1;
    }
}

std::vector<unsigned char> SNMP_PDU_Data::fGetRawData() const
{
    std::vector<unsigned char> result, temp_vec, temp_vec2, temp_vec3;
    SNMP_Data temp_data;

    temp_data.fSetIntData(f_request_id);
    temp_vec = temp_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    temp_data.fSetIntData(f_error_status);
    temp_vec = temp_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    temp_data.fSetIntData(f_error_index);
    temp_vec = temp_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    for(size_t i=0;i<f_data.size();i++)
    {
        temp_vec = f_data[i].fGetRawData();
        temp_vec2.insert(temp_vec2.end(),temp_vec.begin(),temp_vec.end());
    }

    result.insert(result.end(),0x30);
    temp_vec3 = ConvSizeT_to_Legnth(temp_vec2.size());
    result.insert(result.end(),temp_vec3.begin(),temp_vec3.end());
    result.insert(result.end(),temp_vec2.begin(),temp_vec2.end());

    temp_vec3 = ConvSizeT_to_Legnth(result.size());
    result.insert(result.begin(),temp_vec3.begin(),temp_vec3.end());

    result.insert(result.begin(),static_cast<unsigned char>(f_pdu_type));

    return result;
}

void SNMP_PDU_Data::fAddVarBinding(const SNMP_Variable_Bindings &input)
{
    f_data.push_back(input);
}

void SNMP_PDU_Data::fSetPDUType(const type_pdu &pdu_type)
{
    f_pdu_type = pdu_type;
}

void SNMP_PDU_Data::fSetRequestID(const int &id)
{
    f_request_id = id;
}

void SNMP_PDU_Data::fSetErrorStatus(const int &error_state)
{
    f_error_status = error_state;
}

void SNMP_PDU_Data::fSetErrorIndex(const int &error_index)
{
    f_error_index = error_index;
}

type_pdu SNMP_PDU_Data::fGetPDUType() const
{
    return f_pdu_type;
}

string SNMP_PDU_Data::fGetPDUTypeAsString() const
{
    string result = "unkown";

    switch(f_pdu_type)
    {
    case PDU_GETREQUEST:
        result = "Get Request";
        break;
    case PDU_GETNEXTREQUEST:
        result = "Get Next Request";
        break;
    case PDU_GETRESPONSE:
        result = "Get Response";
        break;
    case PDU_SETREQUEST:
        result = "Set Request";
        break;
    case PDU_TRAP:
        result = "trap";
        break;
    }

    return result;
}

int SNMP_PDU_Data::fGetRequestID() const
{
    return f_request_id;
}

int SNMP_PDU_Data::fGetErrorStatus() const
{
    return f_error_status;
}

int SNMP_PDU_Data::fGetErrorIndex() const
{
    return f_error_index;
}

size_t SNMP_PDU_Data::fGetNumVarBindings() const
{
    return f_data.size();
}

SNMP_Variable_Bindings SNMP_PDU_Data::fGetVarBinding(const size_t &index) const
{
    if(index < f_data.size())
    {
        return f_data[index];
    }
    else
    {
        return SNMP_Variable_Bindings();
    }
}

void SNMP_PDU_Data::fClearVarBindings()
{
    f_data.clear();
}

void SNMP_PDU_Data::fLogData(HNS_LogALine2 *logaline)
{
    stringstream ss;
    string caller = "SNMP_PDU_Data::fLogData";

    if(logaline != nullptr)
    {
        ss.str("");
        ss << "This PDU has a type of " << fGetPDUTypeAsString() << ", a request ID of " << fGetRequestID() << ", error status of " << fGetErrorStatus() << ", error index of " << fGetErrorIndex() << ", and number of bindings: " << f_data.size();
        logaline->fLogALine(ss.str(),caller);

        for(size_t ui=0;ui<f_data.size();ui++)
        {
            f_data[ui].fLogBinding(logaline);
        }
    }
}

SNMP_Variable_Bindings::SNMP_Variable_Bindings()
{

}

SNMP_Variable_Bindings::SNMP_Variable_Bindings(const std::vector<unsigned char> &input, int &error_code)
{
    SNMP_Variable_Bindings(SNMP_Data(input,error_code),error_code);
}

SNMP_Variable_Bindings::SNMP_Variable_Bindings(const SNMP_Data &input, int &error_code)
{
    //Variable bindings consist of a single OID and one piece of data.
    if(input.fGetNumSequenceItems() == 2)
    {
        //grab OID
        if(input.fGetSequenceItem(0).fGetDataType() == SNMP_OID)
        {
            f_oid = DataToOID(input.fGetSequenceItem(0).fGetData());
        }
        else
        {
            error_code = 1;
        }

        f_data = input.fGetSequenceItem(1);
    }
    else
    {
        error_code = 1;
    }
}

SNMP_Variable_Bindings::SNMP_Variable_Bindings(const std::string &oid, const SNMP_Data &data)
{
    f_oid = oid;
    f_data = data;
}

std::vector<unsigned char> SNMP_Variable_Bindings::fGetRawData() const
{
    std::vector<unsigned char> result,temp_vec;
    SNMP_Data oid_data;

    oid_data.fSetOID(f_oid);
    temp_vec = oid_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    temp_vec = f_data.fGetRawData();
    result.insert(result.end(),temp_vec.begin(),temp_vec.end());

    temp_vec = ConvSizeT_to_Legnth(result.size());
    result.insert(result.begin(),temp_vec.begin(),temp_vec.end());

    result.insert(result.begin(),0x30);

    return result;
}

void SNMP_Variable_Bindings::fSetOID(const std::string &oid)
{
    f_oid = oid;
}

void SNMP_Variable_Bindings::fSetData(const SNMP_Data &data)
{
    f_data = data;
}

std::string SNMP_Variable_Bindings::fGetOID() const
{
    return f_oid;
}

SNMP_Data SNMP_Variable_Bindings::fGetData() const
{
    return f_data;
}

void SNMP_Variable_Bindings::fLogBinding(HNS_LogALine2 *logaline)
{
    stringstream ss;
    string caller = "SNMP_Variable_Bindings::fLogBinding";

    if(logaline != nullptr)
    {
        ss.str("");
        ss << "This binding has an OID of " << fGetOID();
        logaline->fLogALine(ss.str(),caller);

        f_data.fLogData(logaline);
    }
}

std::string DataToOID(const std::vector<unsigned char> &input)
{
    std::string result;
    std::stringstream ss;
    bool found_multibyte;

    size_t turtle = 0;
    int temp1,temp2;

    //first byte is odd, as per IEC 8825-1:2015 section 8.19.4
    temp1 = input[turtle] / 40;
    temp2 = input[turtle++] % 40;

    ss << temp1 << "." << temp2;

    temp1 = 0;
    temp2 = 0;
    found_multibyte = false;
    while(turtle < input.size())
    {
        temp1 = input[turtle++];
        if(temp1 & 0x80)
        {
            temp2 = temp2 << 7;
            temp2 += (temp1 % 0x80);
            found_multibyte = true;
        }
        else
        {
            if(found_multibyte)
            {
                temp2 = temp2 << 7;
                temp2 += (temp1 % 0x80);
                ss << "." << temp2;
                found_multibyte = false;
            }
            else
            {
                ss << "." << temp1;
            }
        }
    }

    result = ss.str();

    return result;
}
