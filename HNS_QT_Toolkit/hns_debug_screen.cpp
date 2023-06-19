#include "hns_debug_screen.h"
#include "ui_hns_debug_screen.h"
#include "hns_debug_gpio_screen.h"
#include "hns_rs485_debug_screen.h"

HNS_Debug_Screen::HNS_Debug_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_Debug_Screen)
{
    ui->setupUi(this);
}

HNS_Debug_Screen::~HNS_Debug_Screen()
{
    delete ui;
}

void HNS_Debug_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Debug_Screen::fGetType() const
{
    return HNS_WIDGET_DEBUG;
}

void HNS_Debug_Screen::on_PB_Debug_RS485_clicked()
{
    fShowScreen(new HNS_RS485_Debug_Screen(this, f_machine));
}


void HNS_Debug_Screen::on_PB_Debug_GPIO_clicked()
{
    fShowScreen(new HNS_Debug_GPIO_Screen(this, f_machine));
}


void HNS_Debug_Screen::on_PB_Debug_Cancel_clicked()
{
    deleteLater();
}

