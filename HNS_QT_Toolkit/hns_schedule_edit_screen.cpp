#include "hns_schedule_edit_screen.h"
#include "ui_hns_schedule_edit_screen.h"

#include "hns_message_edit_screen.h"

#include <hns_qt_toolkit.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "hns_radar_settings_screen.h"
#include "hns_strobe_settings_screen.h"
#include <QTimer>

HNS_Schedule_Edit_Screen::HNS_Schedule_Edit_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_sched_mode &sched_mode, HNS_Schedule *schedule, const int &index) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Schedule_Edit_Screen)
  , f_sched_mode(sched_mode)
  , f_calendar_object(nullptr)
  , f_index(index)
  , f_override_radar(false)
  , f_override_strobe(false)
{
    ui->setupUi(this);

    f_current_access = f_machine->fGetAccess();

    ui->LBL_Sched_Error->hide();

    ui->PB_Sched_StartTime->fSetDefaultLabel("SELECT\nTIME");
    ui->PB_Sched_EndTime->fSetDefaultLabel("SELECT\nTIME");

    ui->PB_Sched_SelectStartDate->fSetDefaultLabel("SELECT\nDATE");
    ui->PB_Sched_EndDate->fSetDefaultLabel("SELECT\nDATE");

    f_temp_radar_settings.fSetUnits(f_machine->fGetRadarUnits());

    installEventFilter(this);

    HNS_Schedule init_schedule;
    if(schedule != nullptr)
    {
        init_schedule = *schedule;
    }
    fInit(init_schedule);
}

HNS_Schedule_Edit_Screen::~HNS_Schedule_Edit_Screen()
{
    delete ui;
}

void HNS_Schedule_Edit_Screen::fSetAccess(const type_user_level &access)
{
    f_current_access = access;

    fUpdate();
}

type_widget HNS_Schedule_Edit_Screen::fGetType() const
{
    return HNS_WIDGET_SCHEDULE_EDIT;
}

void HNS_Schedule_Edit_Screen::fUpdate(const int &update_flags)
{
    bool availableday[]={true, true, true,true, true, true,true};
    int startDay = ui->PB_Sched_SelectStartDate->fGetDate().dayOfWeek();
    int noOfDays = ui->PB_Sched_SelectStartDate->fGetDate().daysTo(ui->PB_Sched_EndDate->fGetDate()) + 1;

    if(!ui->PB_Sched_NeverEnd->isChecked())
    {
        if(ui->PB_Sched_StartNow->isChecked())
        {
            startDay = QDateTime::currentDateTime().date().dayOfWeek();
        }
        noOfDays = noOfDays>=7 ? 7: noOfDays;
        for(int i = 0 ; i < noOfDays; i++)
        {
            if(startDay - 1 >= 0 && startDay - 1 < 7)
            {
                availableday[startDay-1] = false;
                startDay++;
                startDay =  startDay >7? 1: startDay;
            }
        }
    }
    else
    {
        availableday[0] = false;
        availableday[1] = false;
        availableday[2] = false;
        availableday[3] = false;
        availableday[4] = false;
        availableday[5] = false;
        availableday[6] = false;
    }

    ui->PB_Sched_Monday->setDisabled(availableday[0]);
    ui->PB_Sched_Tuesday->setDisabled(availableday[1]);
    ui->PB_Sched_Wednesday->setDisabled(availableday[2]);
    ui->PB_Sched_Thursday->setDisabled(availableday[3]);
    ui->PB_Sched_Friday->setDisabled(availableday[4]);
    ui->PB_Sched_Saturday->setDisabled(availableday[5]);
    ui->PB_Sched_Sunday->setDisabled(availableday[6]);

    ui->PB_Sched_Monday->setChecked(ui->PB_Sched_Monday->isChecked() && !availableday[0]);
    ui->PB_Sched_Tuesday->setChecked(ui->PB_Sched_Tuesday->isChecked() && !availableday[1]);
    ui->PB_Sched_Wednesday->setChecked(ui->PB_Sched_Wednesday->isChecked() && !availableday[2]);
    ui->PB_Sched_Thursday->setChecked(ui->PB_Sched_Thursday->isChecked() && !availableday[3]);
    ui->PB_Sched_Friday->setChecked(ui->PB_Sched_Friday->isChecked() && !availableday[4]);
    ui->PB_Sched_Saturday->setChecked(ui->PB_Sched_Saturday->isChecked() && !availableday[5]);
    ui->PB_Sched_Sunday->setChecked(ui->PB_Sched_Sunday->isChecked() && !availableday[6]);

    ui->PB_Sched_StartNow->setChecked(!ui->PB_Sched_SelectStartDate->fGetDate().isValid());
    ui->PB_Sched_NeverEnd->setChecked(!ui->PB_Sched_EndDate->fGetDate().isValid());

    ui->PB_Sched_SelectStartDate->setChecked(ui->PB_Sched_SelectStartDate->fGetDate().isValid() || ui->PB_Sched_SelectStartDate->fGetEditMode());
    ui->PB_Sched_EndDate->setChecked(ui->PB_Sched_EndDate->fGetDate().isValid() || ui->PB_Sched_EndDate->fGetEditMode());

    ui->PB_Sched_StartTime->setChecked(ui->PB_Sched_StartTime->fGetTime().isValid() || ui->PB_Sched_StartTime->fGetEditMode());
    ui->PB_Sched_EndTime->setChecked(ui->PB_Sched_EndTime->fGetTime().isValid() || ui->PB_Sched_EndTime->fGetEditMode());

    ui->LE_Title->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_EditMessage->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_StartNow->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_NeverEnd->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_SelectStartDate->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_StartTime->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_EndDate->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_EndTime->setDisabled(f_current_access == HNS_VIEWER);

    ui->PB_Sched_Sunday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Monday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Tuesday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Wednesday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Thursday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Friday->setDisabled(f_current_access == HNS_VIEWER);
    ui->PB_Sched_Saturday->setDisabled(f_current_access == HNS_VIEWER);

    if(f_current_access == HNS_VIEWER)
    {

        ui->PB_Sched_NewSched->hide();
    }
    else
    {
        ui->PB_Sched_NewSched->show();
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_Schedule_Edit_Screen::my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &, const type_ui_message_mode &)
{
    ui->GV_Sched_Preview->fSetMessage(multi);
}

void HNS_Schedule_Edit_Screen::my_slot_AMPM_Toggle()
{
    if(fGetKPFocus() == ui->PB_Sched_StartTime)
    {
        ui->PB_Sched_StartTime->fToggleAMPM();
    }
    else if(fGetKPFocus() == ui->PB_Sched_EndTime)
    {
        ui->PB_Sched_EndTime->fToggleAMPM();
    }
}

void HNS_Schedule_Edit_Screen::my_slot_Strobe_Change_Request(const HNS_Radar_Strobe &strobesettings)
{
    f_override_strobe = true;
    f_temp_strobe_settings = strobesettings;

    ui->PGE_Radar->fSetScheduledRadarInfo(f_override_radar,f_override_strobe,f_temp_radar_settings,f_temp_strobe_settings);
}

void HNS_Schedule_Edit_Screen::my_slot_Radar_Change_Request(const type_radar_mode &mode, const type_radar_units &,
                                                            const int &legacy_speed, const HNS_Radar_Police_Speed &police_speed,
                                                            const QString &legacy_msg, const QString &under_min_msg, const QString &under_vio_msg,
                                                            const QString &over_vio_msg, const QString &over_max_msg)
{
    f_override_radar = true;
    f_temp_radar_settings.fSetMode(mode);
    f_temp_radar_settings.fSetLegacySpeed(legacy_speed);
    f_temp_radar_settings.fSetPoliceSpeed(police_speed);
    f_temp_radar_settings.fSetLegacyMessage(legacy_msg.toStdString());
    f_temp_radar_settings.fSetUnderMinMessage(under_min_msg.toStdString());
    f_temp_radar_settings.fSetUnderVioMessage(under_vio_msg.toStdString());
    f_temp_radar_settings.fSetOverVioMessage(over_vio_msg.toStdString());
    f_temp_radar_settings.fSetOverMaxMessage(over_max_msg.toStdString());

    ui->PGE_Radar->fSetScheduledRadarInfo(f_override_radar,f_override_strobe,f_temp_radar_settings,f_temp_strobe_settings);
}

void HNS_Schedule_Edit_Screen::on_PB_Sched_Back_clicked()
{
    deleteLater();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_NewSched_clicked()
{
    HNS_Schedule result;

    result.fSetTitle(ui->LE_Title->text().toStdString());

    result.fSetMessage(ui->GV_Sched_Preview->fGetCurrentMessage());

    result.fSetStartNow(ui->PB_Sched_StartNow->isChecked());
    result.fSetNeverEnd(ui->PB_Sched_NeverEnd->isChecked());

    if(!ui->PB_Sched_StartNow->isChecked())
    {
        result.fSetStartDate(ConvertQDatetoSTLDate(ui->PB_Sched_SelectStartDate->fGetDate()));
    }
    else
    {
        result.fSetStartDate(ConvertQDatetoSTLDate(QDateTime::currentDateTime().date()));
    }

    if(!ui->PB_Sched_NeverEnd->isChecked())
    {
        result.fSetEndDate(ConvertQDatetoSTLDate(ui->PB_Sched_EndDate->fGetDate()));
    }
    else
    {
        result.fSetEndDate(ConvertQDatetoSTLDate(QDate(2367,1,5)));
    }

    result.fSetStartTime(ConvertQTimetoSTLTime(ui->PB_Sched_StartTime->fGetTime()));
    result.fSetStopTime(ConvertQTimetoSTLTime(ui->PB_Sched_EndTime->fGetTime()));

    ui->PB_Sched_Sunday->isChecked() ? result.fSetDay(HNS_SCHED_SUNDAY) : result.fClearDay(HNS_SCHED_SUNDAY);
    ui->PB_Sched_Monday->isChecked() ? result.fSetDay(HNS_SCHED_MONDAY) : result.fClearDay(HNS_SCHED_MONDAY);
    ui->PB_Sched_Tuesday->isChecked() ? result.fSetDay(HNS_SCHED_TUESDAY) : result.fClearDay(HNS_SCHED_TUESDAY);
    ui->PB_Sched_Wednesday->isChecked() ? result.fSetDay(HNS_SCHED_WEDNESDAY) : result.fClearDay(HNS_SCHED_WEDNESDAY);
    ui->PB_Sched_Thursday->isChecked() ? result.fSetDay(HNS_SCHED_THURSDAY) : result.fClearDay(HNS_SCHED_THURSDAY);
    ui->PB_Sched_Friday->isChecked() ? result.fSetDay(HNS_SCHED_FRIDAY) : result.fClearDay(HNS_SCHED_FRIDAY);
    ui->PB_Sched_Saturday->isChecked() ? result.fSetDay(HNS_SCHED_SATURDAY) : result.fClearDay(HNS_SCHED_SATURDAY);

    result.fSetOverrideRadar(f_override_radar);
    result.fSetOverrideStrobe(f_override_strobe);
    result.fSetRadarSettings(f_temp_radar_settings);
    result.fSetStrobeSettings(f_temp_strobe_settings);

//    type_sched_error error;
//    if(HNS_Schedule::fsIsScheduleValid(result,ConvertQDateTimetoSTLDateTime(QDateTime::currentDateTime()),error))
//    {
//        if(f_sched_mode == HNS_SCHED_CREATE)
//        {
//            emit SIG_Schedule(result,-1,true);
//        }
//        else
//        {
//            emit SIG_Schedule(result,f_index,false);
//        }

//        deleteLater();
//    }
//    else
//    {
//        ui->LBL_Sched_Error->show();
//        QTimer::singleShot(3000,this,&HNS_Schedule_Edit_Screen::my_slot_Clear_Invalid);
//    }
    result.fSetActive();
    if(f_sched_mode == HNS_SCHED_CREATE)
    {
        emit SIG_Schedule(result,-1,true);
    }
    else
    {
        emit SIG_Schedule(result,f_index,false);
    }

    deleteLater();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_Cancel_clicked()
{
    deleteLater();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_EditMessage_clicked()
{
    fShowScreen(new HNS_Message_Edit_Screen(this, f_machine, HNS_EDIT_PICKER));
}

void HNS_Schedule_Edit_Screen::fInit(HNS_Schedule &schedule)
{
    HNS_Message2 temp_message;
    QDateTime startDate;
    QDateTime endDate;

    f_override_radar = schedule.fGetOverrideRadar();
    f_override_strobe = schedule.fGetOverrideStrobe();
    f_temp_radar_settings = schedule.fGetRadarSettings();
    f_temp_strobe_settings = schedule.fGetStrobeSettings();

    ui->GV_Sched_Preview->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Sched_Preview->fSetGraphics(*f_machine->fGetGraphics());
    ui->LE_Title->installEventFilter(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    QImage icon = QImage(":/icons/dn_arrow.png");
    icon = icon.scaled(40,49);
    scene->addPixmap(QPixmap::fromImage(icon))->setOffset(0,0);

    ui->GV_DownArrow1->setScene(scene);
    ui->GV_DownArrow1->setSceneRect(0,0,40,49);
    ui->GV_DownArrow1->setObjectName("Glyph");

    ui->GV_DownArrow2->setScene(scene);
    ui->GV_DownArrow2->setSceneRect(0,0,40,49);
    ui->GV_DownArrow2->setObjectName("Glyph");

    scene->update();

    startDate = QDateTime(ConvertSTLDatetoQDate(schedule.fGetStartDate()), ConvertSTLTimetoQTime(schedule.fGetStartTime()));
    endDate = QDateTime(ConvertSTLDatetoQDate(schedule.fGetStartDate()), ConvertSTLTimetoQTime(schedule.fGetStopTime()));

   if(schedule.fStartNow())
    {
        ui->PB_Sched_StartNow->setChecked(true);
    }
    else
    {
        ui->PB_Sched_StartNow->setChecked(false);

        ui->PB_Sched_SelectStartDate->fSetDate(startDate.date());
        ui->PB_Sched_SelectStartDate->setChecked(startDate.date().isValid());
    }

    if(schedule.fNeverEnd())
    {
        ui->PB_Sched_NeverEnd->setChecked(true);
    }
    else
    {
        ui->PB_Sched_NeverEnd->setChecked(false);

        ui->PB_Sched_EndDate->fSetDate(endDate.date());
        ui->PB_Sched_EndDate->setChecked(endDate.date().isValid());
    }

    ui->PB_Sched_StartTime->fSetTime(startDate.time());
    ui->PB_Sched_StartTime->setChecked(startDate.time().isValid());
    ui->PB_Sched_EndTime->fSetTime(endDate.time());
    ui->PB_Sched_StartTime->setChecked(endDate.time().isValid());

    int startDay = ConvertSTLDatetoQDate( schedule.fGetStartDate()).dayOfWeek();
    int noOfDays= ConvertSTLDatetoQDate( schedule.fGetStartDate()).daysTo(ConvertSTLDatetoQDate( schedule.fGetEndDate())) + 1;
    bool availableday[]={true, true, true,true, true, true,true};

    if(!schedule.fNeverEnd())
    {
        if(schedule.fStartNow())
        {
            startDay = QDateTime::currentDateTime().date().dayOfWeek();
        }
        noOfDays = noOfDays>=7 ? 7: noOfDays;
        for(int i = 0 ; i < noOfDays; i++)
        {
            if(startDay - 1 >= 0 && startDay - 1 < 7)
            {
                availableday[startDay-1] = false;
                startDay++;
                startDay =  startDay >7? 1: startDay;
            }
        }
    }
    else
    {
        availableday[0] = false;
        availableday[1] = false;
        availableday[2] = false;
        availableday[3] = false;
        availableday[4] = false;
        availableday[5] = false;
        availableday[6] = false;
    }

    ui->PB_Sched_Monday->setDisabled(availableday[0]);
    ui->PB_Sched_Tuesday->setDisabled(availableday[1]);
    ui->PB_Sched_Wednesday->setDisabled(availableday[2]);
    ui->PB_Sched_Thursday->setDisabled(availableday[3]);
    ui->PB_Sched_Friday->setDisabled(availableday[4]);
    ui->PB_Sched_Saturday->setDisabled(availableday[5]);
    ui->PB_Sched_Sunday->setDisabled(availableday[6]);

    ui->PB_Sched_Monday->setChecked(schedule.fGetDay(HNS_SCHED_MONDAY)&& !availableday[0]);
    ui->PB_Sched_Tuesday->setChecked(schedule.fGetDay(HNS_SCHED_TUESDAY)&& !availableday[1]);
    ui->PB_Sched_Wednesday->setChecked(schedule.fGetDay(HNS_SCHED_WEDNESDAY)&& !availableday[2]);
    ui->PB_Sched_Thursday->setChecked(schedule.fGetDay(HNS_SCHED_THURSDAY)&& !availableday[3]);
    ui->PB_Sched_Friday->setChecked(schedule.fGetDay(HNS_SCHED_FRIDAY)&& !availableday[4]);
    ui->PB_Sched_Saturday->setChecked(schedule.fGetDay(HNS_SCHED_SATURDAY)&& !availableday[5]);
    ui->PB_Sched_Sunday->setChecked(schedule.fGetDay(HNS_SCHED_SUNDAY)&& !availableday[6]);


    schedule.fGetMessage(temp_message);
    ui->GV_Sched_Preview->fSetMessage(temp_message);

    if(f_sched_mode == HNS_SCHED_EDIT)
    {
        ui->LE_Title->setText(QString::fromStdString(schedule.fGetTitle()));
    }
    else if(f_sched_mode == HNS_SCHED_CREATE)
    {
        ui->LE_Title->setText("");
    }

    ui->PGE_Radar->fSetInfoMode(HNS_RADAR_INFO_SCHEDULE);
    ui->PGE_Radar->fSetScheduledRadarInfo(f_override_radar,f_override_strobe,f_temp_radar_settings,f_temp_strobe_settings);

    if(!f_machine->fGetRadarConnected())
    {
        ui->PGE_Radar->hide();
        ui->PB_Sched_ClearRadar->hide();
        ui->PB_Sched_EditRadar->hide();
        ui->PB_Sched_EditStrobes->hide();
        ui->PB_Sched_ClearStrobes->hide();
    }
}

bool HNS_Schedule_Edit_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && obj == ui->LE_Title)
    {
        fToggleKB(ui->LE_Title,true);
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKB();
        fHideKP();
        fHideCalendar();
        ui->LE_Title->clearFocus();
        return true;
    }
    else if (obj == ui->LE_Title && event->type() == QEvent::FocusOut)
    {
        fHideKB();
        ui->LE_Title->clearFocus();
        return true;
    }

    return false;
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_StartNow_clicked()
{
    fCloseKeyboards();

    ui->PB_Sched_StartNow->setChecked(true);

    ui->PB_Sched_StartTime->fClear();
    ui->PB_Sched_SelectStartDate->fClear();

    fUpdate();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_NeverEnd_clicked()
{
    fCloseKeyboards();

    ui->PB_Sched_EndTime->fClear();
    ui->PB_Sched_EndDate->fClear();

    fUpdate();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_SelectStartDate_clicked()
{
    fCloseKeyboards();

    fShowCalendar(ui->PB_Sched_SelectStartDate->fGetDate(),0,390);
    f_calendar_object = ui->PB_Sched_SelectStartDate;
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_StartTime_clicked()
{
    fCloseKeyboards();

    fShowKP(ui->PB_Sched_StartTime,185,390,true);
    ui->PB_Sched_StartTime->fSetEditMode(true);
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_EndDate_clicked()
{
    fCloseKeyboards();

    fShowCalendar(ui->PB_Sched_EndDate->fGetDate(),230,390);
    f_calendar_object = ui->PB_Sched_EndDate;
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_EndTime_clicked()
{
    fCloseKeyboards();

    fShowKP(ui->PB_Sched_EndTime,515,390,true);
    ui->PB_Sched_EndTime->fSetEditMode(true);
}

void HNS_Schedule_Edit_Screen::hideAllPopup(bool startDate, bool endDate, bool startTime, bool endTime)
{
    if(startDate || endDate)
    {
        fHideCalendar();
    }
    if(startTime || endTime)
    {
        fHideKP();
    }
}

void HNS_Schedule_Edit_Screen::fCalendar_Finished(const QDate &date)
{
    if(date.isValid())
    {
        if(f_calendar_object == ui->PB_Sched_SelectStartDate)
        {
            ui->PB_Sched_SelectStartDate->fSetDate(date);
        }
        else if(f_calendar_object == ui->PB_Sched_EndDate)
        {
            ui->PB_Sched_EndDate->fSetDate(date);
        }
        f_calendar_object = nullptr;
    }
    fUpdate();
}

void HNS_Schedule_Edit_Screen::fVKP_Finished(const bool &enter_pressed)
{
    if(fGetKPFocus() == ui->PB_Sched_StartTime)
    {
        ui->PB_Sched_StartTime->fSetEditMode(false,enter_pressed);
    }
    else if(fGetKPFocus() == ui->PB_Sched_EndTime)
    {
        ui->PB_Sched_EndTime->fSetEditMode(false,enter_pressed);
    }

    fUpdate();
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_EditRadar_clicked()
{
    HNS_Radar_Settings_Screen *temp_widget = new HNS_Radar_Settings_Screen(this,f_machine,&f_temp_radar_settings,HNS_RADAR_SETTINGS_SCHEDULE);
    fShowScreen(temp_widget);
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_ClearRadar_clicked()
{
    f_override_radar = false;

    ui->PGE_Radar->fSetScheduledRadarInfo(f_override_radar,f_override_strobe,f_temp_radar_settings,f_temp_strobe_settings);
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_EditStrobes_clicked()
{
    HNS_Strobe_Settings_Screen *temp_widget = new HNS_Strobe_Settings_Screen(this,f_machine,&f_temp_strobe_settings,HNS_STROBE_SETTINGS_SCHEDULE);
    fShowScreen(temp_widget);
}


void HNS_Schedule_Edit_Screen::on_PB_Sched_ClearStrobes_clicked()
{
    f_override_strobe = false;

    ui->PGE_Radar->fSetScheduledRadarInfo(f_override_radar,f_override_strobe,f_temp_radar_settings,f_temp_strobe_settings);
}

void HNS_Schedule_Edit_Screen::my_slot_Clear_Invalid()
{
    ui->LBL_Sched_Error->hide();
}

