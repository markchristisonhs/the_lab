#include "hns_status_screen.h"
#include "ui_hns_status_screen.h"

HNS_Status_Screen::HNS_Status_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Status_Screen)
{
    ui->setupUi(this);

    ui->GRP_Communications->hide();

    if(f_machine->fGetSignCfg().fGetUIMode() == HNS_UI_MODE_TM)
    {
        ui->GB_Radar->hide();
        ui->LBL_OutsideTempTitle->hide();
        ui->LBL_Status_OutsideTemp->hide();
        ui->LBL_SolarCurrentTitle->hide();
        ui->LBL_Status_SolarCurrent->hide();
        ui->LBL_SolarVoltageTitle->hide();
        ui->LBL_Status_SolarVoltage->hide();
    }

    fUpdate();
}

HNS_Status_Screen::~HNS_Status_Screen()
{
    delete ui;
}

type_widget HNS_Status_Screen::fGetType() const
{
    return HNS_WIDGET_STATUS;
}

void HNS_Status_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

void HNS_Status_Screen::fUpdate(const int &update_flags)
{
    QString tempstring;
    if(update_flags & HNS_DATA_ILLUMINATION_CHANGE)
    {
        if(f_machine->fGetPhotocellConnected())
        {
            ui->LBL_Status_BrightStatus->setText("Connected");
        }
        else
        {
            ui->LBL_Status_BrightStatus->setText("Disconnected");
        }

        if(f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_AUTO)
        {
            ui->LBL_Status_BrightMode->setText("Auto");
        }
        else
        {
            ui->LBL_Status_BrightMode->setText("Manual");
        }

        ui->LBL_Status_LightInput->setText(QString("%1").arg(f_machine->fGetLastPhotoCellValue()));
        ui->LBL_Status_Brightness->setText(QString("%1").arg(f_machine->fGetBrightnessLevel()));
    }

    if(update_flags & HNS_DATA_RADAR_CHANGE)
    {
        if(f_machine->fGetRadarConnected())
        {
            ui->LBL_Status_RadarStatus->setText("Connected");

            ui->LBL_Status_LastSpeed->setText(QString("%1 MPH").arg(f_machine->fGetLastSpeed()));
            std::vector<type_speed_log_entry> speed_log = f_machine->fGetSpeedLog();
            QDateTime temp_datetime;
            QDate temp_date;
            QTime temp_time;
            if(speed_log.size() > 0)
            {
                temp_datetime.setMSecsSinceEpoch(speed_log[0].f_time);
                temp_date = temp_datetime.date();
                temp_time = temp_datetime.time();
                tempstring = temp_date.toString("MM/dd/yy") + " | " + temp_time.toString("h:mm AP") + " | " + QString("%1 MPH").arg(speed_log[0].f_speed);
                ui->LBL_Status_SpeedLog1->setText(tempstring);
            }
            else
            {
                ui->LBL_Status_SpeedLog1->setText(QString("... MPH"));
            }

            if(speed_log.size() > 1)
            {
                temp_datetime.setMSecsSinceEpoch(speed_log[1].f_time);
                temp_date = temp_datetime.date();
                temp_time = temp_datetime.time();
                tempstring = temp_date.toString("MM/dd/yy") + " | " + temp_time.toString("h:mm AP") + " | " + QString("%1 MPH").arg(speed_log[1].f_speed);
                ui->LBL_Status_SpeedLog2->setText(tempstring);
            }
            else
            {
                ui->LBL_Status_SpeedLog2->setText(QString("... MPH"));
            }

            if(speed_log.size() > 2)
            {
                temp_datetime.setMSecsSinceEpoch(speed_log[2].f_time);
                temp_date = temp_datetime.date();
                temp_time = temp_datetime.time();
                tempstring = temp_date.toString("MM/dd/yy") + " | " + temp_time.toString("h:mm AP") + " | " + QString("%1 MPH").arg(speed_log[2].f_speed);
                ui->LBL_Status_SpeedLog3->setText(tempstring);
            }
            else
            {
                ui->LBL_Status_SpeedLog3->setText(QString("... MPH"));
            }
        }
        else
        {
            ui->LBL_Status_RadarStatus->setText("Disconnected");
            ui->LBL_Status_LastSpeed->setText("... MPH");
            ui->LBL_Status_SpeedLog1->setText("... MPH");
            ui->LBL_Status_SpeedLog2->setText("... MPH");
            ui->LBL_Status_SpeedLog3->setText("... MPH");
        }
    }

    if(update_flags & HNS_DATA_CHARGER_CHANGE)
    {
        double batt_voltage,batt_current,solar_current, solar_voltage;
        batt_voltage = f_machine->fGetBattVoltage();
        solar_current = f_machine->fGetSolarCurrent();
        batt_current = f_machine->fGetBattCurrent();
        solar_voltage = f_machine->fGetSolarVoltage();

        ui->LBL_Status_BattVoltage->setText(QString("%1").arg(batt_voltage, 0, 'f',1));
        ui->LBL_Status_BattCurrent->setText(QString("%1").arg(batt_current, 0, 'f', 1));
        ui->LBL_Status_SolarCurrent->setText(QString("%1").arg(solar_current, 0, 'f',1));
        ui->LBL_Status_SolarVoltage->setText(QString("%1").arg(solar_voltage, 0, 'f',1));

        double batt_temp,box_temp;
        batt_temp = f_machine->fGetBattTemp();
        box_temp = f_machine->fGetBoxTemp();

        ui->LBL_Status_BoxTemp->setText(QString("%1").arg(batt_temp,0,'f',1));
        ui->LBL_Status_OutsideTemp->setText(QString("%1").arg(box_temp,0,'f',1));
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_Status_Screen::on_PB_Status_Back_clicked()
{
    emit SIG_GO_HOME();
}

