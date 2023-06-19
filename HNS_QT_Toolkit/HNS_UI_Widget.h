#ifndef HNS_UI_WIDGET_H
#define HNS_UI_WIDGET_H

#include <QWidget>
#include "security.h"
#include "hns_machine_interface.h"
#include "hns_vkb.h"
#include "hns_vkp.h"
#include <QCalendarWidget>
#include <QListView>
#include <HNS_schedule.h>

typedef enum
{
    HNS_WIDGET_HOME = 0,
    HNS_WIDGET_MESSAGE,
    HNS_WIDGET_SCHEDULE_LIST,
    HNS_WIDGET_SCHEDULE_EDIT,
    HNS_WIDGET_STATUS,
    HNS_WIDGET_SETTINGS,
    HNS_WIDGET_MESSAGE_EDIT,
    HNS_WIDGET_MESSAGE_LIBRARY_SELECT,
    HNS_WIDGET_MESSAGE_LIBRARY,
    HNS_WIDGET_DATE_TIME_SETTINGS,
    HNS_WIDGET_BRIGHTNESS_SETTINGS,
    HNS_WIDGET_RADAR_SETTINGS,
    HNS_WIDGET_PAGE_SETTINGS,
    HNS_WIDGET_COMM_SETTINGS,
    HNS_WIDGET_FONT_SETTINGS,
    HNS_WIDGET_PREVIEW,
    HNS_WIDGET_ADMIN,
    HNS_WIDGET_TRAILER,
    HNS_WIDGET_PIXOUT,
    HNS_WIDGET_VSL,
    HNS_WIDGET_STROBE_SETTINGS,
    HNS_WIDGET_DIAGNOSTIC_SELECT,
    HNS_WIDGET_TEST_PATTERN,
    HNS_WIDGET_CHANGE_LOGIN,
    HNS_WIDGET_DEBUG,
    HNS_WIDGET_RS485_DEBUG,
    HNS_WIDGET_GPIO_DEBUG,
    HNS_WIDGET_LOGIN,
    HNS_WIDGET_DIRECT_MULTI,
    HNS_WIDGET_CONFIRM,
    HNS_WIDGET_HOME_TTMB,
    HNS_WIDGET_TM_CUSTOM_MSG,
    HNS_WIDGET_AUXIO
} type_widget;

typedef enum
{
    HNS_PLAY_MESSAGE = 0,
    HNS_ADD_ONLY,
    HNS_ADD_AND_PLAY
} type_ui_message_mode;

class HNS_UI_Widget : public QWidget
{
    Q_OBJECT

public:
    HNS_UI_Widget(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_UI_Widget();

    virtual type_widget fGetType() const = 0;
    virtual void fSetAccess(const type_user_level &access) = 0;

    virtual void fUpdate(const int &update_flags = HNS_DATA_ALL) {if(f_shown_screen != nullptr)f_shown_screen->fUpdate(update_flags);}

    virtual bool fIsVisible() {return f_shown_screen == nullptr;}
public slots:
    virtual void my_slot_Go_Home() {emit SIG_GO_HOME();}

    virtual void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode) {emit SIG_Message(multi,id_code,mode);}
    virtual void my_slot_Delete_Request(const HNS_NTCIP_MessageIDCode &id_code, const bool &del_all) {emit SIG_Delete_Request(id_code,del_all);}

    virtual void my_slot_Schedule(const HNS_Schedule &schedule, const int &index, const bool &add) {emit SIG_Schedule(schedule, index, add);}
    virtual void my_slot_Delete_Schedule_Request(const int &index) {emit SIG_Delete_Schedule_Request(index);}

    virtual void my_slot_Run_PixOut() {emit SIG_Run_PixOut();}

    virtual void my_slot_Lock_Request() {emit SIG_Lock_Request();}
    virtual void my_slot_Reset_Request() {emit SIG_Reset_Request();}
    virtual void my_slot_Update_Request() {emit SIG_Update_Request();}

    virtual void my_slot_Change_Login_Request(const HNS_Security &security) {emit SIG_Change_Login_Request(security);}

    virtual void my_slot_Trailer_Change_Request(const int &trailer_select) {emit SIG_Trailer_Change_Request(trailer_select);}

    virtual void my_slot_PageSettings_Change_Request(const type_justification_line &justification_line,
                                                     const type_justification_page &justification_page,
                                                     const double &page_time_on,
                                                     const double &page_time_off,
                                                     const int &font_select) {emit SIG_PageSettings_Change_Request(justification_line,
                                                                                                                   justification_page,
                                                                                                                   page_time_on,
                                                                                                                   page_time_off,
                                                                                                                   font_select);}
    virtual void my_slot_DateTime_Change_Request(const QDate &date, const QTime &time, const QString &time_zone) {emit SIG_DateTime_Change_Request(date,time,time_zone);}
    virtual void my_slot_Brightness_Change_Request(const type_brightness_mode &mode, const int &level) {emit SIG_Brightness_Change_Request(mode,level);}
    virtual void my_slot_Strobe_Test() {emit SIG_Strobe_Test();}
    virtual void my_slot_Strobe_Change_Request(const HNS_Radar_Strobe &strobesettings) {emit SIG_Strobe_Change_Request(strobesettings);}
    virtual void my_slot_Radar_Change_Request(const type_radar_mode &mode, const type_radar_units &units,
                                              const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed,
                                              const QString &legacy_msg, const QString &under_min_msg, const QString &under_vio_msg,
                                              const QString &over_vio_msg, const QString &over_max_msg) {emit SIG_Radar_Change_Request(mode,units,legacy_speed,police_speed,legacy_msg,under_min_msg,under_vio_msg,over_vio_msg,over_max_msg);}
    virtual void my_slot_Comm_Change_Request(const HNS_IP_Settings &ip_settings) {emit SIG_Comm_Change_Request(ip_settings);}

    virtual void my_slot_GPIO_Change_Request(const int &index, const int &value) {emit SIG_GPIO_Change_Request(index,value);}
    virtual void my_slot_TM_Custom_Msg_Change_Request(const int &index, const HNS_NTCIP_MessageIDCode &id_code) {emit SIG_TM_Custom_Msg_Change_Request(index,id_code);}

    virtual void my_slot_AuxIO_Change_Request(const QVector<HNS_NTCIP_MessageIDCode> &messages, const QVector<int> &durations, const QVector<unsigned char> &enables) {emit SIG_AuxIO_Change_Request(messages,durations,enables);}

    virtual void my_slot_AMPM_Toggle() {}
signals:
    void SIG_HNS_UI_CANCEL();
    void SIG_HNS_UI_OK();
    void SIG_GO_HOME();

    void SIG_VKB_Closed();
    void SIG_VKP_Closed(const bool &enter_pressed);

    void SIG_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);
    void SIG_Delete_Request(const HNS_NTCIP_MessageIDCode &id_code, const bool &del_all);

    void SIG_Schedule(const HNS_Schedule &schedule, const int &index, const bool &add);
    void SIG_Delete_Schedule_Request(const int &index);

    void SIG_Run_PixOut();

    void SIG_Lock_Request();
    void SIG_Reset_Request();
    void SIG_Update_Request();

    void SIG_Change_Login_Request(const HNS_Security &security);

    void SIG_Trailer_Change_Request(const int &trailer_select);

    void SIG_PageSettings_Change_Request(const type_justification_line &justification_line,
                                         const type_justification_page &justification_page,
                                         const double &page_time_on,
                                         const double &page_time_off,
                                         const int &font_select);
    void SIG_DateTime_Change_Request(const QDate &date, const QTime &time, const QString &time_zone);
    void SIG_Brightness_Change_Request(const type_brightness_mode &mode, const int &level);
    void SIG_Strobe_Test();
    void SIG_Strobe_Change_Request(const HNS_Radar_Strobe &strobesettings);
    void SIG_Radar_Change_Request(const type_radar_mode &mode, const type_radar_units &units,
                                  const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed,
                                  const QString &legacy_msg, const QString &under_min_msg, const QString &under_vio_msg,
                                  const QString &over_vio_msg, const QString &over_max_msg);
    void SIG_Comm_Change_Request(const HNS_IP_Settings &ip_settings);

    void SIG_Update_Data_Request(const int &change_flag);

    void SIG_GPIO_Change_Request(const int &index, const int &value);

    void SIG_TM_Custom_Msg_Change_Request(const int &index, const HNS_NTCIP_MessageIDCode &id_code);

    void SIG_AuxIO_Change_Request(const QVector<HNS_NTCIP_MessageIDCode> &messages, const QVector<int> &durations, const QVector<unsigned char> &enables);
private slots:
    void my_Slot_Shown_Screen_Destroyed(QObject *obj = nullptr);

    void my_slot_close_VKP();

    void my_slot_VKP_destroyed(QObject *);

    void my_slot_Date_Selected();

    void my_slot_Calendar_Destroyed(QObject *);
protected:
    HNS_UI_Widget *f_shown_screen;
    HNS_Machine_Interface *f_machine;

    bool fShowScreen(HNS_UI_Widget *screen, const int &x = 0, const int &y = 0);
    void fToggleKB(QObject *obj, const bool &narrow_mode);
    void fShowKB(QObject *obj, const bool &narrow_mode);
    void fShowKB(QObject *obj, const int &x, const int &y, const bool &narrow_mode);
    void fHideKB();
    void fChangeKBFocus(QObject *obj);
    QObject *fGetKBFocus();
    bool fIsKBVisible();
    void fToggleKP(QObject *obj, const int &x, const int &y, const bool &time_mode = false);
    void fShowKP(QObject *obj, const int &x, const int &y, const bool &time_mode = false);
    void fHideKP();
    void fChangeKPFocus(QObject *obj);
    QObject *fGetKPFocus();
    bool fIsKPVisible();
    void fCloseKeyboards();

    void fShowCalendar(const QDate &date, const int &x, const int &y);
    void fHideCalendar();
    bool fIsCalendarVisible();

    virtual void fVKP_Finished(const bool &) {}
    virtual void fCalendar_Finished(const QDate &) {}
private:
    HNS_VKB *f_vkb;
    HNS_VKP *f_vkp;
    QCalendarWidget *f_calendar;
    QListView *f_yearView;

    void initYearPopup(QCalendarWidget *calendar);
};

#endif // HNS_UI_WIDGET_H
