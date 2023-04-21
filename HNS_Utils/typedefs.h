#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>

typedef enum
{
    HNS_MESSAGE_TYPE_FACTORY = 0,
    HNS_MESSAGE_TYPE_CUSTOM
} type_message_type;

typedef enum
{
    HNS_BRIGHTNESS_MANUAL=0,
    HNS_BRIGHTNESS_AUTO
} type_brightness_mode;

typedef enum
{
    HNS_RADAR_MODE_NORMAL = 0,
    HNS_RADAR_MODE_LEGACY,
    HNS_RADAR_MODE_POLICE
} type_radar_mode;

typedef enum
{
    HNS_RADAR_UNITS_MPH = 0,
    HNS_RADAR_UNITS_KPH
} type_radar_units;

struct type_speed_log_entry
{
    int64_t f_time;
    int f_speed;
};

typedef enum
{
    HNS_DATA_NOCHANGE = 0x0,
    HNS_DATA_FONT_CHANGE = 0x1,
    HNS_DATA_GRAPHIC_CHANGE = 0x2,
    HNS_DATA_RADAR_CHANGE = 0x4,
    HNS_DATA_ILLUMINATION_CHANGE = 0x8,
    HNS_DATA_CHARGER_CHANGE = 0x10,
    HNS_DATA_MESSAGE_CHANGE = 0x20,
    HNS_DATA_SCHEDULE_CHANGE = 0x40,
    HNS_DATA_PAGE_DEFAULT_CHANGE = 0x80,
    HNS_DATA_TRAILER_CHANGE = 0x100,
    HNS_DATA_NEW_PIX_OUT = 0x200,
    HNS_DATA_NEW_ACTIVATION = 0x400,
    HNS_DATA_NEW_GPIO = 0x800,
    HNS_DATA_ENTER_LVD = 0x1000,
    HNS_DATA_NEW_DISPLAY = 0x2000,
    HNS_DATA_ALL = 0xFFFFFFFF
} type_change_flags;

#endif // TYPEDEFS_H
