#ifndef HNS_SCHEDULE_EDIT_SCREEN_H
#define HNS_SCHEDULE_EDIT_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <HNS_schedule.h>
#include <QCalendarWidget>

typedef enum
{
    HNS_SCHED_CREATE = 0,
    HNS_SCHED_EDIT
} type_sched_mode;

namespace Ui {
class HNS_Schedule_Edit_Screen;
}

class HNS_Schedule_Edit_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Schedule_Edit_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_sched_mode &sched_mode = HNS_SCHED_CREATE, HNS_Schedule *schedule = nullptr, const int &index = -1);
    ~HNS_Schedule_Edit_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

    type_sched_mode fGetMode() const {return f_sched_mode;}

public slots:
    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

    void my_slot_AMPM_Toggle();

    void my_slot_Strobe_Change_Request(const HNS_Radar_Strobe &strobesettings);
    void my_slot_Radar_Change_Request(const type_radar_mode &mode, const type_radar_units &units,
                                      const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed,
                                      const QString &legacy_msg, const QString &under_min_msg, const QString &under_vio_msg,
                                      const QString &over_vio_msg, const QString &over_max_msg);

private slots:
    void on_PB_Sched_Back_clicked();

    void on_PB_Sched_NewSched_clicked();

    void on_PB_Sched_Cancel_clicked();

    void on_PB_Sched_EditMessage_clicked();

    void on_PB_Sched_StartNow_clicked();

    void on_PB_Sched_NeverEnd_clicked();

    void on_PB_Sched_SelectStartDate_clicked();

    void on_PB_Sched_StartTime_clicked();

    void on_PB_Sched_EndDate_clicked();

    void on_PB_Sched_EndTime_clicked();

    void on_PB_Sched_EditRadar_clicked();

    void on_PB_Sched_ClearRadar_clicked();

    void on_PB_Sched_EditStrobes_clicked();

    void on_PB_Sched_ClearStrobes_clicked();

    void my_slot_Clear_Invalid();

private:
    Ui::HNS_Schedule_Edit_Screen *ui;

    type_sched_mode f_sched_mode;

    QObject *f_calendar_object;

    int f_index;

    void fInit(HNS_Schedule &schedule);

    bool eventFilter(QObject *obj, QEvent *event);
    void hideAllPopup(bool startDate = true, bool endDate = true, bool startTime = true, bool endTime = true);

    void fCalendar_Finished(const QDate &date);
    void fVKP_Finished(const bool &enter_pressed);

    type_user_level f_current_access;
    HNS_Radar_Settings f_temp_radar_settings;
    HNS_Radar_Strobe f_temp_strobe_settings;
    bool f_override_radar,f_override_strobe;
};

#endif // HNS_SCHEDULE_EDIT_SCREEN_H
