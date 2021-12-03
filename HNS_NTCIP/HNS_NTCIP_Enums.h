#ifndef HNS_NTCIP_ENUMS_H
#define HNS_NTCIP_ENUMS_H

#include <graphics.h>

namespace NTCIP_MESSAGE
{
typedef enum
{
    FONT_STATUS_NOT_USED=1,
    FONT_STATUS_MODIFYING=2,
    FONT_STATUS_CALCULATING_ID=3,
    FONT_STATUS_READY_FOR_USE=4,
    FONT_STATUS_IN_USE=5,
    FONT_STATUS_PERMANENT=6,
    FONT_STATUS_MODIFY_REQ=7,
    FONT_STATUS_READY_FOR_USE_REQ=8,
    FONT_STATUS_NOT_USED_REQ=9,
    FONT_STATUS_UNMANAGED_REQ=10,
    FONT_STATUS_UNMANAGED=11,
} type_ntcip_font_status_type;

typedef enum
{
    GRAPHIC_STATUS_NOT_USED=1,
    GRAPHIC_STATUS_MODIFYING=2,
    GRAPHIC_STATUS_CALCULATING_ID=3,
    GRAPHIC_STATUS_READY_FOR_USE=4,
    GRAPHIC_STATUS_IN_USE=5,
    GRAPHIC_STATUS_PERMANENT=6,
    GRAPHIC_STATUS_MODIFY_REQ=7,
    GRAPHIC_STATUS_READY_FOR_USE_REQ=8,
    GRAPHIC_STATUS_NOT_USED_REQ=9,
} type_ntcip_graphic_status_type;

typedef enum
{
    MEM_TYPE_PERMANENT=2,
    MEM_TYPE_CHANGEABLE=3,
    MEM_TYPE_VOLATILE=4,
    MEM_TYPE_CURRENT=5,
    MEM_TYPE_SCHEDULE=6,
    MEM_TYPE_BLANK=7
} type_ntcip_message_memory_type;

typedef enum
{
    STATUS_NOT_USED = 1,
    STATUS_MODIFYING = 2,
    STATUS_VALIDATING = 3,
    STATUS_VALID = 4,
    STATUS_ERROR = 5,
    STATUS_MODIFYREQ = 6,
    STATUS_VALIDATEREQ = 7,
    STATUS_NOT_USED_REQ = 8
} type_ntcip_message_status_type;

}

namespace NTCIP_GRAPHICS
{
typedef enum
{
    graphictype_monochrome1bit = 1,
    graphictype_monochrome8bit = 2,
    graphictype_colorclassic = 3,
    graphictype_color24bit = 4
} type_ntcip_graphic_type;

const HNS_Color gk_classic_black = HNS_Color(0,0,0);
const HNS_Color gk_classic_red = HNS_Color(255,0,0);
const HNS_Color gk_classic_yellow = HNS_Color(255,255,0);
const HNS_Color gk_classic_green = HNS_Color(0,255,0);
const HNS_Color gk_classic_cyan = HNS_Color(0,255,255);
const HNS_Color gk_classic_blue = HNS_Color(0,0,255);
const HNS_Color gk_classic_magenta = HNS_Color(255,0,255);
const HNS_Color gk_classic_white = HNS_Color(255,255,255);
const HNS_Color gk_classic_orange = HNS_Color(255,165,0);
const HNS_Color gk_classic_amber = HNS_Color(255,191,0);
}

namespace NTCIP_SNMP
{
typedef enum
{
    SNMP_ERR_noError = 0,
    SNMP_ERR_tooBig = 1,
    SNMP_ERR_noSuchName = 2,
    SNMP_ERR_badValue = 3,
    SNMP_ERR_readOnly = 4,
    SNMP_ERR_genErr = 5,
    SNMP_ERR_noAccess = 6,
    SNMP_ERR_wrongType = 7,
    SNMP_ERR_wrongLength = 8,
    SNMP_ERR_wrongEncoding = 9,
    SNMP_ERR_wrongValue = 10,
    SNMP_ERR_noCreation = 11,
    SNMP_ERR_inconsistentValue = 12,
    SNMP_ERR_resourceUnavailable = 13,
    SNMP_ERR_commitFailed = 14,
    SNMP_ERR_undoFailed = 15,
    SNMP_ERR_authorizationError = 16,
    SNMP_ERR_notWritable = 17,
    SNMP_ERR_inconsistentName = 18
} type_snmp_error;
}

#endif // HNS_NTCIP_ENUMS_H
