#include "hns_schedule_list_item.h"
#include "ui_hns_schedule_list_item.h"

#include <QDateTime>
#include <hns_qt_toolkit.h>

//HNS_Schedule_List_Item::HNS_Schedule_List_Item(const int &index, const QString &message, const QDate &startDate, const QDate &endDate, const QString &day, QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::HNS_Schedule_List_Item),
//    f_index(index),
//    f_message(message),
//    f_start_date(startDate),
//    f_end_date(endDate),
//    f_day(day)
//{
//    ui->setupUi(this);
//    ui->LBL_Sched_Details->setText(QString("%1 %2 %3 %4").arg(f_message).arg(f_start_date.toString()).arg(f_end_date.toString()).arg(day));
//}

HNS_Schedule_List_Item::HNS_Schedule_List_Item(const int &index, const HNS_Schedule &schedule, QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::HNS_Schedule_List_Item)
{
    QString day = "  <font color=\"#1A3760\">";
    schedule.fGetDay(HNS_SCHED_SUNDAY)? day.append("SU "): day.append("");
    schedule.fGetDay(HNS_SCHED_MONDAY)?day.append("M "): day.append("");
    schedule.fGetDay(HNS_SCHED_TUESDAY)?day.append("T "): day.append("");
    schedule.fGetDay(HNS_SCHED_WEDNESDAY)?day.append("W "): day.append("");
    schedule.fGetDay(HNS_SCHED_THURSDAY)?day.append("T "): day.append("");
    schedule.fGetDay(HNS_SCHED_FRIDAY)?day.append("F "): day.append("");
    schedule.fGetDay(HNS_SCHED_SATURDAY)?day.append("S "): day.append("");
    day.append("</font>");

    f_message = QString::fromStdString(schedule.fGetTitle());
    f_day = day;

    f_start_date = ConvertSTLDatetoQDate(schedule.fGetStartDate());
    f_end_date = ConvertSTLDatetoQDate(schedule.fGetEndDate());
    QString neverEndOrEndDate;
    if (f_end_date > QDate(2300,12,30))
        neverEndOrEndDate = "Never End";
    else
        neverEndOrEndDate = f_end_date.toString();

    f_start_time = ConvertSTLTimetoQTime(schedule.fGetStartTime());
    f_stop_time = ConvertSTLTimetoQTime(schedule.fGetStopTime());

    f_index = index;

    ui->setupUi(this);

    QString tempstring = QString("%1 %2 %3 %4<br>%5 %6").arg(f_message).arg(f_start_date.toString()).arg(neverEndOrEndDate).arg(f_day).arg(f_start_time.toString("h:mm AP")).arg(f_stop_time.toString("h:mm AP"));
    ui->LBL_Sched_Details->setText(tempstring);
}

HNS_Schedule_List_Item::~HNS_Schedule_List_Item()
{
    delete ui;
}

void HNS_Schedule_List_Item::fSetChangeable(const bool &changeable)
{
    if(changeable)
    {
        ui->PB_Sched_Del->show();
        ui->PB_Sched_Edt->show();
    }
    else
    {
        ui->PB_Sched_Del->hide();
        ui->PB_Sched_Edt->hide();
    }
}

void HNS_Schedule_List_Item::on_PB_Sched_Edt_clicked()
{
    emit editClicked(f_index);
}

void HNS_Schedule_List_Item::on_PB_Sched_Del_clicked()
{
    emit deleteClicked(f_index);
}


