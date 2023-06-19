#ifndef HNS_FONT_SETTINGS_SCREEN_H
#define HNS_FONT_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

typedef enum
{
    HNS_FONT_EDIT = 0,
    HNS_FONT_SETTINGS
} type_Font_Mode;

namespace Ui {
class HNS_Font_Settings_Screen;
}

class HNS_Font_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Font_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_Font_Mode &font_mode);
    ~HNS_Font_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fSetFontSelection(const int &select);

signals:
    void SIG_Font_Result(const int &font_select, const bool &apply_all);

private slots:
    void on_PB_FontSet_Save_clicked();

    void on_PB_FontSet_Cancel_clicked();

    void on_PB_FontSet_Back_clicked();

    void my_slot_GV_Font_Preview1_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview2_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview3_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview4_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview5_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview6_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview7_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview8_clicked(QMouseEvent *event);
    void my_slot_GV_Font_Preview9_clicked(QMouseEvent *event);

    void on_PB_FontSet_ApplyAll_clicked();

    void on_PB_FontSet_ApplyAfter_clicked();

private:
    Ui::HNS_Font_Settings_Screen *ui;

    void fUpdateFonts();
    int fGetFontSelection();
};

#endif // HNS_FONT_SETTINGS_SCREEN_H
