#include "hns_schedule_info_widget.h"
#include "ui_hns_schedule_info_widget.h"

#include <QDateTime>
#include <hns_qt_toolkit.h>

HNS_Schedule_Info_Widget::HNS_Schedule_Info_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_Schedule_Info_Widget)
{
    ui->setupUi(this);
}

HNS_Schedule_Info_Widget::~HNS_Schedule_Info_Widget()
{
    delete ui;
}

void HNS_Schedule_Info_Widget::fSetSchedule(const HNS_Schedule &schedule)
{
    QString tempstring;

    QDateTime startDate(ConvertSTLDatetoQDate(schedule.fGetStartDate()), ConvertSTLTimetoQTime(schedule.fGetStartTime()));
    QDateTime endDate(ConvertSTLDatetoQDate(schedule.fGetEndDate()), ConvertSTLTimetoQTime(schedule.fGetStopTime()));

    tempstring = QString("MM/dd/yyyy ") + QString("h:mm AP");
    if (endDate > QDateTime(QDate(2300,12,30), QTime(00,00,00)))
        ui->LBL_End->setText("Never End");
    else
        ui->LBL_End->setText(endDate.toString(tempstring));

    ui->LBL_Title->setText(QString::fromStdString(schedule.fGetTitle()));
    ui->LBL_Start->setText(startDate.toString(tempstring));
}

void HNS_Schedule_Info_Widget::fClear()
{
    ui->LBL_Title->setText("None");
    ui->LBL_Start->setText("None");
    ui->LBL_End->setText("None");
}
