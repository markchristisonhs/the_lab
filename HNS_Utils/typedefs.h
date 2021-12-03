#ifndef TYPEDEFS_H
#define TYPEDEFS_H

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

#endif // TYPEDEFS_H
