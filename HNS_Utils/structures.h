#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

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

class HNS_Radar_Police_Speed
{
public:
    HNS_Radar_Police_Speed();

    HNS_Radar_Police_Speed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed);

    bool operator == (const HNS_Radar_Police_Speed &rhs) const;
    bool operator != (const HNS_Radar_Police_Speed &rhs) const;

    int fGetMinimumSpeed();
    void fSetMinimumSpeed(const int &minimum_speed);
    int fGetViolatorSpeed();
    void fSetViolatorSpeed(const int &violator_speed);
    int fGetMaximumSpeed();
    void fSetMaximumSpeed(const int &maximum_speed);

    void fSetSpeed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed);
private:
    int f_minimum_speed,f_violator_speed,f_maximum_speed;
};

class HNS_Radar_Strobe
{
public:
    HNS_Radar_Strobe();
    void fSetStrobeSettings(const int &duration, const int &pattern, const int &speed);
    void fSetDuration(const int &duration);
    void fSetPattern(const int &pattern);
    void fSetSpeed(const int &speed);
    void fSetEnabled(const bool &enabled);

    void fGetStrobeSettings(int &duration, int &pattern, int &speed) const;
    int fGetDuration() const;
    int fGetPattern() const;
    int fGetSpeed() const;
    bool fIsEnabled() const;
private:
    int f_duration;
    int f_pattern;
    int f_speed;
    bool f_enabled;
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
