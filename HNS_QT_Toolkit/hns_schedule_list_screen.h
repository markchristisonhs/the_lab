#ifndef HNS_SCHEDULE_LIST_SCREEN_H
#define HNS_SCHEDULE_LIST_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Schedule_List_Screen;
}

class HNS_Schedule_List_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Schedule_List_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Schedule_List_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_ShedList_NewSchedule_clicked();

    void on_PB_SchedList_Back_clicked();

    void my_slot_Del_Schedule_List_Item(int index);

    void my_slot_Edit_Schedule_List_Item(int index);

private:
    Ui::HNS_Schedule_List_Screen *ui;
};

#endif // HNS_SCHEDULE_LIST_SCREEN_H
