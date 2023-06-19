#ifndef HNS_MACHINE_INTERFACE_H
#define HNS_MACHINE_INTERFACE_H

#include <vector>
#include <fonts.h>
#include <graphics.h>
#include <message.h>
#include <string>
#include <HNS_NTCIP_Enums.h>
#include <HNS_NTCIP_Datatypes.h>
#include <security.h>
#include <HNS_schedule.h>
#include <structures.h>
#include <hns_message_info.h>
#include <HNS_GPIO.h>

class HNS_Machine_Interface
{
public:
    virtual type_user_level fGetAccess() const = 0;

    virtual std::vector<HNS_Font> *fGetFonts() = 0;
    virtual std::vector<HNS_Graphic> *fGetGraphics() = 0;
    virtual HNS_Field_Data *fGetFieldData() = 0;

    virtual size_t fGetNumMessages(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const std::string &searchString) const = 0;

    virtual std::string fGetMessage(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const size_t &index, const std::string &searchString = "", HNS_NTCIP_MessageActivationCode *code = nullptr, const int &sort_mode = HNS_MESSAGE_SORT_NONE) const = 0;
    virtual std::string fGetMessage(const HNS_NTCIP_MessageIDCode &id_code, HNS_NTCIP_MessageActivationCode *act_code = nullptr) const = 0;
    virtual HNS_Message2 fGetMessage2(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const size_t &index, const std::string &searchString = "", HNS_NTCIP_MessageActivationCode *code = nullptr, const int &sort_mode = HNS_MESSAGE_SORT_NONE) const = 0;
    virtual HNS_NTCIP_MessageActivationCode fGetMessageActivation(const NTCIP_MESSAGE::type_ntcip_message_memory_type &memory_type, const size_t &index) = 0;
    virtual std::vector<HNS_Message_Info> fGetMessageLog() const = 0;

    virtual HNS_Graphic fGetGraphicByIndex(const size_t &index) const = 0;

    virtual std::vector < std::vector <int> > *fGetAnimations() = 0;
    virtual int fGetChangableIndex(HNS_Message2 &msg) = 0;

    virtual std::vector<HNS_Schedule> fGetSchedules() const = 0;

    virtual type_brightness_mode fGetBrightnessMode() const = 0;
    virtual int fGetLastPhotoCellValue() const = 0;
    virtual int fGetBrightnessLevel() const = 0;
    virtual bool fGetPhotocellConnected() const = 0;

    virtual int fGetLastSpeed() const = 0;
    virtual int fGetCurrentSpeed() const = 0;
    virtual bool fGetRadarConnected() const = 0;
    virtual std::vector<type_speed_log_entry> fGetSpeedLog() const = 0;
    virtual type_radar_mode fGetRadarMode() const = 0;
    virtual type_radar_units fGetRadarUnits() const = 0;
    virtual int fGetRadarLegacySpeed() const = 0;
    virtual HNS_Message2 fGetLegacyMessage() = 0;
    virtual std::string fGetLegacyMessageAsMulti() = 0;
    virtual HNS_Message2 fGetUnderMinMessage() = 0;
    virtual std::string fGetUnderMinMessageAsMulti() = 0;
    virtual HNS_Message2 fGetUnderVioMessage() = 0;
    virtual std::string fGetUnderVioMessageAsMulti() = 0;
    virtual HNS_Message2 fGetOverVioMessage() = 0;
    virtual std::string fGetOverVioMessageAsMulti() = 0;
    virtual HNS_Message2 fGetMaximumMessage() = 0;
    virtual std::string fGetMaximumMessageAsMulti() = 0;
    virtual HNS_Radar_Police_Speed fGetRadarPoliceSpeed() const = 0;
    virtual HNS_Radar_Settings fGetRadarSettings() const = 0;

    virtual HNS_Radar_Strobe fGetStrobeSettings() const = 0;

    virtual double fGetBattVoltage() const = 0;
    virtual double fGetBattCurrent() const = 0;
    virtual double fGetSolarCurrent() const = 0;
    virtual double fGetSolarVoltage() const = 0;
    virtual double fGetBattTemp() const = 0;
    virtual double fGetBoxTemp() const = 0;
    virtual unsigned char fGetChargerError() const = 0;

    virtual HNS_IP_Settings fGetIPSettings() const = 0;

    virtual HNS_SignBoard fGetPixOutBrd() const = 0;
    virtual bool fGetPixOutRunning() const = 0;

    virtual bool fGetLockState() const = 0;
    virtual bool fGetLockRequired() const = 0;

    virtual HNS_Security fGetLoginInfo() const = 0;
    virtual HNS_Security fGetDefaultLogin() const = 0;

    virtual std::vector<std::string> fGetTrailerNames() const = 0;
    virtual int fGetSelectedTrailer() const = 0;

    virtual std::string fGetCurrentMessage() const = 0;

    virtual int fGetGPIO(const int &port) const = 0;
    virtual std::vector<HNS_GPIO_Port> fGetGPIOPorts() const = 0;
    virtual HNS_SignBoard_Info fGetSignCfg() const = 0;

    virtual HNS_NTCIP_MessageIDCode fGetTMCustomMsg(const size_t &index) const = 0;
    virtual std::string fGetTMCustomMsgMULTI(const size_t &index) const = 0;

    virtual std::vector<HNS_NTCIP_MessageIDCode> fGetAuxIOMessages() const = 0;
    virtual std::vector<std::string> fGetAUXIOMessagesMULTI() const = 0;
    virtual std::vector<unsigned char> fGetAUXIOEnables() const = 0;
    virtual std::vector<int> fGetAUXIODurations() const = 0;
    virtual size_t fGetNumAuxIO() const = 0;

    virtual bool fHasBeacons() const = 0;

    virtual ~HNS_Machine_Interface() {}
};

#endif // HNS_MACHINE_INTERFACE_H
