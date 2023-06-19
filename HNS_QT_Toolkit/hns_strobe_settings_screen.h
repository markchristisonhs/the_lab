#ifndef HNS_STROBE_SETTINGS_SCREEN_H
#define HNS_STROBE_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Strobe_Settings_Screen;
}

enum
{
    HNS_STROBE_SETTINGS_NORAML,
    HNS_STROBE_SETTINGS_SCHEDULE
};

class HNS_Strobe_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Strobe_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, const HNS_Radar_Strobe *strobe_settings = nullptr, const int &mode = HNS_STROBE_SETTINGS_NORAML);
    ~HNS_Strobe_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;
signals:
    void SIG_Strobe_Result();

private slots:
    void on_PB_StrobeSet_Save_clicked();

    void on_PB_StrobeSet_Cancel_clicked();

    void on_PB_StrobeSet_Test_clicked();

    void on_PB_StrobeSet_Speed_Up_clicked();

    void on_PB_StrobeSet_Speed_Dn_clicked();

    void on_PB_StrobeSet_Duration_Up_clicked();

    void on_PB_StrobeSet_Duration_Dn_clicked();

    void on_PB_StrobeSet_Pattern_Up_clicked();

    void on_PB_StrobeSet_Pattern_Dn_clicked();

    void on_PB_StrobeSet_Enable_clicked(bool checked);

private:
    Ui::HNS_Strobe_Settings_Screen *ui;
};

#endif // HNS_STROBE_SETTINGS_SCREEN_H
