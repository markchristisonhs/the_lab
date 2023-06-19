#include "hns_preview_screen.h"
#include "ui_hns_preview_screen.h"

#include "hns_schedule_edit_screen.h"

#include <QDateTime>
#include <QTimer>

HNS_Preview_Screen::HNS_Preview_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_preview_mode &preview_mode, const QString &multi) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Preview_Screen)
//  , f_timer(new QTimer(this))
{
    ui->setupUi(this);

//    connect(f_timer,SIGNAL(timeout()),SLOT(fTickTock()));
//    f_timer->setInterval(100);
//    f_timer->start();

    ui->GV_Preview_Preview->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Preview_Preview->fSetGraphics(*f_machine->fGetGraphics());

//    ui->GV_Preview_Preview->fSetMessage(ui->GV_Edit_Preview->fGetCurrentMessage());
    ui->GV_Preview_Preview->fSetMessage(multi);
//    ui->GV_Preview_Preview->fSetActivationCode(ui->GV_Edit_Preview->fGetActivationCode());

    ui->GV_Preview_Preview->fStartPreview(QDateTime::currentMSecsSinceEpoch());
    if(preview_mode == HNS_PREVIEW_NORMAL)
    {
        ui->PB_Preview_SaveToLibrary->show();
        ui->PB_Preview_Edit->show();
        ui->PB_Preview_Schedule->show();
        ui->PB_Preview_Run->setText("SAVE AND RUN\nNOW");
    }
    else
    {
        ui->PB_Preview_SaveToLibrary->hide();
        ui->PB_Preview_Edit->hide();
        ui->PB_Preview_Schedule->hide();
        ui->PB_Preview_Run->setText("USE MESSAGE");
    }
}

HNS_Preview_Screen::~HNS_Preview_Screen()
{
    delete ui;
}

void HNS_Preview_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Preview_Screen::fGetType() const
{
    return HNS_WIDGET_PREVIEW;
}

void HNS_Preview_Screen::on_PB_Preview_SaveToLibrary_clicked()
{
    emit SIG_Preview_Add_Library();

    deleteLater();
}


void HNS_Preview_Screen::on_PB_Preview_Run_clicked()
{
    emit SIG_Preview_Accepted();

    deleteLater();
}


void HNS_Preview_Screen::on_PB_Preview_Schedule_clicked()
{
    fShowScreen(new HNS_Schedule_Edit_Screen(this, f_machine));
}


void HNS_Preview_Screen::on_PB_Preview_Edit_clicked()
{
    deleteLater();
}


void HNS_Preview_Screen::on_PB_Preview_Back_clicked()
{
    deleteLater();
}

