#include "hns_settings_screen.h"
#include "ui_hns_settings_screen.h"

#include "hns_date_time_settings_screen.h"
#include "hns_brightness_settings_screen.h"
#include "hns_radar_settings_screen.h"
#include "hns_page_settings_page.h"
#include "hns_comm_settings_screen.h"
#include "hns_admin_screen.h"
#include "hns_debug_screen.h"
#include "hns_diagnostic_select_screen.h"
#include "hns_auxio_screen.h"

HNS_Settings_Screen::HNS_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Settings_Screen)
  , f_timer(new QTimer(this))
{
    ui->setupUi(this);

    connect(f_timer,SIGNAL(timeout()),this,SLOT(fTickTock()));
    f_timer->setInterval(1000);
    f_timer->start();

    ui->LBL_Settings_CommMode->hide();
    ui->LBL_Settings_IPMode->hide();

    if(f_machine->fGetAccess() != HNS_ADMIN)
    {
        ui->PB_Admin->hide();
    }
    if(f_machine->fGetAccess() == HNS_VIEWER)
    {
        ui->PB_Settings_Brightness->hide();
        ui->PB_Settings_DateTime->hide();
        ui->PB_Settings_PageDefaults->hide();
        ui->PB_Settings_Comm->hide();
        ui->PB_Settings_Radar->setText("VIEW");
        ui->PB_Diagnostics->hide();
    }

    if(f_machine->fGetSignCfg().fGetUIMode() == HNS_UI_MODE_TM)
    {
        ui->GB_Communications->hide();
        ui->GB_Radar->hide();
    }

    fUpdate();
}

HNS_Settings_Screen::~HNS_Settings_Screen()
{
    delete ui;
}

type_widget HNS_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_SETTINGS;
}

void HNS_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

void HNS_Settings_Screen::fUpdate(const int &update_flags)
{
    QString tempstring;

    fTickTock();

    if(update_flags & HNS_DATA_ILLUMINATION_CHANGE)
    {
        if(f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_AUTO)
        {
            ui->LBL_Settings_BrightMode->setText("Automatic Mode");
            ui->LBL_Settings_BrightLevel->setText("Level Automatic");
        }
        else
        {
            ui->LBL_Settings_BrightMode->setText("Manual Mode");
            tempstring = QString("Level %1").arg(f_machine->fGetBrightnessLevel());
            ui->LBL_Settings_BrightLevel->setText(QString("Level %1").arg(f_machine->fGetBrightnessLevel()));
        }
    }

    if(update_flags & HNS_DATA_RADAR_CHANGE)
    {
        switch(f_machine->fGetRadarMode())
        {
        case HNS_RADAR_MODE_NORMAL:
            ui->LBL_Settings_RadarMode->setText("Normal");
            break;
        case HNS_RADAR_MODE_LEGACY:
            ui->LBL_Settings_RadarMode->setText("Legacy");
            break;
        case HNS_RADAR_MODE_POLICE:
            ui->LBL_Settings_RadarMode->setText("Police");
            break;
        }

        switch(f_machine->fGetRadarUnits())
        {
        case HNS_RADAR_UNITS_KPH:
            ui->LBL_Settings_RadarUnits->setText("KPH");
            break;
        case HNS_RADAR_UNITS_MPH:
            ui->LBL_Settings_RadarUnits->setText("MPH");
            break;
        }
    }

    if(update_flags & HNS_DATA_PAGE_DEFAULT_CHANGE)
    {
        switch(HNS_Message2::fGetDefaultLineJustification())
        {
        case HNS_JUSTIFICATION_LINE_CENTER:
            ui->LBL_Settings_LineJustification->setText("Center Line Justification");
            break;
        case HNS_JUSTIFICATION_LEFT:
            ui->LBL_Settings_LineJustification->setText("Left Line Justification");
            break;
        case HNS_JUSTIFICATION_RIGHT:
            ui->LBL_Settings_LineJustification->setText("Right Line Justification");
            break;
        case HNS_JUSTIFICATION_FULL:
            ui->LBL_Settings_LineJustification->setText("Full Line Justification");
            break;
        }

        switch(HNS_Message2::fGetDefaultPageJustification())
        {
        case HNS_JUSTIFICATION_PAGE_CENTER:
            ui->LBL_Settings_PageJustification->setText("Center Page Justification");
            break;
        case HNS_JUSTIFICATION_TOP:
            ui->LBL_Settings_PageJustification->setText("Top Page Justification");
            break;
        case HNS_JUSTIFICATION_BOTTOM:
            ui->LBL_Settings_PageJustification->setText("Bottom Page Justification");
            break;
        }

        ui->LBL_Settings_DefaultFont->setText(QString("Default Font %1").arg(HNS_Message2::fGetDefaultFont()));
    }

    ui->LBL_Settings_IP_Port->setText(QString("Port: %1").arg(f_machine->fGetIPSettings().fGetPort()));

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_Settings_Screen::on_PB_Settings_DateTime_clicked()
{
    fShowScreen(new HNS_Date_Time_Settings_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Settings_Brightness_clicked()
{
    fShowScreen(new HNS_Brightness_Settings_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Settings_Radar_clicked()
{
    fShowScreen(new HNS_Radar_Settings_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Settings_PageDefaults_clicked()
{
    fShowScreen(new HNS_Page_Settings_Page(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Settings_Comm_clicked()
{
    fShowScreen(new HNS_Comm_Settings_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Admin_clicked()
{
    fShowScreen(new HNS_Admin_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Diagnostics_clicked()
{
    fShowScreen(new HNS_Diagnostic_Select_Screen(this, f_machine));
}


void HNS_Settings_Screen::on_PB_Settings_Back_clicked()
{
    emit SIG_GO_HOME();
}

void HNS_Settings_Screen::fTickTock()
{
    QDateTime temp_datetime = QDateTime::currentDateTime();
    QString tempstring;

    tempstring = temp_datetime.date().toString();
    ui->LBL_Settings_Date->setText(tempstring);
    tempstring = temp_datetime.time().toString("h:mm AP");
    ui->LBL_Settings_Time->setText(tempstring);
}


void HNS_Settings_Screen::on_PB_Settings_AuxIO_clicked()
{
    fShowScreen(new HNS_AuxIO_Screen(this,f_machine));
}
