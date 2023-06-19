#include "hns_date_time_settings_screen.h"
#include "ui_hns_date_time_settings_screen.h"

#include <QDateTime>
#include <QTimeZone>

HNS_Date_Time_Settings_Screen::HNS_Date_Time_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Date_Time_Settings_Screen)
{
    QTimeZone time_zone = QTimeZone::systemTimeZone();
    int offset = time_zone.standardTimeOffset(QDateTime::currentDateTime());

    ui->setupUi(this);

    installEventFilter(this);

    ui->PB_DateSet_Time12_24->hide();
    ui->LBL_DateSet_12Hr_Title->hide();

    ui->PB_DateSet_Date->fSetDate(QDateTime::currentDateTime().date());
    ui->PB_DateSet_Time->fSetTime(QDateTime::currentDateTime().time());

    if(offset == -18000)
    {
        ui->PB_DateSet_TimeZone->setText("EST");
    }
    else if(offset == -21600)
    {
        ui->PB_DateSet_TimeZone->setText("CST");
    }
    else if(offset == -25200)
    {
        ui->PB_DateSet_TimeZone->setText("MST");
    }
    else if(offset == -28800)
    {
        ui->PB_DateSet_TimeZone->setText("PST");
    }
    else if(offset == -32400)
    {
        ui->PB_DateSet_TimeZone->setText("AST");
    }
    else if(offset == -36000)
    {
        ui->PB_DateSet_TimeZone->setText("HST");
    }
    else
    {
        ui->PB_DateSet_TimeZone->setText("CST");
    }

    ui->PB_DateSet_Date->setEnabled(f_machine->fGetAccess() < HNS_VIEWER);
    ui->PB_DateSet_Time->setEnabled(f_machine->fGetAccess() < HNS_VIEWER);
    ui->PB_DateSet_TimeZone->setEnabled(f_machine->fGetAccess() < HNS_VIEWER);

    if(f_machine->fGetAccess() < HNS_VIEWER)
    {
        ui->PB_DateSet_Save->show();
    }
    else
    {
        ui->PB_DateSet_Save->hide();
    }
}

HNS_Date_Time_Settings_Screen::~HNS_Date_Time_Settings_Screen()
{
    delete ui;
}

void HNS_Date_Time_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Date_Time_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_DATE_TIME_SETTINGS;
}

void HNS_Date_Time_Settings_Screen::my_slot_AMPM_Toggle()
{
    ui->PB_DateSet_Time->fToggleAMPM();
}

void HNS_Date_Time_Settings_Screen::on_PB_DateSet_Save_clicked()
{
    emit SIG_DateTime_Change_Request(ui->PB_DateSet_Date->fGetDate(),ui->PB_DateSet_Time->fGetTime(),ui->PB_DateSet_TimeZone->text());

    deleteLater();
}


void HNS_Date_Time_Settings_Screen::on_PB_DateSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Date_Time_Settings_Screen::on_PB_DateSetCancel_clicked()
{
    deleteLater();
}

bool HNS_Date_Time_Settings_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKP();
        fHideCalendar();
        return true;
    }

    return false;
}

void HNS_Date_Time_Settings_Screen::fCalendar_Finished(const QDate &date)
{
    if(date.isValid())
    {
        ui->PB_DateSet_Date->fSetDate(date);
    }
}

void HNS_Date_Time_Settings_Screen::fVKP_Finished(const bool &enter_pressed)
{
    if(fGetKPFocus() == ui->PB_DateSet_Time)
    {
        ui->PB_DateSet_Time->fSetEditMode(false,enter_pressed);
    }
}


void HNS_Date_Time_Settings_Screen::on_PB_DateSet_Date_clicked()
{
    fHideKP();
    fShowCalendar(ui->PB_DateSet_Date->fGetDate(),560,250);
}


void HNS_Date_Time_Settings_Screen::on_PB_DateSet_Time_clicked()
{
    fHideCalendar();
    fShowKP(ui->PB_DateSet_Time,860,400,true);
    ui->PB_DateSet_Time->fSetEditMode(true);
}


void HNS_Date_Time_Settings_Screen::on_PB_DateSet_TimeZone_clicked()
{
    fHideKP();
    fHideCalendar();

    if(ui->PB_DateSet_TimeZone->text() == "HST")
    {
        ui->PB_DateSet_TimeZone->setText("AST");
    }
    else if(ui->PB_DateSet_TimeZone->text() == "AST")
    {
        ui->PB_DateSet_TimeZone->setText("PST");
    }
    else if(ui->PB_DateSet_TimeZone->text() == "PST")
    {
        ui->PB_DateSet_TimeZone->setText("MST");
    }
    else if(ui->PB_DateSet_TimeZone->text() == "MST")
    {
        ui->PB_DateSet_TimeZone->setText("CST");
    }
    else if(ui->PB_DateSet_TimeZone->text() == "CST")
    {
        ui->PB_DateSet_TimeZone->setText("EST");
    }
    else if(ui->PB_DateSet_TimeZone->text() == "EST")
    {
        ui->PB_DateSet_TimeZone->setText("HST");
    }
    else
    {
        ui->PB_DateSet_TimeZone->setText("CST");
    }
}

