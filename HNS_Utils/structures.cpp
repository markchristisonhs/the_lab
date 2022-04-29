#include "structures.h"
#include <sstream>

using namespace std;

HNS_Radar_Police_Speed::HNS_Radar_Police_Speed():
 f_minimum_speed(20),f_violator_speed(55),f_maximum_speed(85),
 f_minimum_speed_offset(0),f_violator_speed_offset(5),f_maximum_speed_offset(10),f_speed_mode(HNS_RADAR_NORMAL)
{

}

HNS_Radar_Police_Speed::HNS_Radar_Police_Speed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed, const int &speed_mode)
{
    if(speed_mode == HNS_VSL_OFFSET)
    {
        f_minimum_speed_offset = minimum_speed;
        f_violator_speed_offset = violator_speed;
        f_maximum_speed_offset = maximum_speed;

        f_minimum_speed = 20;
        f_violator_speed = 55;
        f_maximum_speed = 85;

        f_speed_mode = speed_mode;
    }
    else
    {
        f_minimum_speed_offset = 0;
        f_violator_speed_offset = 5;
        f_maximum_speed_offset = 10;

        f_minimum_speed = minimum_speed;
        f_violator_speed = violator_speed;
        f_maximum_speed = maximum_speed;

        f_speed_mode = HNS_RADAR_NORMAL;
    }
}

bool HNS_Radar_Police_Speed::operator == (const HNS_Radar_Police_Speed &rhs) const
{
    return (f_minimum_speed == rhs.f_minimum_speed)
        && (f_violator_speed == rhs.f_violator_speed)
        && (f_maximum_speed == rhs.f_maximum_speed)
        && (f_minimum_speed_offset == rhs.f_minimum_speed_offset)
        && (f_violator_speed_offset == rhs.f_violator_speed_offset)
        && (f_maximum_speed_offset == rhs.f_maximum_speed_offset)
        && (f_speed_mode == rhs.f_speed_mode);
}

bool HNS_Radar_Police_Speed::operator != (const HNS_Radar_Police_Speed &rhs) const
{
    return !(*this == rhs);
}

int HNS_Radar_Police_Speed::fGetMinimumSpeed()
{
    return f_minimum_speed;
}

void HNS_Radar_Police_Speed::fSetMinimumSpeed(const int &minimum_speed)
{
    f_minimum_speed = minimum_speed;
}

int HNS_Radar_Police_Speed::fGetViolatorSpeed()
{
    return f_violator_speed;
}

void HNS_Radar_Police_Speed::fSetViolatorSpeed(const int &violator_speed)
{
    f_violator_speed = violator_speed;
}

int HNS_Radar_Police_Speed::fGetMaximumSpeed()
{
    return f_maximum_speed;
}

void HNS_Radar_Police_Speed::fSetMaximumSpeed(const int &maximum_speed)
{
    f_maximum_speed = maximum_speed;
}

int HNS_Radar_Police_Speed::fGetMinimumSpeedOffset() const
{
    return f_minimum_speed_offset;
}

int HNS_Radar_Police_Speed::fGetViolatorSpeedOffset() const
{
    return f_violator_speed_offset;
}

int HNS_Radar_Police_Speed::fGetMaximumSpeedOffSet() const
{
    return f_maximum_speed_offset;
}

int HNS_Radar_Police_Speed::fGetSpeedMode() const
{
    return f_speed_mode;
}

void HNS_Radar_Police_Speed::fSetMinimumSpeedOffset(const int &offset)
{
    f_minimum_speed_offset = offset;
}

void HNS_Radar_Police_Speed::fSetViolatorSpeedOffset(const int &offset)
{
    f_violator_speed_offset = offset;
}

void HNS_Radar_Police_Speed::fSetMaximumSpeedOffset(const int &offset)
{
    f_maximum_speed_offset = offset;
}

void HNS_Radar_Police_Speed::fSetSpeedMode(const int &mode)
{
    if(mode == HNS_RADAR_NORMAL || mode == HNS_VSL_OFFSET)
    {
        f_speed_mode = mode;
    }
}

void HNS_Radar_Police_Speed::fSetSpeed(const int &minimum_speed, const int &violator_speed, const int &maximum_speed)
{
    f_minimum_speed = minimum_speed;
    f_violator_speed = violator_speed;
    f_maximum_speed = maximum_speed;
}

HNS_Radar_Strobe::HNS_Radar_Strobe():
    f_duration(1),
    f_pattern(1),
    f_speed(15),
    f_speed_offset(0),
    f_enabled(false),
    f_speed_mode(HNS_RADAR_NORMAL)
{

}

void HNS_Radar_Strobe::fSetStrobeSettings(const int &duration, const int &pattern, const int &speed)
{
    fSetDuration(duration);
    fSetPattern(pattern);
    fSetSpeed(speed);
}

void HNS_Radar_Strobe::fSetDuration(const int &duration)
{
    if(duration > 9)
    {
        f_duration = 9;
    }
    else if(duration < 1)
    {
        f_duration = 1;
    }
    else
    {
        f_duration = duration;
    }
}

void HNS_Radar_Strobe::fSetPattern(const int &pattern)
{
    if(pattern > 3)
    {
        f_pattern = 3;
    }
    else if(pattern < 0)
    {
        f_pattern = 0;
    }
    else
    {
        f_pattern = pattern;
    }
}

void HNS_Radar_Strobe::fSetPatternUI(const int &pattern)
{
    fSetPattern(pattern-1);
}

void HNS_Radar_Strobe::fSetSpeed(const int &speed)
{
    if(speed > 95)
    {
        f_speed = 95;
    }
    else if(speed < 15)
    {
        f_speed = 15;
    }
    else
    {
        f_speed = speed;
    }
}

void HNS_Radar_Strobe::fSetEnabled(const bool &enabled)
{
    f_enabled = enabled;
}

void HNS_Radar_Strobe::fSetSpeedMode(const int &speed_mode)
{
    if(speed_mode == HNS_RADAR_NORMAL || speed_mode == HNS_VSL_OFFSET)
    {
        f_speed_mode = speed_mode;
    }
}

void HNS_Radar_Strobe::fSetSpeedOffset(const int &speed_offset)
{
    f_speed_offset = speed_offset;
}

void HNS_Radar_Strobe::fGetStrobeSettings(int &duration, int &pattern, int &speed) const
{
    duration = fGetDuration();
    pattern = fGetPattern();
    speed = fGetSpeed();
}

int HNS_Radar_Strobe::fGetDuration() const
{
    return f_duration;
}

int HNS_Radar_Strobe::fGetPattern() const
{
    return f_pattern;
}

int HNS_Radar_Strobe::fGetPatternUI() const
{
    return f_pattern+1;
}

int HNS_Radar_Strobe::fGetSpeed() const
{
    return f_speed;
}

bool HNS_Radar_Strobe::fIsEnabled() const
{
    return f_enabled;
}

HNS_IP_Settings::HNS_IP_Settings():
    f_ip_address(0),
    f_gateway(0),
    f_subnet_mask(0),
    f_dns_server(0),
    f_port(0),
    f_comm_type(HNS_COMM_ETHERNET_SERIAL),
    f_comm_adress_type(HNS_COMM_STATIC)
{

}

void HNS_IP_Settings::fSetSettings(const int &ip, const int &gateway, const int &mask, const int &dns, const int &port, const type_comm &comm_type, const type_comm_address &address_type)
{
    fSetIpAddress(ip);
    fSetGateway(gateway);
    fSetSubnetMask(mask);
    fSetDNSServer(dns);
    fSetPort(port);
    fSetCommType(comm_type);
    fSetAddressType(address_type);
}

void HNS_IP_Settings::fSetSettings(const std::string &ip, const std::string &gateway, const std::string &mask, const std::string &dns, const int &port, const type_comm &comm_type, const type_comm_address &address_type)
{
    fSetIpAddress(ip);
    fSetGateway(gateway);
    fSetSubnetMask(mask);
    fSetDNSServer(dns);
    fSetPort(port);
    fSetCommType(comm_type);
    fSetAddressType(address_type);
}

void HNS_IP_Settings::fSetIpAddress(const int &ip)
{
    f_ip_address = ip;
}

void HNS_IP_Settings::fSetIpAddress(const std::string &ip)
{
    fSetIpAddress(fStringToIp(ip));
}

void HNS_IP_Settings::fSetGateway(const int &gateway)
{
    f_gateway = gateway;
}

void HNS_IP_Settings::fSetGateway(const std::string &gateway)
{
    fSetGateway(fStringToIp(gateway));
}

void HNS_IP_Settings::fSetSubnetMask(const int &mask)
{
    f_subnet_mask = mask;
}

void HNS_IP_Settings::fSetSubnetMask(const std::string &mask)
{
    fSetSubnetMask(fStringToIp(mask));
}

void HNS_IP_Settings::fSetDNSServer(const int &dns)
{
    f_dns_server = dns;
}

void HNS_IP_Settings::fSetDNSServer(const std::string &dns)
{
    fSetDNSServer(fStringToIp(dns));
}

void HNS_IP_Settings::fSetPort(const int &port)
{
    f_port = port;
}

void HNS_IP_Settings::fSetCommType( const type_comm &comm_type)
{
    f_comm_type = comm_type;
}

void HNS_IP_Settings::fSetAddressType(const type_comm_address &address_type)
{
    f_comm_adress_type = address_type;
}

void HNS_IP_Settings::fGetSettings(int &ip, int &gateway, int &mask, int &dns, int &port, type_comm &comm_type, type_comm_address &address_type) const
{
    ip = fGetIpAddress();
    gateway = fGetGateway();
    mask = fGetSubnetMask();
    dns = fGetDNSServer();
    port = fGetPort();
    comm_type = fGetCommType();
    address_type = fGetAddressType();
}

int HNS_IP_Settings::fGetIpAddress() const
{
    return f_ip_address;
}

int HNS_IP_Settings::fGetGateway() const
{
    return f_gateway;
}

int HNS_IP_Settings::fGetSubnetMask() const
{
    return f_subnet_mask;
}

int HNS_IP_Settings::fGetDNSServer() const
{
    return f_dns_server;
}

int HNS_IP_Settings::fGetPort() const
{
    return f_port;
}

type_comm HNS_IP_Settings::fGetCommType() const
{
    return f_comm_type;
}

type_comm_address HNS_IP_Settings::fGetAddressType() const
{
    return f_comm_adress_type;
}

int HNS_IP_Settings::fStringToIp(const std::string &ip_string)
{
    int ip_address = 0;
    string tempstring = ip_string;

    ip_address = (std::stoi(tempstring.substr(0,tempstring.find_first_of('.'))) & 0xFF) << 24;
    tempstring = tempstring.substr(tempstring.find_first_of('.')+1);

    ip_address = ip_address | ((std::stoi(tempstring.substr(0,tempstring.find_first_of('.'))) & 0xFF) << 16);
    tempstring = tempstring.substr(tempstring.find_first_of('.')+1);

    ip_address = ip_address | ((std::stoi(tempstring.substr(0,tempstring.find_first_of('.'))) & 0xFF) << 8);
    tempstring = tempstring.substr(tempstring.find_first_of('.')+1);

    ip_address = ip_address | (std::stoi(tempstring) & 0xFF);

    return ip_address;
}

std::string HNS_IP_Settings::fIptoString(const int &ip)
{
    stringstream ss;

    ss << ((ip >> 24) & 0xFF) << "." << ((ip >> 16) & 0xFF) << "." << ((ip >> 8) & 0xFF) << "." << (ip & 0xFF);

    return ss.str();
}

string HNS_IP_Settings::fIPByteArraytoString(const vector<uint8_t> &ip)
{
    stringstream ss;
    size_t ilimit = min(ip.size(),static_cast<size_t>(4));

    for(size_t i=0;i<ilimit;i++)
    {
        ss << static_cast<unsigned int>(ip[i]);
        if(i < (ilimit-1))
        {
            ss << ".";
        }
    }

    return ss.str();
}

vector<unsigned char> HNS_IP_Settings::fIPStringtoByteArray(const string &ip)
{
    istringstream iss;
    iss.str(ip);
    string token;
    vector<unsigned char> result;

    while(getline(iss,token,'.'))
    {
        result.push_back(stoi(token));
    }

    return result;
}

HNS_AuxSettings::HNS_AuxSettings():
    f_enable(2,false),
    f_duration(10)
{

}

void HNS_AuxSettings::fSetEnable(const int &index, const bool &enable)
{
    if(index >= 0 && index <= static_cast<int>(f_enable.size()))
    {
        f_enable[static_cast<size_t>(index)] = enable;
    }
}

void HNS_AuxSettings::fSetEnables(std::vector<bool> &enables)
{
    size_t i;
    //the number of aux lines should not change
    for(i=0;i<f_enable.size();i++)
    {
        f_enable[i] = enables[i];
    }
}

bool HNS_AuxSettings::fGetEnable(const int &index) const
{
    if(index >= 0 && index <= static_cast<int>(f_enable.size()))
    {
        return f_enable[static_cast<size_t>(index)];
    }
    else
    {
        return false;
    }
}

vector<bool> HNS_AuxSettings::fGetEnables() const
{
    return f_enable;
}

void HNS_AuxSettings::fSetDuration(const int &duration)
{
    f_duration = duration;
}

int HNS_AuxSettings::fGetDuration() const
{
    return f_duration;
}
