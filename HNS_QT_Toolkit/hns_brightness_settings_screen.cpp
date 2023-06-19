#include "hns_brightness_settings_screen.h"
#include "ui_hns_brightness_settings_screen.h"

HNS_Brightness_Settings_Screen::HNS_Brightness_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Brightness_Settings_Screen)
{
    ui->setupUi(this);

    ui->PB_BrightSet_Auto->setChecked(f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_AUTO);
    ui->PB_BrightSet_Manual->setChecked(f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL);

    ui->PB_BrightSet_Lv1->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 1));
    ui->PB_BrightSet_Lv2->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 2));
    ui->PB_BrightSet_Lv3->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 3));
    ui->PB_BrightSet_Lv4->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 4));
    ui->PB_BrightSet_Lv5->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 5));
    ui->PB_BrightSet_Lv6->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 6));
    ui->PB_BrightSet_Lv7->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 7));
    ui->PB_BrightSet_Lv8->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 8));
    ui->PB_BrightSet_Lv9->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 9));
    ui->PB_BrightSet_Lv10->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 10));
    ui->PB_BrightSet_Lv11->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 11));
    ui->PB_BrightSet_Lv12->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 12));
    ui->PB_BrightSet_Lv13->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 13));
    ui->PB_BrightSet_Lv14->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 14));
    ui->PB_BrightSet_Lv15->setChecked((f_machine->fGetBrightnessMode() == HNS_BRIGHTNESS_MANUAL) && (f_machine->fGetBrightnessLevel() == 15));
}

HNS_Brightness_Settings_Screen::~HNS_Brightness_Settings_Screen()
{
    delete ui;
}

void HNS_Brightness_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Brightness_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_BRIGHTNESS_SETTINGS;
}

void HNS_Brightness_Settings_Screen::fSetBrightnessButtons(const int &brightness)
{
    ui->PB_BrightSet_Lv1->setChecked(brightness == 1);
    ui->PB_BrightSet_Lv2->setChecked(brightness == 2);
    ui->PB_BrightSet_Lv3->setChecked(brightness == 3);
    ui->PB_BrightSet_Lv4->setChecked(brightness == 4);
    ui->PB_BrightSet_Lv5->setChecked(brightness == 5);
    ui->PB_BrightSet_Lv6->setChecked(brightness == 6);
    ui->PB_BrightSet_Lv7->setChecked(brightness == 7);
    ui->PB_BrightSet_Lv8->setChecked(brightness == 8);
    ui->PB_BrightSet_Lv9->setChecked(brightness == 9);
    ui->PB_BrightSet_Lv10->setChecked(brightness == 10);
    ui->PB_BrightSet_Lv11->setChecked(brightness == 11);
    ui->PB_BrightSet_Lv12->setChecked(brightness == 12);
    ui->PB_BrightSet_Lv13->setChecked(brightness == 13);
    ui->PB_BrightSet_Lv14->setChecked(brightness == 14);
    ui->PB_BrightSet_Lv15->setChecked(brightness == 15);
}

int HNS_Brightness_Settings_Screen::fGetBrightnessLevelFromButtons()
{
    if(ui->PB_BrightSet_Lv1->isChecked())
    {
        return 1;
    }
    else if(ui->PB_BrightSet_Lv2->isChecked())
    {
        return 2;
    }
    else if(ui->PB_BrightSet_Lv3->isChecked())
    {
        return 3;
    }
    else if(ui->PB_BrightSet_Lv4->isChecked())
    {
        return 4;
    }
    else if(ui->PB_BrightSet_Lv5->isChecked())
    {
        return 5;
    }
    else if(ui->PB_BrightSet_Lv6->isChecked())
    {
        return 6;
    }
    else if(ui->PB_BrightSet_Lv7->isChecked())
    {
        return 7;
    }
    else if(ui->PB_BrightSet_Lv8->isChecked())
    {
        return 8;
    }
    else if(ui->PB_BrightSet_Lv9->isChecked())
    {
        return 9;
    }
    else if(ui->PB_BrightSet_Lv10->isChecked())
    {
        return 10;
    }
    else if(ui->PB_BrightSet_Lv11->isChecked())
    {
        return 11;
    }
    else if(ui->PB_BrightSet_Lv12->isChecked())
    {
        return 12;
    }
    else if(ui->PB_BrightSet_Lv13->isChecked())
    {
        return 13;
    }
    else if(ui->PB_BrightSet_Lv14->isChecked())
    {
        return 14;
    }
    else if(ui->PB_BrightSet_Lv15->isChecked())
    {
        return 15;
    }
    else
    {
        return 0;
    }
}

void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Save_clicked()
{
    emit SIG_Brightness_Change_Request(ui->PB_BrightSet_Auto->isChecked() ? HNS_BRIGHTNESS_AUTO : HNS_BRIGHTNESS_MANUAL,fGetBrightnessLevelFromButtons());
    deleteLater();
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Back_clicked()
{
    deleteLater();
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Auto_clicked()
{
    ui->PB_BrightSet_Auto->setChecked(true);
    ui->PB_BrightSet_Manual->setChecked(false);
    fSetBrightnessButtons(0);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Manual_clicked()
{
    ui->PB_BrightSet_Auto->setChecked(false);
    ui->PB_BrightSet_Manual->setChecked(true);
    fSetBrightnessButtons(f_machine->fGetBrightnessLevel());
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv1_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(1);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv2_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(2);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv3_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(3);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv4_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(4);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv5_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(5);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv6_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(6);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv7_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(7);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv8_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(8);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv9_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(9);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv10_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(10);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv11_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(11);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv12_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(12);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv13_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(13);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv14_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(14);
}


void HNS_Brightness_Settings_Screen::on_PB_BrightSet_Lv15_clicked()
{
    on_PB_BrightSet_Manual_clicked();
    fSetBrightnessButtons(15);
}

