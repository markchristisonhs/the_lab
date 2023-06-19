#include "hns_ttmb_home_screen.h"
#include "ui_hns_ttmb_home_screen.h"
#include "hns_tm_custom_msg.h"

using namespace std;
using namespace NTCIP_MESSAGE;

HNS_TTMB_Home_Screen::HNS_TTMB_Home_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_TTMB_Home_Screen)
{
    ui->setupUi(this);
}

HNS_TTMB_Home_Screen::~HNS_TTMB_Home_Screen()
{
    delete ui;
}

void HNS_TTMB_Home_Screen::fSetAccess(const type_user_level &)
{

}

void HNS_TTMB_Home_Screen::fSetSignBrd(const HNS_SignBoard &sign_board)
{
    ui->GV_HomePreview->fSetSignBoard(sign_board);
}

void HNS_TTMB_Home_Screen::on_PB_Raise_Sign_clicked()
{
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_Blank_Sign_clicked()
{
    emit SIG_Msg_Stop_Request();
}


void HNS_TTMB_Home_Screen::on_PB_Blank_Lower_clicked()
{
    emit SIG_Msg_Stop_Request();

    emit SIG_GPIO_Change_Request(46,0);
}


void HNS_TTMB_Home_Screen::on_PB_LeftArrow_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,263,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_RightArrow_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,262,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_DoubleArrow_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,269,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_StoppedTraffic_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,215,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_RoadWork_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,171,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_PaintCrew_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code;

    multi = QString::fromStdString(f_machine->fGetMessage(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,141,"",&code));

    id_code.fSetCRC(code.fGetCRCAsVector());
    id_code.fSetMemoryType(code.fGetMemoryType());
    id_code.fSetMessageNumber(code.fGetMessageNo());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_Custom1_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code = f_machine->fGetTMCustomMsg(0);
    multi = QString::fromStdString(f_machine->fGetMessage(id_code,&code));
    id_code.fSetCRC(code.fGetCRCAsVector());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_Custom2_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code = f_machine->fGetTMCustomMsg(1);
    multi = QString::fromStdString(f_machine->fGetMessage(id_code,&code));
    id_code.fSetCRC(code.fGetCRCAsVector());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_Custom3_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code = f_machine->fGetTMCustomMsg(2);
    multi = QString::fromStdString(f_machine->fGetMessage(id_code,&code));
    id_code.fSetCRC(code.fGetCRCAsVector());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_Custom4_clicked()
{
    QString multi;
    HNS_NTCIP_MessageActivationCode code;
    HNS_NTCIP_MessageIDCode id_code = f_machine->fGetTMCustomMsg(3);
    multi = QString::fromStdString(f_machine->fGetMessage(id_code,&code));
    id_code.fSetCRC(code.fGetCRCAsVector());

    emit SIG_Message(multi,id_code,HNS_PLAY_MESSAGE);
    emit SIG_GPIO_Change_Request(46,1);
}


void HNS_TTMB_Home_Screen::on_PB_SetCustom_clicked()
{
    HNS_TM_Custom_Msg *temp_widget = new HNS_TM_Custom_Msg(this, f_machine);

    fShowScreen(temp_widget);
}
