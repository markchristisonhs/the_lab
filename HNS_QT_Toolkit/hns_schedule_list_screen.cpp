#include "hns_schedule_list_screen.h"
#include "ui_hns_schedule_list_screen.h"

#include "hns_schedule_edit_screen.h"

#include "hns_schedule_list_item.h"
#include "hns_qt_toolkit.h"
#include <QHBoxLayout>

HNS_Schedule_List_Screen::HNS_Schedule_List_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Schedule_List_Screen)
{
    ui->setupUi(this);

    fUpdate();
}

HNS_Schedule_List_Screen::~HNS_Schedule_List_Screen()
{
    delete ui;
}

type_widget HNS_Schedule_List_Screen::fGetType() const
{
    return HNS_WIDGET_SCHEDULE_LIST;
}

void HNS_Schedule_List_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

void HNS_Schedule_List_Screen::fUpdate(const int &update_flags)
{
//    QString messageTemp;
    HNS_Schedule schedule;

    ui->LW_Schedule_List->clear();
    ui->LW_Schedule_List->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->LW_Schedule_List->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->LW_Schedule_List->setStyleSheet("QListView::item:selected{background-color: #1A3760;}");

    for (size_t i=0; i<f_machine->fGetSchedules().size(); i++)
    {
        schedule = f_machine->fGetSchedules().at(i);
//        messageTemp =  QString::fromStdString(schedule.fGetTitle());

//        QString day = "  <font color=\"#1A3760\">";
//        schedule.fGetDay(HNS_SCHED_SUNDAY)? day.append("SU "): day.append("");
//        schedule.fGetDay(HNS_SCHED_MONDAY)?day.append("M "): day.append("");
//        schedule.fGetDay(HNS_SCHED_TUESDAY)?day.append("T "): day.append("");
//        schedule.fGetDay(HNS_SCHED_WEDNESDAY)?day.append("W "): day.append("");
//        schedule.fGetDay(HNS_SCHED_THURSDAY)?day.append("T "): day.append("");
//        schedule.fGetDay(HNS_SCHED_FRIDAY)?day.append("F "): day.append("");
//        schedule.fGetDay(HNS_SCHED_SATURDAY)?day.append("S "): day.append("");
//        day.append("</font>");

//        HNS_Schedule_List_Item *scheduleListItem = new HNS_Schedule_List_Item(i, messageTemp,
//                                                                              ConvertSTLDatetoQDate(schedule.fGetStartDate()),
//                                                                              ConvertSTLDatetoQDate(schedule.fGetEndDate()),
//                                                                              day);
        HNS_Schedule_List_Item *scheduleListItem = new HNS_Schedule_List_Item(i,schedule,this);

        scheduleListItem->setFixedSize(845, 70);
        scheduleListItem->fSetChangeable(f_machine->fGetAccess() < HNS_VIEWER);
        QListWidgetItem *item = new QListWidgetItem(ui->LW_Schedule_List);
        connect(scheduleListItem,SIGNAL(deleteClicked(int)),this,SLOT(my_slot_Del_Schedule_List_Item(int)));
        connect(scheduleListItem,SIGNAL(editClicked(int)),this,SLOT(my_slot_Edit_Schedule_List_Item(int)));

        QWidget *widget = new QWidget;
        widget->setStyleSheet("background-color:#FFFFFF;");
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(scheduleListItem);
        layout->addStretch();
        layout->setSizeConstraint(layout->SetFixedSize);
        widget->setLayout(layout);
        item->setSizeHint(widget->sizeHint());
        ui->LW_Schedule_List->addItem(item);
        ui->LW_Schedule_List->setItemWidget(item, widget);
    }

    HNS_UI_Widget::fUpdate(update_flags);
}

void HNS_Schedule_List_Screen::on_PB_ShedList_NewSchedule_clicked()
{
    fShowScreen(new HNS_Schedule_Edit_Screen(this, f_machine,HNS_SCHED_CREATE));
}


void HNS_Schedule_List_Screen::on_PB_SchedList_Back_clicked()
{
    emit SIG_GO_HOME();
}

void HNS_Schedule_List_Screen::my_slot_Del_Schedule_List_Item(int index)
{
    emit SIG_Delete_Schedule_Request(index);
}

void HNS_Schedule_List_Screen::my_slot_Edit_Schedule_List_Item(int index)
{
    HNS_Schedule schedule;
    if(index >= 0 && static_cast<size_t>(index) < f_machine->fGetSchedules().size())
    {
        schedule = f_machine->fGetSchedules().at(index);
        HNS_Schedule_Edit_Screen *temp_widget = new HNS_Schedule_Edit_Screen(this,f_machine,HNS_SCHED_EDIT,&schedule,index);
        temp_widget->fSetAccess(HNS_VIEWER);
        fShowScreen(temp_widget);
    }
}

