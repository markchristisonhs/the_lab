#include "hns_tm_custom_msg.h"
#include "ui_hns_tm_custom_msg.h"

#include "hns_message_library.h"

HNS_TM_Custom_Msg::HNS_TM_Custom_Msg(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_TM_Custom_Msg)
  , f_index(-1)
{
    ui->setupUi(this);

    ui->GV_Custom_Preview1->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Custom_Preview1->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Custom_Preview2->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Custom_Preview2->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Custom_Preview3->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Custom_Preview3->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Custom_Preview4->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Custom_Preview4->fSetGraphics(*f_machine->fGetGraphics());

    fUpdate();
}

void HNS_TM_Custom_Msg::fUpdate(const int &update_flags)
{
    QString tempstring;
    if(update_flags & HNS_DATA_MESSAGE_CHANGE)
    {
        tempstring = QString::fromStdString(f_machine->fGetTMCustomMsgMULTI(0));
        ui->GV_Custom_Preview1->fSetMessage(tempstring);

        tempstring = QString::fromStdString(f_machine->fGetTMCustomMsgMULTI(1));
        ui->GV_Custom_Preview2->fSetMessage(tempstring);

        tempstring = QString::fromStdString(f_machine->fGetTMCustomMsgMULTI(2));
        ui->GV_Custom_Preview3->fSetMessage(tempstring);

        tempstring = QString::fromStdString(f_machine->fGetTMCustomMsgMULTI(3));
        ui->GV_Custom_Preview4->fSetMessage(tempstring);
    }
}

void HNS_TM_Custom_Msg::my_slot_Message(const QString &, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &)
{
    emit SIG_TM_Custom_Msg_Change_Request(f_index,id_code);

    f_index = -1;

    fUpdate();
}

HNS_TM_Custom_Msg::~HNS_TM_Custom_Msg()
{
    delete ui;
}

void HNS_TM_Custom_Msg::on_PB_Change_Custom1_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 0;
}


void HNS_TM_Custom_Msg::on_PB_Change_Custom2_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 1;
}


void HNS_TM_Custom_Msg::on_PB_Change_Custom3_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 2;
}


void HNS_TM_Custom_Msg::on_PB_Change_Custom4_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 3;
}


void HNS_TM_Custom_Msg::on_PB_Back_clicked()
{
    deleteLater();
}

