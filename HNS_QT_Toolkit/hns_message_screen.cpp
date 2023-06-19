#include "hns_message_screen.h"
#include "ui_hns_message_screen.h"

#include "hns_message_edit_screen.h"
#include "hns_message_library_select_screen.h"

HNS_Message_Screen::HNS_Message_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Message_Screen)
{
    ui->setupUi(this);

    if(f_machine->fGetAccess() < HNS_VIEWER)
    {
        ui->PB_Message_Create->show();
        ui->PB_Message_OpenLibrary->setText("CHOOSE FROM\nSAVED LIBRARY");
    }
    else
    {
        ui->PB_Message_Create->hide();
        ui->PB_Message_OpenLibrary->setText("BROWSE\nSAVED LIBRARY");
    }
}

HNS_Message_Screen::~HNS_Message_Screen()
{
    delete ui;
}

type_widget HNS_Message_Screen::fGetType() const
{
    return HNS_WIDGET_MESSAGE;
}

void HNS_Message_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

void HNS_Message_Screen::my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode)
{
    emit SIG_Message(multi,id_code,mode);

    emit SIG_GO_HOME();
}

void HNS_Message_Screen::on_PB_Message_Create_clicked()
{
    HNS_Message_Edit_Screen *temp_widget = new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_NORMAL);
    fShowScreen(temp_widget);
}


void HNS_Message_Screen::on_PB_Message_OpenLibrary_clicked()
{
    fShowScreen(new HNS_Message_Library_Select_Screen(this, f_machine));
}


void HNS_Message_Screen::on_PB_Message_Back_clicked()
{
    emit SIG_GO_HOME();
}

