#ifndef HNS_SCHEDULE_LIST_ITEM_H
#define HNS_SCHEDULE_LIST_ITEM_H

#include <QWidget>
#include <QDate>
#include <QTime>
#include <HNS_schedule.h>

namespace Ui {
class HNS_Schedule_List_Item;
}

class HNS_Schedule_List_Item : public QWidget
{
    Q_OBJECT

public:
//    explicit HNS_Schedule_List_Item(const int &index, const QString &message, const QDate &startDate, const QDate &endDate, const QString &day, QWidget *parent = nullptr);
    explicit HNS_Schedule_List_Item(const int &index, const HNS_Schedule &schedule, QWidget *parent = nullptr);
    ~HNS_Schedule_List_Item();

    void fSetChangeable(const bool &changeable);

private slots:
    void on_PB_Sched_Edt_clicked();
    void on_PB_Sched_Del_clicked();

signals:
    void editClicked(int i);
    void deleteClicked(int i);

private:
    Ui::HNS_Schedule_List_Item *ui;
    int f_index;
    QString f_message;
    QDate f_start_date;
    QDate f_end_date;
    QTime f_start_time,f_stop_time;
    QString f_day;
};

#endif // HNS_SCHEDULE_LIST_ITEM_H
