#ifndef HNS_BRIGHTNESS_SETTINGS_SCREEN_H
#define HNS_BRIGHTNESS_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Brightness_Settings_Screen;
}

class HNS_Brightness_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Brightness_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Brightness_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_BrightSet_Save_clicked();

    void on_PB_BrightSet_Cancel_clicked();

    void on_PB_BrightSet_Back_clicked();

    void on_PB_BrightSet_Auto_clicked();

    void on_PB_BrightSet_Manual_clicked();

    void on_PB_BrightSet_Lv1_clicked();

    void on_PB_BrightSet_Lv2_clicked();

    void on_PB_BrightSet_Lv3_clicked();

    void on_PB_BrightSet_Lv4_clicked();

    void on_PB_BrightSet_Lv5_clicked();

    void on_PB_BrightSet_Lv6_clicked();

    void on_PB_BrightSet_Lv7_clicked();

    void on_PB_BrightSet_Lv8_clicked();

    void on_PB_BrightSet_Lv9_clicked();

    void on_PB_BrightSet_Lv10_clicked();

    void on_PB_BrightSet_Lv11_clicked();

    void on_PB_BrightSet_Lv12_clicked();

    void on_PB_BrightSet_Lv13_clicked();

    void on_PB_BrightSet_Lv14_clicked();

    void on_PB_BrightSet_Lv15_clicked();

private:
    void fSetBrightnessButtons(const int &brightness);
    int fGetBrightnessLevelFromButtons();

    Ui::HNS_Brightness_Settings_Screen *ui;
};

#endif // HNS_BRIGHTNESS_SETTINGS_SCREEN_H
