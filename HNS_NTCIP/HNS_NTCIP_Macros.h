#ifndef HNS_NTCIP_MACROS_H
#define HNS_NTCIP_MACROS_H

#include <vector>
#include <string>
#include "SNMP.h"
#include "HNS_NTCIP_Enums.h"

//Some handy functions to generate NTCIP commands for common applications

std::vector<SNMP_Packet> ChangeMessage(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                       const size_t &message_number,
                                       const std::string &multi,
                                       const std::string &owner,
                                       const unsigned char &priority,
                                       bool *error = nullptr);

std::vector<SNMP_Packet> ActivateMessage(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type,
                                         const size_t &message_number,
                                         const unsigned char &priority,
                                         const int &duration,
                                         bool *error = nullptr);

#endif // HNS_NTCIP_MACROS_H
