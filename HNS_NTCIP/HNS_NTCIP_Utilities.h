#ifndef HNS_NTCIP_UTILITIES_H
#define HNS_NTCIP_UTILITIES_H

#include "NTCIP.h"
#include "HNS_NTCIP_Enums.h"
#include "HNS_NTCIP_Datatypes.h"
#include <string>
#include <vector>
#include <graphics.h>

int CountMessages(NTCIP_Node *ntcip, const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type);
int CountGraphics(NTCIP_Node *ntcip);

bool IsSecurity(const HNS_NTCIP_OID &oid);

uint16_t CalculateMessageCRC(const std::string &message, const uint8_t &beacon, const uint8_t &pixel);

std::vector<unsigned char> GetBitmapDataStream(const HNS_Bitmap &bitmap, const NTCIP_GRAPHICS::type_ntcip_graphic_type &graphicType);

void CorrectTree(NTCIP_Node *ntcip);

#endif // HNS_NTCIP_UTILITIES_H
