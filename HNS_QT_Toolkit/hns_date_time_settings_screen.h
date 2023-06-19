#ifndef HNS_DATE_TIME_SETTINGS_SCREEN_H
#define HNS_DATE_TIME_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Date_Time_Settings_Screen;
}

class HNS_Date_Time_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Date_Time_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Date_Time_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;
public slots:
    void my_slot_AMPM_Toggle();

private slots:
    void on_PB_DateSet_Save_clicked();

    void on_PB_DateSet_Cancel_clicked();

    void on_PB_DateSetCancel_clicked();

    void on_PB_DateSet_Date_clicked();

    void on_PB_DateSet_Time_clicked();

    void on_PB_DateSet_TimeZone_clicked();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void fCalendar_Finished(const QDate &date);
    void fVKP_Finished(const bool &enter_pressed);

    Ui::HNS_Date_Time_Settings_Screen *ui;
};

#endif // HNS_DATE_TIME_SETTINGS_SCREEN_H
