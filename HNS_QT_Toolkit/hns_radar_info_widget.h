#ifndef HNS_RADAR_INFO_WIDGET_H
#define HNS_RADAR_INFO_WIDGET_H

#include <QWidget>
#include <typedefs.h>
#include <structures.h>
#include <HNS_schedule.h>

namespace Ui {
class HNS_Radar_Info_Widget;
}

enum
{
    HNS_RADAR_INFO_NORMAL,
    HNS_RADAR_INFO_SCHEDULE
};

class HNS_Radar_Info_Widget : public QWidget
{
    Q_OBJECT

public:
    HNS_Radar_Info_Widget(QWidget *parent = nullptr, const int &mode = HNS_RADAR_INFO_NORMAL);
    ~HNS_Radar_Info_Widget();

    void fSetInfoMode(const int &mode);
    int fGetInfoMode() const {return f_info_mode;}

    void fSetRadarInfo(const type_radar_mode &mode, const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed, const HNS_Radar_Strobe &strobe);
    void fSetScheduledRadarInfo(const HNS_Schedule &schedule);
    void fSetScheduledRadarInfo(const bool &override_radar, const bool &override_strobe, const HNS_Radar_Settings &settings, const HNS_Radar_Strobe &strobe_settings);

private:
    Ui::HNS_Radar_Info_Widget *ui;

    int f_info_mode;
};

#endif // HNS_RADAR_INFO_WIDGET_H
