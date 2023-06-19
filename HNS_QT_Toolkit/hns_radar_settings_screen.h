#ifndef HNS_RADAR_SETTINGS_SCREEN_H
#define HNS_RADAR_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <hns_message_preview.h>

namespace Ui {
class HNS_Radar_Settings_Screen;
}

enum
{
    HNS_RADAR_SETTINGS_NORMAL,
    HNS_RADAR_SETTINGS_SCHEDULE
};

class HNS_Radar_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Radar_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, HNS_Radar_Settings *settings = nullptr, const int &mode = HNS_RADAR_SETTINGS_NORMAL);
    ~HNS_Radar_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdateRadarInputs();
public slots:
    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

private slots:
    void on_PB_RadarSet_Save_clicked();

    void on_PB_RadarSet_Cancel_clicked();

    void on_PB_RadarSet_Back_clicked();

    void on_PB_RadarSet_Strobes_clicked();

    void on_PB_RadarSet_Normal_clicked();

    void on_PB_RadarSet_Legacy_clicked();

    void on_PB_RadarSet_Police_clicked();

    void on_PB_RadarSet_Units_clicked();

    void my_slot_Clear_Invalid();

private:
    bool eventFilter(QObject *watched, QEvent *event);

    void on_GV_RadarMsg1_Clicked();
    void on_GV_RadarMsg2_Clicked();
    void on_GV_RadarMsg3_Clicked();
    void on_GV_RadarMsg4_Clicked();

    void fVKP_Finished(const bool &);

    Ui::HNS_Radar_Settings_Screen *ui;

    HNS_Message2 f_legacy_message, f_under_min_message, f_under_vio_message, f_over_vio_message, f_over_max_message;
    int f_legacy_speed;
    HNS_Radar_Police_Speed f_police_speed;
    HNS_Radar_Settings f_radar_settings;

    HNS_Message_Preview *f_message_target;
};

#endif // HNS_RADAR_SETTINGS_SCREEN_H
