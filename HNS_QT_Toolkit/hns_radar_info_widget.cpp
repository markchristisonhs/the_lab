#include "hns_radar_info_widget.h"
#include "ui_hns_radar_info_widget.h"

HNS_Radar_Info_Widget::HNS_Radar_Info_Widget(QWidget *parent, const int &mode) :
    QWidget(parent),
    ui(new Ui::HNS_Radar_Info_Widget)
  , f_info_mode(mode)
{
    ui->setupUi(this);
}

HNS_Radar_Info_Widget::~HNS_Radar_Info_Widget()
{
    delete ui;
}

void HNS_Radar_Info_Widget::fSetInfoMode(const int &mode)
{
    f_info_mode = mode;
}

void HNS_Radar_Info_Widget::fSetRadarInfo(const type_radar_mode &mode, const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed, const HNS_Radar_Strobe &strobe)
{
    if(f_info_mode == HNS_RADAR_INFO_NORMAL)
    {
        QString tempstring;
        if(mode == HNS_RADAR_MODE_LEGACY)
        {
            tempstring = "LEGACY";
        }
        else if(mode == HNS_RADAR_MODE_POLICE)
        {
            tempstring = "POLICE";
        }
        else
        {
            tempstring = "NORMAL";
        }
        ui->LBL_Radar_Mode->setText(tempstring);

        if(mode == HNS_RADAR_MODE_LEGACY)
        {
            tempstring = QString("%1").arg(legacy_speed);
            ui->LBL_Radar_Info->setText(tempstring);
        }
        else if(mode == HNS_RADAR_MODE_POLICE)
        {
            tempstring = QString("%1/%2/%3").arg(police_speed.fGetMinimumSpeed()).arg(police_speed.fGetViolatorSpeed()).arg(police_speed.fGetMaximumSpeed());
            ui->LBL_Radar_Info->setText(tempstring);
        }
        else
        {
            ui->LBL_Radar_Info->setText("");
        }

        if(strobe.fIsEnabled())
        {
            tempstring = QString("%1/%2/%3").arg(strobe.fGetSpeed()).arg(strobe.fGetDuration()).arg(strobe.fGetPattern());
            ui->LBL_Strobe_Info->setText(tempstring);
        }
        else
        {
            ui->LBL_Strobe_Info->setText("DISABLED");
        }
    }
}

void HNS_Radar_Info_Widget::fSetScheduledRadarInfo(const HNS_Schedule &schedule)
{
    fSetScheduledRadarInfo(schedule.fGetOverrideRadar(),schedule.fGetOverrideStrobe(),schedule.fGetRadarSettings(),schedule.fGetStrobeSettings());
}

void HNS_Radar_Info_Widget::fSetScheduledRadarInfo(const bool &override_radar, const bool &override_strobe, const HNS_Radar_Settings &settings, const HNS_Radar_Strobe &strobe_settings)
{
    if(f_info_mode == HNS_RADAR_INFO_SCHEDULE)
    {
        QString tempstring;
        if(override_radar)
        {
            QString tempstring;
            if(settings.fGetMode() == HNS_RADAR_MODE_LEGACY)
            {
                tempstring = "LEGACY";
            }
            else if(settings.fGetMode() == HNS_RADAR_MODE_POLICE)
            {
                tempstring = "POLICE";
            }
            else
            {
                tempstring = "NORMAL";
            }
            ui->LBL_Radar_Mode->setText(tempstring);
            ui->LBL_Radar_Info->show();

            if(settings.fGetMode() == HNS_RADAR_MODE_LEGACY)
            {
                tempstring = QString("%1").arg(settings.fGetLegacySpeed());
                ui->LBL_Radar_Info->setText(tempstring);
            }
            else if(settings.fGetMode() == HNS_RADAR_MODE_POLICE)
            {
                tempstring = QString("%1/%2/%3").arg(settings.fGetPoliceSpeed().fGetMinimumSpeed()).arg(settings.fGetPoliceSpeed().fGetViolatorSpeed()).arg(settings.fGetPoliceSpeed().fGetMaximumSpeed());
                ui->LBL_Radar_Info->setText(tempstring);
            }
            else
            {
                ui->LBL_Radar_Info->setText("");
            }
        }
        else
        {
            ui->LBL_Radar_Mode->setText("NO CHANGE");
            ui->LBL_Radar_Info->hide();
        }

        if(override_strobe)
        {
            if(strobe_settings.fIsEnabled())
            {
                tempstring = QString("%1/%2/%3").arg(strobe_settings.fGetSpeed()).arg(strobe_settings.fGetDuration()).arg(strobe_settings.fGetPattern());
                ui->LBL_Strobe_Info->setText(tempstring);
            }
            else
            {
                ui->LBL_Strobe_Info->setText("DISABLED");
            }
        }
        else
        {
            ui->LBL_Strobe_Info->setText("NO CHANGE");
        }
    }
}
