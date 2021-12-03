#include "HNS_NTCIP_Macros.h"

using namespace std;

vector<SNMP_Packet> ChangeMessage(const NTCIP_MESSAGE::type_ntcip_message_memory_type &/*memory_type*/,
                                  const size_t &/*message_number*/,
                                  const string &/*multi*/,
                                  const string &/*owner*/,
                                  const unsigned char &/*priority*/,
                                  bool *error)
{
    vector<SNMP_Packet> result;

    if(error != nullptr)
    {
        *error = false;
    }

    return result;
}

vector<SNMP_Packet> ActivateMessage(const NTCIP_MESSAGE::type_ntcip_message_memory_type &/*memory_type*/,
                                    const size_t &/*message_number*/,
                                    const unsigned char &/*priority*/,
                                    const int &/*duration*/,
                                    bool *error)
{
    vector<SNMP_Packet> result;

    if(error != nullptr)
    {
        *error = false;
    }

    return result;
}
