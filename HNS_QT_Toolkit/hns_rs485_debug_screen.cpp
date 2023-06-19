#include "hns_rs485_debug_screen.h"
#include "ui_hns_rs485_debug_screen.h"

HNS_RS485_Debug_Screen::HNS_RS485_Debug_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_RS485_Debug_Screen)
{
    ui->setupUi(this);
}

HNS_RS485_Debug_Screen::~HNS_RS485_Debug_Screen()
{
    delete ui;
}

void HNS_RS485_Debug_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_RS485_Debug_Screen::fGetType() const
{
    return HNS_WIDGET_RS485_DEBUG;
}

void HNS_RS485_Debug_Screen::on_PB_Db485_Cancel_clicked()
{
    deleteLater();
}

