#include "hns_radar_settings_screen.h"
#include "ui_hns_radar_settings_screen.h"

#include "hns_strobe_settings_screen.h"
#include "hns_message_edit_screen.h"

#include <QTimer>

using namespace std;

HNS_Radar_Settings_Screen::HNS_Radar_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, HNS_Radar_Settings *settings, const int &mode) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Radar_Settings_Screen)
  , f_message_target(nullptr)
{
    HNS_NTCIP_MessageActivationCode temp_act_code;
    QString tempstring;
    int speed_temp;

    f_police_speed = f_machine->fGetRadarPoliceSpeed();

    if(settings != nullptr)
    {
        f_radar_settings = *settings;
    }
    else if(f_machine != nullptr)
    {
        f_radar_settings = f_machine->fGetRadarSettings();
    }

    ui->setupUi(this);

    if(mode == HNS_RADAR_SETTINGS_SCHEDULE)
    {
        ui->PB_RadarSet_Strobes->hide();
    }

    ui->GV_RadarSet_Msg1->installEventFilter(this);
    ui->GV_RadarSet_Msg2->installEventFilter(this);
    ui->GV_RadarSet_Msg3->installEventFilter(this);
    ui->GV_RadarSet_Msg4->installEventFilter(this);
    ui->LE_RadarSet_MaxSpeed->installEventFilter(this);
    ui->LE_RadarSet_MinSpeed->installEventFilter(this);
    ui->LE_RadarSet_VioSpeed->installEventFilter(this);
    installEventFilter(this);

    if(f_machine != nullptr)
    {
        ui->GV_RadarSet_Msg1->fSetFonts(*f_machine->fGetFonts());
        ui->GV_RadarSet_Msg2->fSetFonts(*f_machine->fGetFonts());
        ui->GV_RadarSet_Msg3->fSetFonts(*f_machine->fGetFonts());
        ui->GV_RadarSet_Msg4->fSetFonts(*f_machine->fGetFonts());

        ui->GV_RadarSet_Msg1->fSetGraphics(*f_machine->fGetGraphics());
        ui->GV_RadarSet_Msg2->fSetGraphics(*f_machine->fGetGraphics());
        ui->GV_RadarSet_Msg3->fSetGraphics(*f_machine->fGetGraphics());
        ui->GV_RadarSet_Msg4->fSetGraphics(*f_machine->fGetGraphics());
    }

    ui->PB_RadarSet_SpeedMode->hide();

    ui->PB_RadarSet_Legacy->setChecked(f_radar_settings.fGetMode() == HNS_RADAR_MODE_LEGACY);
    ui->PB_RadarSet_Normal->setChecked(f_radar_settings.fGetMode() == HNS_RADAR_MODE_NORMAL);
    ui->PB_RadarSet_Police->setChecked(f_radar_settings.fGetMode() == HNS_RADAR_MODE_POLICE);

    if(f_radar_settings.fGetUnits() == HNS_RADAR_UNITS_MPH)
    {
        ui->PB_RadarSet_Units->setText("MPH");
    }
    else
    {
        ui->PB_RadarSet_Units->setText("KPH");
    }

    if(mode == HNS_RADAR_SETTINGS_SCHEDULE)
    {
        ui->PB_RadarSet_Units->hide();
    }

    switch(f_radar_settings.fGetMode())
    {
    case HNS_RADAR_MODE_NORMAL:
        ui->LBL_RadarSet_MaxSpeed->hide();
        ui->LE_RadarSet_MaxSpeed->hide();
        ui->LBL_RadarSet_MinSpeed->hide();
        ui->LE_RadarSet_MinSpeed->hide();
        ui->LBL_RadarSet_VioSpeed->hide();
        ui->LE_RadarSet_VioSpeed->hide();
        ui->GV_RadarSet_Msg1->hide();
        ui->GV_RadarSet_Msg2->hide();
        ui->GV_RadarSet_Msg3->hide();
        ui->GV_RadarSet_Msg4->hide();
        ui->PB_RadarSet_Normal->show();
        ui->LBL_RadarSet_TouchMsg->hide();

        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->PB_RadarSet_Legacy->show();
            ui->PB_RadarSet_Police->show();
            ui->PB_RadarSet_Save->show();
            ui->PB_RadarSet_Units->setEnabled(true);
        }
        else
        {
            ui->PB_RadarSet_Legacy->hide();
            ui->PB_RadarSet_Police->hide();
            ui->PB_RadarSet_Save->hide();
            ui->PB_RadarSet_Units->setEnabled(false);
        }
        break;
    case HNS_RADAR_MODE_LEGACY:
        ui->LBL_RadarSet_MaxSpeed->hide();
        ui->LE_RadarSet_MaxSpeed->hide();
        ui->LBL_RadarSet_MinSpeed->hide();
        ui->LE_RadarSet_MinSpeed->hide();
        ui->LBL_RadarSet_VioSpeed->show();
        ui->LBL_RadarSet_VioSpeed->setText("Trip Speed");
        ui->LE_RadarSet_VioSpeed->show();
        tempstring = QString("%1 ").arg(f_radar_settings.fGetLegacySpeed()) + ((f_radar_settings.fGetUnits() == HNS_RADAR_UNITS_MPH) ? QString("MPH") : QString("KPH"));
        ui->LE_RadarSet_VioSpeed->setText(tempstring);
        ui->GV_RadarSet_Msg1->hide();
        ui->GV_RadarSet_Msg2->hide();
        ui->LBL_RadarSet_TouchMsg->show();

        ui->GV_RadarSet_Msg3->show();
        ui->GV_RadarSet_Msg3->fSetMessage(QString::fromStdString(f_radar_settings.fGetLegacyMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,91,f_radar_settings.fGetLegacyMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg3->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg4->hide();
        ui->PB_RadarSet_Legacy->show();

        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->PB_RadarSet_Normal->show();
            ui->PB_RadarSet_Police->show();
            ui->PB_RadarSet_Save->show();
            ui->LE_RadarSet_VioSpeed->setEnabled(true);
            ui->PB_RadarSet_Units->setEnabled(true);
        }
        else
        {
            ui->GV_RadarSet_Msg3->fSetMessage(QString::fromStdString(f_radar_settings.fGetLegacyMessage()));
            temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,91,f_radar_settings.fGetLegacyMessage(),"127.0.0.1");
            ui->GV_RadarSet_Msg3->fSetActivationCode(temp_act_code);

            ui->PB_RadarSet_Normal->hide();
            ui->PB_RadarSet_Police->hide();
            ui->PB_RadarSet_Save->hide();
            ui->LE_RadarSet_VioSpeed->setEnabled(false);
            ui->PB_RadarSet_Units->setEnabled(false);
            ui->LBL_RadarSet_TouchMsg->hide();
        }
        break;
    case HNS_RADAR_MODE_POLICE:
        ui->LBL_RadarSet_MaxSpeed->show();
        speed_temp = f_radar_settings.fGetPoliceSpeed().fGetMaximumSpeed();
        tempstring = QString("%1 ").arg(speed_temp) + ((f_radar_settings.fGetUnits() == HNS_RADAR_UNITS_MPH) ? QString("MPH") : QString("KPH"));

        ui->LE_RadarSet_MaxSpeed->show();
        ui->LE_RadarSet_MaxSpeed->setText(tempstring);
        ui->LBL_RadarSet_MinSpeed->show();

        speed_temp = f_radar_settings.fGetPoliceSpeed().fGetMinimumSpeed();
        tempstring = QString("%1 ").arg(speed_temp) + ((f_radar_settings.fGetUnits() == HNS_RADAR_UNITS_MPH) ? QString("MPH") : QString("KPH"));

        ui->LE_RadarSet_MinSpeed->show();
        ui->LE_RadarSet_MinSpeed->setText(tempstring);
        ui->LBL_RadarSet_VioSpeed->show();
        ui->LBL_RadarSet_VioSpeed->setText("Violator Speed");

        speed_temp = f_radar_settings.fGetPoliceSpeed().fGetViolatorSpeed();
        tempstring = QString("%1 ").arg(speed_temp) + ((f_radar_settings.fGetUnits() == HNS_RADAR_UNITS_MPH) ? QString("MPH") : QString("KPH"));

        ui->LE_RadarSet_VioSpeed->show();
        ui->LE_RadarSet_VioSpeed->setText(tempstring);

        ui->GV_RadarSet_Msg1->show();
        ui->GV_RadarSet_Msg1->fSetMessage(QString::fromStdString(f_radar_settings.fGetUnderMinMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,92,f_radar_settings.fGetUnderMinMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg2->show();
        ui->GV_RadarSet_Msg2->fSetMessage(QString::fromStdString(f_radar_settings.fGetUnderVioMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,93,f_radar_settings.fGetUnderVioMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg3->show();
        ui->GV_RadarSet_Msg3->fSetMessage(QString::fromStdString(f_radar_settings.fGetOverVioMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,94,f_radar_settings.fGetOverVioMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg4->show();
        ui->GV_RadarSet_Msg4->fSetMessage(QString::fromStdString(f_radar_settings.fGetOverMaxMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,95,f_radar_settings.fGetOverMaxMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->PB_RadarSet_Police->show();
        ui->LBL_RadarSet_TouchMsg->show();

        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->PB_RadarSet_Legacy->show();
            ui->PB_RadarSet_Normal->show();
            ui->PB_RadarSet_Save->show();
            ui->LE_RadarSet_VioSpeed->setEnabled(true);
            ui->LE_RadarSet_MaxSpeed->setEnabled(true);
            ui->LE_RadarSet_MinSpeed->setEnabled(true);
            ui->PB_RadarSet_Units->setEnabled(true);
        }
        else
        {
            ui->PB_RadarSet_Legacy->hide();
            ui->PB_RadarSet_Normal->hide();
            ui->PB_RadarSet_Save->hide();
            ui->LE_RadarSet_VioSpeed->setEnabled(false);
            ui->LE_RadarSet_MaxSpeed->setEnabled(false);
            ui->LE_RadarSet_MinSpeed->setEnabled(false);
            ui->PB_RadarSet_Units->setEnabled(false);
            ui->LBL_RadarSet_TouchMsg->hide();
        }
        break;
    }

    ui->LBL_RadarSet_Invalid->hide();
}

HNS_Radar_Settings_Screen::~HNS_Radar_Settings_Screen()
{
    delete ui;
}

void HNS_Radar_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Radar_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_RADAR_SETTINGS;
}

void HNS_Radar_Settings_Screen::fUpdateRadarInputs()
{
    QString tempstring;
    HNS_NTCIP_MessageActivationCode temp_act_code;

    if(ui->PB_RadarSet_Legacy->isChecked())
    {
        ui->LBL_RadarSet_MaxSpeed->hide();
        ui->LE_RadarSet_MaxSpeed->hide();
        ui->LBL_RadarSet_MinSpeed->hide();
        ui->LE_RadarSet_MinSpeed->hide();
        ui->LBL_RadarSet_VioSpeed->show();
        ui->LBL_RadarSet_VioSpeed->setText("Trip Speed");
        ui->LE_RadarSet_VioSpeed->show();
        ui->LBL_RadarSet_TouchMsg->show();

        tempstring = QString("%1 ").arg(f_radar_settings.fGetLegacySpeed()) + ((ui->PB_RadarSet_Units->text() == "MPH") ? QString("MPH") : QString("KPH"));
        ui->LE_RadarSet_VioSpeed->setText(tempstring);

        ui->GV_RadarSet_Msg1->hide();
        ui->GV_RadarSet_Msg2->hide();

        ui->GV_RadarSet_Msg3->show();
        ui->GV_RadarSet_Msg3->fSetMessage(QString::fromStdString(f_radar_settings.fGetLegacyMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,91,f_radar_settings.fGetLegacyMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg3->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg4->hide();
    }
    else if(ui->PB_RadarSet_Normal->isChecked())
    {
        ui->LBL_RadarSet_MaxSpeed->hide();
        ui->LE_RadarSet_MaxSpeed->hide();
        ui->LBL_RadarSet_MinSpeed->hide();
        ui->LE_RadarSet_MinSpeed->hide();
        ui->LBL_RadarSet_VioSpeed->hide();
        ui->LE_RadarSet_VioSpeed->hide();
        ui->GV_RadarSet_Msg1->hide();
        ui->GV_RadarSet_Msg2->hide();
        ui->GV_RadarSet_Msg3->hide();
        ui->GV_RadarSet_Msg4->hide();
        ui->LBL_RadarSet_TouchMsg->hide();
    }
    else if(ui->PB_RadarSet_Police->isChecked())
    {
        ui->PB_RadarSet_Police->show();
        ui->LBL_RadarSet_MaxSpeed->show();
        ui->LBL_RadarSet_TouchMsg->show();
        tempstring = QString("%1 ").arg(f_radar_settings.fGetPoliceSpeed().fGetMaximumSpeed()) + ((ui->PB_RadarSet_Units->text() == "MPH") ? QString("MPH") : QString("KPH"));
        ui->LE_RadarSet_MaxSpeed->show();
        ui->LE_RadarSet_MaxSpeed->setText(tempstring);
        ui->LBL_RadarSet_MinSpeed->show();
        tempstring = QString("%1 ").arg(f_radar_settings.fGetPoliceSpeed().fGetMinimumSpeed()) + ((ui->PB_RadarSet_Units->text() == "MPH") ? QString(" MPH") : QString(" KPH"));
        ui->LE_RadarSet_MinSpeed->show();
        ui->LE_RadarSet_MinSpeed->setText(tempstring);
        ui->LBL_RadarSet_VioSpeed->show();
        ui->LBL_RadarSet_VioSpeed->setText("Violator Speed");
        tempstring = QString("%1 ").arg(f_radar_settings.fGetPoliceSpeed().fGetViolatorSpeed()) + ((ui->PB_RadarSet_Units->text() == "MPH") ? QString("MPH") : QString("KPH"));
        ui->LE_RadarSet_VioSpeed->show();
        ui->LE_RadarSet_VioSpeed->setText(tempstring);
        ui->GV_RadarSet_Msg1->show();
        ui->GV_RadarSet_Msg2->show();
        ui->GV_RadarSet_Msg3->show();
        ui->GV_RadarSet_Msg4->show();

        ui->GV_RadarSet_Msg1->fSetMessage(QString::fromStdString(f_radar_settings.fGetUnderMinMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,92,f_radar_settings.fGetUnderMinMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg2->fSetMessage(QString::fromStdString(f_radar_settings.fGetUnderVioMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,93,f_radar_settings.fGetUnderVioMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg1->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg3->fSetMessage(QString::fromStdString(f_radar_settings.fGetOverVioMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,94,f_radar_settings.fGetOverVioMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg3->fSetActivationCode(temp_act_code);

        ui->GV_RadarSet_Msg4->fSetMessage(QString::fromStdString(f_radar_settings.fGetOverMaxMessage()));
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,95,f_radar_settings.fGetOverMaxMessage(),"127.0.0.1");
        ui->GV_RadarSet_Msg4->fSetActivationCode(temp_act_code);
    }
}

void HNS_Radar_Settings_Screen::my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &, const type_ui_message_mode &)
{
    string tempstring = multi.toStdString();
    if(f_message_target == ui->GV_RadarSet_Msg1)
    {
        f_radar_settings.fSetUnderMinMessage(tempstring);
        fUpdateRadarInputs();
    }
    else if(f_message_target == ui->GV_RadarSet_Msg2)
    {
        f_radar_settings.fSetUnderVioMessage(tempstring);
        fUpdateRadarInputs();
    }
    else if(f_message_target == ui->GV_RadarSet_Msg3)
    {
        if(ui->PB_RadarSet_Legacy->isChecked())
        {
            f_radar_settings.fSetLegacyMessage(tempstring);
        }
        else if(ui->PB_RadarSet_Police->isChecked())
        {
            f_radar_settings.fSetOverVioMessage(tempstring);
        }
        fUpdateRadarInputs();
    }
    else if(f_message_target == ui->GV_RadarSet_Msg4)
    {
        f_radar_settings.fSetOverMaxMessage(tempstring);
        fUpdateRadarInputs();
    }
}

void HNS_Radar_Settings_Screen::on_PB_RadarSet_Save_clicked()
{
    type_radar_mode mode = HNS_RADAR_MODE_NORMAL;
    type_radar_units units;
    bool valid = true;

    if(ui->PB_RadarSet_Legacy->isChecked())
    {
        mode = HNS_RADAR_MODE_LEGACY;
    }
    else if(ui->PB_RadarSet_Police->isChecked())
    {
        mode = HNS_RADAR_MODE_POLICE;

        if(f_radar_settings.fGetPoliceSpeed().fGetMinimumSpeed() > f_radar_settings.fGetPoliceSpeed().fGetViolatorSpeed())
        {
            valid = false;
        }
        else if(f_radar_settings.fGetPoliceSpeed().fGetViolatorSpeed() > f_radar_settings.fGetPoliceSpeed().fGetMaximumSpeed())
        {
            valid = false;
        }
    }

    if(ui->PB_RadarSet_Units->text() == "MPH")
    {
        units = HNS_RADAR_UNITS_MPH;
    }
    else
    {
        units = HNS_RADAR_UNITS_KPH;
    }

    if(valid)
    {
        emit SIG_Radar_Change_Request(mode,units,f_radar_settings.fGetLegacySpeed(),f_radar_settings.fGetPoliceSpeed(),
                                      QString::fromStdString(f_radar_settings.fGetLegacyMessage()),
                                      QString::fromStdString(f_radar_settings.fGetUnderMinMessage()),
                                      QString::fromStdString(f_radar_settings.fGetUnderVioMessage()),
                                      QString::fromStdString(f_radar_settings.fGetOverVioMessage()),
                                      QString::fromStdString(f_radar_settings.fGetOverMaxMessage()));

        deleteLater();
    }
    else
    {
        ui->LBL_RadarSet_Invalid->show();
        QTimer::singleShot(3000,this,&HNS_Radar_Settings_Screen::my_slot_Clear_Invalid);
    }
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Back_clicked()
{
    deleteLater();
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Strobes_clicked()
{
    fShowScreen(new HNS_Strobe_Settings_Screen(this, f_machine));
}

bool HNS_Radar_Settings_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_RadarSet_Msg1)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            on_GV_RadarMsg1_Clicked();
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_RadarSet_Msg2)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            on_GV_RadarMsg2_Clicked();
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_RadarSet_Msg3)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            on_GV_RadarMsg3_Clicked();
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_RadarSet_Msg4)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            on_GV_RadarMsg4_Clicked();
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_RadarSet_MaxSpeed)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->LE_RadarSet_MaxSpeed->setText("");
            fShowKP(ui->LE_RadarSet_MaxSpeed,580,330);
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_RadarSet_MinSpeed)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->LE_RadarSet_MinSpeed->setText("");
            fShowKP(ui->LE_RadarSet_MinSpeed,390,330);
        }
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_RadarSet_VioSpeed)
    {
        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->LE_RadarSet_VioSpeed->setText("");
            fShowKP(ui->LE_RadarSet_VioSpeed,240,330);
        }
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKP();
        ui->LE_RadarSet_MaxSpeed->clearFocus();
        ui->LE_RadarSet_MinSpeed->clearFocus();
        ui->LE_RadarSet_VioSpeed->clearFocus();
        return true;
    }
    return false;
}

void HNS_Radar_Settings_Screen::on_GV_RadarMsg1_Clicked()
{
    if(ui->PB_RadarSet_Police->isChecked())
    {
        fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER,QString::fromStdString(f_radar_settings.fGetUnderMinMessage())));
        f_message_target = ui->GV_RadarSet_Msg1;
    }
}

void HNS_Radar_Settings_Screen::on_GV_RadarMsg2_Clicked()
{
    if(ui->PB_RadarSet_Police->isChecked())
    {
        fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER,QString::fromStdString(f_radar_settings.fGetUnderVioMessage())));
        f_message_target = ui->GV_RadarSet_Msg2;
    }
}

void HNS_Radar_Settings_Screen::on_GV_RadarMsg3_Clicked()
{

    if(ui->PB_RadarSet_Police->isChecked())
    {
        fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER,QString::fromStdString(f_radar_settings.fGetOverVioMessage())));
        f_message_target = ui->GV_RadarSet_Msg3;
    }
    else if(ui->PB_RadarSet_Legacy->isChecked())
    {
        fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER,QString::fromStdString(f_radar_settings.fGetLegacyMessage())));
        f_message_target = ui->GV_RadarSet_Msg3;
    }
}

void HNS_Radar_Settings_Screen::on_GV_RadarMsg4_Clicked()
{
    if(ui->PB_RadarSet_Police->isChecked())
    {
        fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER,QString::fromStdString(f_radar_settings.fGetOverMaxMessage())));
        f_message_target = ui->GV_RadarSet_Msg4;
    }
}

void HNS_Radar_Settings_Screen::fVKP_Finished(const bool &enter_pressed)
{
    QString tempstring;
    if(fGetKPFocus() == ui->LE_RadarSet_MaxSpeed)
    {
        if(enter_pressed)
        {
            tempstring = ui->LE_RadarSet_MaxSpeed->text();
            f_radar_settings.fPoliceSpeed().fSetMaximumSpeed(tempstring.toInt());
            tempstring += ((ui->PB_RadarSet_Units->text() == "MPH") ? QString(" MPH") : QString(" KPH"));
            ui->LE_RadarSet_MaxSpeed->setText(tempstring);
        }
        else
        {
            fUpdateRadarInputs();
        }
    }
    else if(fGetKPFocus() == ui->LE_RadarSet_MinSpeed)
    {
        if(enter_pressed)
        {
            tempstring = ui->LE_RadarSet_MinSpeed->text();
            f_radar_settings.fPoliceSpeed().fSetMinimumSpeed(tempstring.toInt());
            tempstring += ((ui->PB_RadarSet_Units->text() == "MPH") ? QString(" MPH") : QString(" KPH"));
            ui->LE_RadarSet_MinSpeed->setText(tempstring);
        }
        else
        {
            fUpdateRadarInputs();
        }
    }
    else if(fGetKPFocus() == ui->LE_RadarSet_VioSpeed)
    {
        if(enter_pressed)
        {
            tempstring = ui->LE_RadarSet_VioSpeed->text();
            if(ui->PB_RadarSet_Police->isChecked())
            {
                f_radar_settings.fPoliceSpeed().fSetViolatorSpeed(tempstring.toInt());
            }
            else
            {
                f_radar_settings.fSetLegacySpeed(tempstring.toInt());
            }
            tempstring += ((ui->PB_RadarSet_Units->text() == "MPH") ? QString(" MPH") : QString(" KPH"));
            ui->LE_RadarSet_VioSpeed->setText(tempstring);
        }
        else
        {
            fUpdateRadarInputs();
        }
    }
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Normal_clicked()
{
    ui->PB_RadarSet_Legacy->setChecked(false);
    ui->PB_RadarSet_Normal->setChecked(true);
    ui->PB_RadarSet_Police->setChecked(false);
    fUpdateRadarInputs();
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Legacy_clicked()
{
    ui->PB_RadarSet_Legacy->setChecked(true);
    ui->PB_RadarSet_Normal->setChecked(false);
    ui->PB_RadarSet_Police->setChecked(false);
    fUpdateRadarInputs();
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Police_clicked()
{
    ui->PB_RadarSet_Legacy->setChecked(false);
    ui->PB_RadarSet_Normal->setChecked(false);
    ui->PB_RadarSet_Police->setChecked(true);
    fUpdateRadarInputs();
}


void HNS_Radar_Settings_Screen::on_PB_RadarSet_Units_clicked()
{
    if(ui->PB_RadarSet_Units->text() == "MPH")
    {
        ui->PB_RadarSet_Units->setText("KPH");
    }
    else
    {
        ui->PB_RadarSet_Units->setText("MPH");
    }

    fUpdateRadarInputs();
}

void HNS_Radar_Settings_Screen::my_slot_Clear_Invalid()
{
    ui->LBL_RadarSet_Invalid->hide();
}

