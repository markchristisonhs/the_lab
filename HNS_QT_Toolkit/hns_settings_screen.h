#ifndef HNS_SETTINGS_SCREEN_H
#define HNS_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <QTimer>

namespace Ui {
class HNS_Settings_Screen;
}

class HNS_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Settings_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_Settings_DateTime_clicked();

    void on_PB_Settings_Brightness_clicked();

    void on_PB_Settings_Radar_clicked();

    void on_PB_Settings_PageDefaults_clicked();

    void on_PB_Settings_Comm_clicked();

    void on_PB_Admin_clicked();

    void on_PB_Diagnostics_clicked();

    void on_PB_Settings_Back_clicked();

    void fTickTock();

    void on_PB_Settings_AuxIO_clicked();

private:
    Ui::HNS_Settings_Screen *ui;

    QTimer *f_timer;
};

#endif // HNS_SETTINGS_SCREEN_H
