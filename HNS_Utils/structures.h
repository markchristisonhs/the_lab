#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>
#include <typedefs.h>

typedef enum
{
    HNS_COMM_ETHERNET_SERIAL = 0,
    HNS_COMM_ETHERNET,
    HNS_COMM_SERIAL
} type_comm;

typedef enum
{
    HNS_COMM_STATIC = 0,
    HNS_COMM_DYNAMIC
} type_comm_address;

enum
{
    HNS_RADAR_NORMAL = 0,
    HNS_VSL_OFFSET
};

class HNS_Radar_Police_Speed
{
public:
    HNS_Radar_Police_Speed();

    HNS_Radar_Police_Speed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed, const int &speed_mode = HNS_RADAR_NORMAL);

    bool operator == (const HNS_Radar_Police_Speed &rhs) const;
    bool operator != (const HNS_Radar_Police_Speed &rhs) const;

    int fGetMinimumSpeed() const;
    void fSetMinimumSpeed(const int &minimum_speed);
    int fGetViolatorSpeed() const;
    void fSetViolatorSpeed(const int &violator_speed);
    int fGetMaximumSpeed() const;
    void fSetMaximumSpeed(const int &maximum_speed);

    int fGetMinimumSpeedOffset() const;
    int fGetViolatorSpeedOffset() const;
    int fGetMaximumSpeedOffSet() const;
    int fGetSpeedMode() const;

    void fSetMinimumSpeedOffset(const int &offset);
    void fSetViolatorSpeedOffset(const int &offset);
    void fSetMaximumSpeedOffset(const int &offset);
    void fSetSpeedMode(const int &mode);

    void fSetSpeed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed);
private:
    int f_minimum_speed,f_violator_speed,f_maximum_speed;
    int f_minimum_speed_offset,f_violator_speed_offset,f_maximum_speed_offset;

    int f_speed_mode;
};

class HNS_Radar_Strobe
{
public:
    HNS_Radar_Strobe();
    void fSetStrobeSettings(const int &duration, const int &pattern, const int &speed);
    void fSetDuration(const int &duration);
    void fSetPattern(const int &pattern);
    void fSetPatternUI(const int &pattern);
    void fSetSpeed(const int &speed);
    void fSetEnabled(const bool &enabled);
    void fSetSpeedMode(const int &speed_mode);
    void fSetSpeedOffset(const int &speed_offset);

    void fGetStrobeSettings(int &duration, int &pattern, int &speed) const;
    int fGetDuration() const;
    int fGetPattern() const;
    int fGetPatternUI() const;
    int fGetSpeed() const;
    bool fIsEnabled() const;
    int fGetSpeedMode() const {return f_speed_mode;}
    int fGetSpeedOffset() const {return f_speed_offset;}
private:
    int f_duration;
    int f_pattern;
    int f_speed;
    int f_speed_offset;
    bool f_enabled;
    int f_speed_mode;
};

//just a bucket to carry radar settings for convenience
class HNS_Radar_Settings
{
public:
    HNS_Radar_Settings();

    void fSetLegacyMessage(const std::string &message) {f_legacy_message = message;}
    std::string fGetLegacyMessage() const {return f_legacy_message;}

    void fSetUnderMinMessage(const std::string &message) {f_under_min_message = message;}
    std::string fGetUnderMinMessage() const {return f_under_min_message;}

    void fSetUnderVioMessage(const std::string &message) {f_under_vio_message = message;}
    std::string fGetUnderVioMessage() const {return f_under_vio_message;}

    void fSetOverVioMessage(const std::string &message) {f_over_vio_message = message;}
    std::string fGetOverVioMessage() const {return f_over_vio_message;}

    void fSetOverMaxMessage(const std::string &message) {f_over_max_message = message;}
    std::string fGetOverMaxMessage() const {return f_over_max_message;}

    void fSetPoliceMessages(const std::string &under_min_message,
                            const std::string &under_vio_message,
                            const std::string &over_vio_message,
                            const std::string &over_max_message);
    void fGetPoliceMessages(std::string &under_min_message,
                            std::string &under_vio_message,
                            std::string &over_vio_message,
                            std::string &over_max_message) const;

    void fSetLegacySpeed(const int &speed) {f_legacy_speed = speed;}
    int fGetLegacySpeed() const {return f_legacy_speed;}

    void fSetPoliceSpeed(const HNS_Radar_Police_Speed &speed) {f_police_speed = speed;}
    HNS_Radar_Police_Speed &fPoliceSpeed() {return f_police_speed;}
    HNS_Radar_Police_Speed fGetPoliceSpeed() const {return f_police_speed;}

    void fSetMode(const type_radar_mode &mode) {f_mode = mode;}
    type_radar_mode fGetMode() const {return f_mode;}

    void fSetUnits(const type_radar_units &units) {f_units = units;}
    type_radar_units fGetUnits() const {return f_units;}
private:
    std::string f_legacy_message;
    std::string f_under_min_message, f_under_vio_message, f_over_vio_message, f_over_max_message;

    int f_legacy_speed;
    HNS_Radar_Police_Speed f_police_speed;

    type_radar_mode f_mode;
    type_radar_units f_units;
};

class HNS_IP_Settings
{
public:
    HNS_IP_Settings();

    void fSetSettings(const int &ip, const int &gateway, const int &mask, const int &dns, const int &port, const type_comm &comm_type, const type_comm_address &address_type);
    void fSetSettings(const std::string &ip, const std::string &gateway, const std::string &mask, const std::string &dns, const int &port, const type_comm &comm_type, const type_comm_address &address_type);
    void fSetIpAddress(const int &ip);
    void fSetIpAddress(const std::string &ip);
    void fSetGateway(const int &gateway);
    void fSetGateway(const std::string &gateway);
    void fSetSubnetMask(const int &mask);
    void fSetSubnetMask(const std::string &mask);
    void fSetDNSServer(const int &dns);
    void fSetDNSServer(const std::string &dns);
    void fSetPort(const int &port);
    void fSetCommType( const type_comm &comm_type);
    void fSetAddressType(const type_comm_address &address_type);

    void fGetSettings(int &ip, int &gateway, int &mask, int &dns, int &port, type_comm &comm_type, type_comm_address &address_type) const;
    int fGetIpAddress() const;
    int fGetGateway() const;
    int fGetSubnetMask() const;
    int fGetDNSServer() const;
    int fGetPort() const;
    type_comm fGetCommType() const;
    type_comm_address fGetAddressType() const;

    static std::string fIptoString(const int &ip);
    static std::string fIPByteArraytoString(const std::vector<uint8_t> &ip);
    static std::vector<unsigned char> fIPStringtoByteArray(const std::string &ip);
private:
    int f_ip_address;
    int f_gateway;
    int f_subnet_mask;
    int f_dns_server;
    int f_port;
    type_comm f_comm_type;
    type_comm_address f_comm_adress_type;

    int fStringToIp(const std::string &ip_string);
};

class HNS_AuxSettings
{
public:
    HNS_AuxSettings();

    void fSetEnable(const int &index, const bool &enable);
    void fSetEnables(std::vector<bool> &enables);
    bool fGetEnable(const int &index) const;
    std::vector<bool> fGetEnables() const;

    void fSetDuration(const int &duration);
    int fGetDuration() const;
private:
    std::vector<bool> f_enable;
    int f_duration;
};

#endif // STRUCTURES_H
