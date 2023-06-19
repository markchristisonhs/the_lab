#ifndef HNS_TM_CUSTOM_MSG_H
#define HNS_TM_CUSTOM_MSG_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_TM_Custom_Msg;
}

class HNS_TM_Custom_Msg : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_TM_Custom_Msg(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_TM_Custom_Msg();

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

    type_widget fGetType() const {return HNS_WIDGET_TM_CUSTOM_MSG;}
    void fSetAccess(const type_user_level &) {}

    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

private slots:
    void on_PB_Change_Custom1_clicked();

    void on_PB_Change_Custom2_clicked();

    void on_PB_Change_Custom3_clicked();

    void on_PB_Change_Custom4_clicked();

    void on_PB_Back_clicked();

private:
    Ui::HNS_TM_Custom_Msg *ui;

    int f_index;
};

#endif // HNS_TM_CUSTOM_MSG_H
