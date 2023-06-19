#ifndef HNS_ADMIN_SCREEN_H
#define HNS_ADMIN_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Admin_Screen;
}

class HNS_Admin_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Admin_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Admin_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_Admin_Cancel_clicked();

    void on_PB_Admin_Back_clicked();

    void on_PB_Admin_Debug_clicked();

    void on_PB_Admin_Trailer_clicked();

    void on_PB_Admin_Lock_clicked();

    void on_PB_Admin_Pass_clicked();

    void on_PB_Admin_Update_clicked();

    void on_PB_Admin_Reset_clicked();

    void my_slot_Reset_Confirmation(const bool &confirm);

private:
    Ui::HNS_Admin_Screen *ui;
};

#endif // HNS_ADMIN_SCREEN_H
