#include "hns_admin_screen.h"
#include "ui_hns_admin_screen.h"

#include "hns_debug_screen.h"
#include "hns_trailer_screen.h"
#include "hns_change_login_screen.h"
#include "hns_confirm_screen.h"

HNS_Admin_Screen::HNS_Admin_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Admin_Screen)
{
    ui->setupUi(this);

    fUpdate();
}

HNS_Admin_Screen::~HNS_Admin_Screen()
{
    delete ui;
}

void HNS_Admin_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Admin_Screen::fGetType() const
{
    return HNS_WIDGET_ADMIN;
}

void HNS_Admin_Screen::fUpdate(const int &update_flags)
{
    if(f_machine->fGetLockRequired())
    {
        ui->PB_Admin_Lock->show();
        ui->PB_Admin_Lock->setText(f_machine->fGetLockState() ? "LOCKED":"UNLOCKED");
    }
    else
    {
        ui->PB_Admin_Lock->hide();
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_Admin_Screen::on_PB_Admin_Cancel_clicked()
{
    deleteLater();
}


void HNS_Admin_Screen::on_PB_Admin_Back_clicked()
{
    deleteLater();
}


void HNS_Admin_Screen::on_PB_Admin_Debug_clicked()
{
    fShowScreen(new HNS_Debug_Screen(this,f_machine));
}


void HNS_Admin_Screen::on_PB_Admin_Trailer_clicked()
{
    fShowScreen(new HNS_Trailer_Screen(this, f_machine));
}


void HNS_Admin_Screen::on_PB_Admin_Lock_clicked()
{
    emit SIG_Lock_Request();
}


void HNS_Admin_Screen::on_PB_Admin_Pass_clicked()
{
    fShowScreen(new HNS_Change_Login_Screen(this, f_machine));
}


void HNS_Admin_Screen::on_PB_Admin_Update_clicked()
{
    emit SIG_Update_Request();
}


void HNS_Admin_Screen::on_PB_Admin_Reset_clicked()
{
    HNS_Confirm_Screen *temp_widget = new HNS_Confirm_Screen(this,f_machine,"PLEASE CONFIRM FACTORY RESET","RESET","CANCEL");

    fShowScreen(temp_widget);

    connect(temp_widget,SIGNAL(SIG_Confirm_Closed(bool)),this,SLOT(my_slot_Reset_Confirmation(bool)));
}

void HNS_Admin_Screen::my_slot_Reset_Confirmation(const bool &confirm)
{
    if(confirm)
    {
        emit SIG_Reset_Request();
    }
}

