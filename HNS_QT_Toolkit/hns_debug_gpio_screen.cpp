#include "hns_debug_gpio_screen.h"
#include "ui_hns_debug_gpio_screen.h"

HNS_Debug_GPIO_Screen::HNS_Debug_GPIO_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Debug_GPIO_Screen)
{
    ui->setupUi(this);

    fUpdate();
}

HNS_Debug_GPIO_Screen::~HNS_Debug_GPIO_Screen()
{
    delete ui;
}

void HNS_Debug_GPIO_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Debug_GPIO_Screen::fGetType() const
{
    return HNS_WIDGET_GPIO_DEBUG;
}

void HNS_Debug_GPIO_Screen::fUpdate(const int &update_flags)
{
    if(update_flags & HNS_DATA_NEW_GPIO)
    {
        ui->PB_Debug_GPIO_43->setChecked(f_machine->fGetGPIO(43) != 0);
        ui->PB_Debug_GPIO_44->setChecked(f_machine->fGetGPIO(44) != 0);
        ui->PB_Debug_GPIO_45->setChecked(f_machine->fGetGPIO(45) != 0);
        ui->PB_Debug_GPIO_46->setChecked(f_machine->fGetGPIO(46) != 0);

        ui->PB_Debug_GPIO_11->setChecked(f_machine->fGetGPIO(11) != 0);
        ui->PB_Debug_GPIO_12->setChecked(f_machine->fGetGPIO(12) != 0);
        ui->PB_Debug_GPIO_13->setChecked(f_machine->fGetGPIO(13) != 0);
        ui->PB_Debug_GPIO_14->setChecked(f_machine->fGetGPIO(14) != 0);
    }
}

void HNS_Debug_GPIO_Screen::on_PB_Debug_GPIO_Cancel_clicked()
{
    deleteLater();
}

void HNS_Debug_GPIO_Screen::on_PB_Debug_GPIO_43_clicked(bool checked)
{
    emit SIG_GPIO_Change_Request(43,checked ? 1 : 0);
}

void HNS_Debug_GPIO_Screen::on_PB_Debug_GPIO_44_clicked(bool checked)
{
    emit SIG_GPIO_Change_Request(44,checked ? 1 : 0);
}

void HNS_Debug_GPIO_Screen::on_PB_Debug_GPIO_45_clicked(bool checked)
{
    emit SIG_GPIO_Change_Request(45,checked ? 1 : 0);
}

void HNS_Debug_GPIO_Screen::on_PB_Debug_GPIO_46_clicked(bool checked)
{
    emit SIG_GPIO_Change_Request(46,checked ? 1 : 0);
}
