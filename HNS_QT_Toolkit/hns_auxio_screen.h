#ifndef HNS_AUXIO_SCREEN_H
#define HNS_AUXIO_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_AuxIO_Screen;
}

class HNS_AuxIO_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_AuxIO_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_AuxIO_Screen();

    type_widget fGetType() const {return HNS_WIDGET_AUXIO;}
    void fSetAccess(const type_user_level &) {}

    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_PB_Set_Msg1_clicked();

    void on_PB_Enable1_clicked();

    void on_PB_Set_Msg2_clicked();

    void on_PB_Enable2_clicked();

    void on_PB_Set_Msg3_clicked();

    void on_PB_Enable3_clicked();

    void on_PB_Set_Msg4_clicked();

    void on_PB_Enable4_clicked();

    void on_PB_Save_clicked();

    void on_PB_Back_clicked();

private:
    Ui::HNS_AuxIO_Screen *ui;

    std::vector<std::string> f_msgs_multi;
    std::vector<HNS_NTCIP_MessageIDCode> f_msgs;
    std::vector<unsigned char> f_enables;
    std::vector<int> f_durations;

    void LE_Duration1_clicked();
    void LE_Duration2_clicked();
    void LE_Duration3_clicked();
    void LE_Duration4_clicked();

    void LE_Duration1_editingFinished(const bool &enter_pressed);
    void LE_Duration2_editingFinished(const bool &enter_pressed);
    void LE_Duration3_editingFinished(const bool &enter_pressed);
    void LE_Duration4_editingFinished(const bool &enter_pressed);

    void fVKP_Finished(const bool &);

    int f_index;
};

#endif // HNS_AUXIO_SCREEN_H
