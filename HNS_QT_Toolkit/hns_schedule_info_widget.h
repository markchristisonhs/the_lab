#ifndef HNS_SCHEDULE_INFO_WIDGET_H
#define HNS_SCHEDULE_INFO_WIDGET_H

#include <QWidget>
#include <HNS_schedule.h>

namespace Ui {
class HNS_Schedule_Info_Widget;
}

class HNS_Schedule_Info_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Schedule_Info_Widget(QWidget *parent = nullptr);
    ~HNS_Schedule_Info_Widget();

    void fSetSchedule(const HNS_Schedule &schedule);
    void fClear();

private:
    Ui::HNS_Schedule_Info_Widget *ui;
};

#endif // HNS_SCHEDULE_INFO_WIDGET_H
