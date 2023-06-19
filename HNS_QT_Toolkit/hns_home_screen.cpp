#include "hns_home_screen.h"
#include "ui_hns_home_screen.h"

#include "hns_message_edit_screen.h"
#include "hns_message_screen.h"
#include "hns_schedule_edit_screen.h"

#include <QGraphicsPixmapItem>
#include <QImage>

#include <hns_qt_toolkit.h>

using namespace std;

HNS_Home_Screen::HNS_Home_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Home_Screen)
  , f_current_access(HNS_USER)
{
    ui->setupUi(this);

    ui->GV_HomePreview->installEventFilter(this);

    if(f_machine->fGetAccess() == HNS_VIEWER)
    {
        ui->PB_Home_StopMessage->hide();
        ui->PB_Home_NewSchedule->hide();
        ui->PB_Home_CreateMessage->hide();
        ui->PB_Home_EditMessage->hide();
    }
    else
    {
        ui->PB_Home_StopMessage->show();
        ui->PB_Home_NewSchedule->show();
        ui->PB_Home_CreateMessage->show();
        ui->PB_Home_EditMessage->show();
    }
}

HNS_Home_Screen::~HNS_Home_Screen()
{
    delete ui;
}

void HNS_Home_Screen::fSetAccess(const type_user_level &access)
{
    f_current_access = access;
}

type_widget HNS_Home_Screen::fGetType() const
{
    return HNS_WIDGET_HOME;
}

void HNS_Home_Screen::fSetLogo(const QImage &logo)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    QImage icon = logo;
    icon = icon.scaled(48,48);
    scene->addPixmap(QPixmap::fromImage(icon))->setOffset(0,0);

    ui->GV_Home_Logo->setScene(scene);
    ui->GV_Home_Logo->setSceneRect(0,0,48,48);
    ui->GV_Home_Logo->setObjectName("Glyph");
    scene->update();
}

void HNS_Home_Screen::fSetSignBrd(const HNS_SignBoard &sign_board)
{
    ui->GV_HomePreview->fSetSignBoard(sign_board);
}

void HNS_Home_Screen::fUpdate(const int &update_flags)
{
    if(update_flags & HNS_DATA_NEW_ACTIVATION || update_flags & HNS_DATA_NEW_DISPLAY)
    {
        getActiveSchedule();

        if(f_machine->fGetRadarConnected())
        {
            ui->PGE_Radar_Info->show();
        }
        else
        {
            ui->PGE_Radar_Info->hide();
        }
        ui->PGE_Radar_Info->fSetRadarInfo(f_machine->fGetRadarMode(),f_machine->fGetRadarLegacySpeed(),f_machine->fGetRadarPoliceSpeed(),f_machine->fGetStrobeSettings());

        HNS_Message2 temp_message(HNS_Message2::fGetDefaultSignBoardInfo());
        string tempstring = f_machine->fGetCurrentMessage();
        temp_message.fSetMULTI(tempstring,f_machine->fGetFonts(),f_machine->fGetGraphics(),nullptr);

        ui->LBL_Home_Page->setText(QString("Pages: %1").arg(temp_message.fGetNumPages()));
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

/**
 * @brief MainWindow::getActiveSchedule - This function is responsible for check the current active schedule.
 */
void HNS_Home_Screen::getActiveSchedule()
{
    QDateTime currentDate = QDateTime::currentDateTime();
    if(f_machine->fGetSchedules().size()==0)
    {
        ui->PGE_Current_Schedule->fClear();
    }
    for(auto schedule : f_machine->fGetSchedules())
    {
        QDateTime startDate(ConvertSTLDatetoQDate(schedule.fGetStartDate()), ConvertSTLTimetoQTime(schedule.fGetStartTime()));
        QDateTime endDate(ConvertSTLDatetoQDate(schedule.fGetEndDate()), ConvertSTLTimetoQTime(schedule.fGetStopTime()));
        if(startDate <= currentDate && currentDate <= endDate)
        {
            ui->PGE_Current_Schedule->fSetSchedule(schedule);
        }
        else
        {
            ui->PGE_Current_Schedule->fClear();
        }
    }
}

void HNS_Home_Screen::my_Slot_Home_Preview_clicked()
{
    emit SIG_Message_Preview_Touched();
}

bool HNS_Home_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_HomePreview)
    {
        if(f_current_access < HNS_VIEWER)
        {
            my_Slot_Home_Preview_clicked();
        }
        return true;
    }
    return false;
}

void HNS_Home_Screen::on_PB_Home_EditMessage_clicked()
{
    fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_EXISTING, QString::fromStdString(f_machine->fGetCurrentMessage())));
}


void HNS_Home_Screen::on_PB_Home_CreateMessage_clicked()
{
    fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_NORMAL));
}


void HNS_Home_Screen::on_PB_Home_StopMessage_clicked()
{
    emit SIG_Msg_Stop_Request();
}


void HNS_Home_Screen::on_PB_Home_NewSchedule_clicked()
{
    fShowScreen(new HNS_Schedule_Edit_Screen(this, f_machine));
}

