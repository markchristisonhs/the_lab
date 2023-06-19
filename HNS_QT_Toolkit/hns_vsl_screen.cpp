#include "hns_vsl_screen.h"
#include "ui_hns_vsl_screen.h"

HNS_VSL_Screen::HNS_VSL_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_VSL_Screen)
{
    ui->setupUi(this);

    if(!f_machine->fHasBeacons())
    {
        ui->PB_VSL_Beacons->hide();
    }
}

HNS_VSL_Screen::~HNS_VSL_Screen()
{
    delete ui;
}

void HNS_VSL_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_VSL_Screen::fGetType() const
{
    return HNS_WIDGET_VSL;
}

void HNS_VSL_Screen::fSetSignBrd(const HNS_SignBoard &sign_board)
{
    ui->GV_VSL_Preview->fSetSignBoard(sign_board);
}


void HNS_VSL_Screen::on_PB_VSL_Up_clicked()
{
    emit SIG_VSL_Up_Request();
}


void HNS_VSL_Screen::on_PB_VSL_Down_clicked()
{
    emit SIG_VSL_Dn_Request();
}


void HNS_VSL_Screen::on_PB_VSL_Beacons_clicked()
{
    if(ui->PB_VSL_Beacons->text() == "BEACONS\nOFF")
    {
        ui->PB_VSL_Beacons->setText("BEACONS\nON");
        emit SIG_VSL_Beacon_Request(VSL_BEACON_ON);
    }
    else if(ui->PB_VSL_Beacons->text() == "BEACONS\nON")
    {
        ui->PB_VSL_Beacons->setText("BEACONS\nAUTO");
        emit SIG_VSL_Beacon_Request(VSL_BEACON_AUTO);
    }
    else
    {
        ui->PB_VSL_Beacons->setText("BEACONS\nOFF");
        emit SIG_VSL_Beacon_Request(VSL_BEACON_OFF);
    }
}

int HNS_VSL_Screen::fGetBeaconMode()
{
    int result = VSL_BEACON_OFF;

    if(ui->PB_VSL_Beacons->text() == "BEACONS\nOFF")
    {
        result = VSL_BEACON_OFF;
    }
    else if(ui->PB_VSL_Beacons->text() == "BEACONS\nON")
    {
        result = VSL_BEACON_ON;
    }
    else if(ui->PB_VSL_Beacons->text() == "BEACONS\nAUTO")
    {
        result = VSL_BEACON_AUTO;
    }

    return result;
}

