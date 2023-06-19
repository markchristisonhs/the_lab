#include "hns_pixout_screen.h"
#include "ui_hns_pixout_screen.h"

HNS_PixOut_Screen::HNS_PixOut_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_PixOut_Screen)
{
    ui->setupUi(this);

    fUpdate();
}

HNS_PixOut_Screen::~HNS_PixOut_Screen()
{
    delete ui;
}

void HNS_PixOut_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_PixOut_Screen::fGetType() const
{
    return HNS_WIDGET_PIXOUT;
}

void HNS_PixOut_Screen::fUpdate(const int &update_flags)
{
    if(update_flags & HNS_DATA_NEW_PIX_OUT)
    {
        if(f_machine->fGetPixOutRunning())
        {
            ui->PB_RunPixelOutTest->setText("RUNNING");
        }
        else
        {
            ui->PB_RunPixelOutTest->setText("RUN TEST");
        }

        ui->GV_PixelOut->fSetSignBoard(f_machine->fGetPixOutBrd());

        size_t sum = 0;

        for(size_t ui=0;ui<f_machine->fGetPixOutBrd().fGetNumBoards();ui++)
        {
            sum += f_machine->fGetPixOutBrd().fGetPixelsOut(ui).size();
        }

        ui->LBL_PixOutResult->setText(QString("PIX OUT COUNT:\n%1").arg(sum));
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_PixOut_Screen::on_PB_Diagnostic_Back_clicked()
{
    deleteLater();
}


void HNS_PixOut_Screen::on_PB_RunPixelOutTest_clicked()
{
    emit SIG_Run_PixOut();
}

