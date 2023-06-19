#include "hns_strobe_settings_screen.h"
#include "ui_hns_strobe_settings_screen.h"

HNS_Strobe_Settings_Screen::HNS_Strobe_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, const HNS_Radar_Strobe *strobe_settings, const int &) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Strobe_Settings_Screen)
{
    HNS_Radar_Strobe strobesettings;

    if(strobe_settings != nullptr)
    {
        strobesettings = *strobe_settings;
    }
    else if(f_machine != nullptr)
    {
        strobesettings = f_machine->fGetStrobeSettings();
    }

    ui->setupUi(this);

    ui->PB_StrobeSet_SpeedMode->hide();
    ui->PB_StrobeSet_SpeedMode->setChecked(false);

    ui->PB_StrobeSet_Enable->setChecked(strobesettings.fIsEnabled());
    ui->PB_StrobeSet_Enable->setText(strobesettings.fIsEnabled() ? "ENABLED" : "DISABLED");

    ui->PB_StrobeSet_Enable->setChecked(strobesettings.fIsEnabled());
    ui->LE_StrobeSet_Duration->setText(QString::number(strobesettings.fGetDuration()));
    ui->LE_StrobeSet_Pattern->setText(QString::number(strobesettings.fGetPatternUI()));
    if(strobesettings.fGetSpeedMode() == HNS_RADAR_NORMAL)
    {
        ui->LE_StrobeSet_Speed->setText(QString::number(strobesettings.fGetSpeed()));
    }
    else if(strobesettings.fGetSpeedMode() == HNS_VSL_OFFSET)
    {
        ui->LE_StrobeSet_Speed->setText(QString::number(strobesettings.fGetSpeedOffset()));
    }
    else
    {
        ui->LE_StrobeSet_Speed->setText(QString::number(strobesettings.fGetSpeed()));
    }

    if(f_machine->fGetAccess() < HNS_VIEWER)
    {
        ui->PB_StrobeSet_Save->show();
        ui->PB_StrobeSet_Duration_Dn->show();
        ui->PB_StrobeSet_Duration_Up->show();
        ui->PB_StrobeSet_Pattern_Dn->show();
        ui->PB_StrobeSet_Pattern_Up->show();
        ui->PB_StrobeSet_Speed_Dn->show();
        ui->PB_StrobeSet_Speed_Up->show();
        ui->PB_StrobeSet_Test->show();
    }
    else
    {
        ui->PB_StrobeSet_Save->hide();
        ui->PB_StrobeSet_Duration_Dn->hide();
        ui->PB_StrobeSet_Duration_Up->hide();
        ui->PB_StrobeSet_Pattern_Dn->hide();
        ui->PB_StrobeSet_Pattern_Up->hide();
        ui->PB_StrobeSet_Speed_Dn->hide();
        ui->PB_StrobeSet_Speed_Up->hide();
        ui->PB_StrobeSet_Test->hide();
    }
}

HNS_Strobe_Settings_Screen::~HNS_Strobe_Settings_Screen()
{
    delete ui;
}

void HNS_Strobe_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Strobe_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_STROBE_SETTINGS;
}

void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Save_clicked()
{
    HNS_Radar_Strobe strobesettings;

    strobesettings.fSetDuration(ui->LE_StrobeSet_Duration->text().toInt());
    strobesettings.fSetEnabled(ui->PB_StrobeSet_Enable->isChecked());
    strobesettings.fSetSpeedMode(HNS_RADAR_NORMAL);
    if(ui->PB_StrobeSet_SpeedMode->isVisible() && ui->PB_StrobeSet_SpeedMode->isChecked())
    {
        strobesettings.fSetSpeedOffset(ui->LE_StrobeSet_Speed->text().toInt());
    }
    else
    {
        strobesettings.fSetSpeed(ui->LE_StrobeSet_Speed->text().toInt());
    }
    strobesettings.fSetPatternUI(ui->LE_StrobeSet_Pattern->text().toInt());

    emit SIG_Strobe_Change_Request(strobesettings);

    deleteLater();
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Test_clicked()
{
    emit SIG_Strobe_Test();
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Speed_Up_clicked()
{
    int speed = ui->LE_StrobeSet_Speed->text().toInt();
    if(ui->PB_StrobeSet_SpeedMode->isChecked())
    {
        if(speed <= 50)
        {
            speed += 5;
        }
        else
        {
            speed = -55;
        }
    }
    else
    {
        if(speed <= 150)
        {
            speed += 5;
        }
        else
        {
            speed = 5;
        }
    }
    ui->LE_StrobeSet_Speed->setText(QString::number(speed));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Speed_Dn_clicked()
{
    int speed = ui->LE_StrobeSet_Speed->text().toInt();
    if(ui->PB_StrobeSet_SpeedMode->isChecked())
    {
        if(speed >= -50)
        {
            speed-=5;
        }
        else
        {
            speed = 55;
        }
    }
    else
    {
        if(speed >= 10)
        {
            speed-=5;
        }
        else
        {
            speed = 155;
        }
    }
    ui->LE_StrobeSet_Speed->setText(QString::number(speed));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Duration_Up_clicked()
{
    int duration = ui->LE_StrobeSet_Duration->text().toInt();

    if(duration >= 9)
    {
        duration = 1;
    }
    else
    {
        duration++;
    }
    ui->LE_StrobeSet_Duration->setText(QString::number(duration));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Duration_Dn_clicked()
{
    int duration = ui->LE_StrobeSet_Duration->text().toInt();

    if(duration <= 1)
    {
        duration = 9;
    }
    else
    {
        duration--;
    }
    ui->LE_StrobeSet_Duration->setText(QString::number(duration));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Pattern_Up_clicked()
{
    int pattern = ui->LE_StrobeSet_Pattern->text().toInt();

    if(pattern >= 4)
    {
        pattern = 1;
    }
    else
    {
        pattern++;
    }
    ui->LE_StrobeSet_Pattern->setText(QString::number(pattern));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Pattern_Dn_clicked()
{
    int pattern = ui->LE_StrobeSet_Pattern->text().toInt();

    if(pattern <= 1)
    {
        pattern = 4;
    }
    else
    {
        pattern--;
    }
    ui->LE_StrobeSet_Pattern->setText(QString::number(pattern));
}


void HNS_Strobe_Settings_Screen::on_PB_StrobeSet_Enable_clicked(bool checked)
{
    if(f_machine->fGetAccess() < HNS_VIEWER)
    {
        ui->PB_StrobeSet_Enable->setText(checked ? "ENABLED" : "DISABLED");
    }
    ui->PB_StrobeSet_Enable->setChecked(ui->PB_StrobeSet_Enable->text() == "ENABLED");
}

