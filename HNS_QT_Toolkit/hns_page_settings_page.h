#ifndef HNS_PAGE_SETTINGS_PAGE_H
#define HNS_PAGE_SETTINGS_PAGE_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Page_Settings_Page;
}

class HNS_Page_Settings_Page : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Page_Settings_Page(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Page_Settings_Page();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_PageSet_Save_clicked();

    void on_PB_PageSet_Cancel_clicked();

    void on_PB_PageSet_Back_clicked();

    void on_PB_PageSet_PickFont_clicked();

    void on_PB_PageSet_LeftJustification_clicked();

    void on_PB_PageSet_CenterLineJustification_clicked();

    void on_PB_PageSet_RightJustification_clicked();

    void on_PB_PageSet_TopJustification_clicked();

    void on_PB_PageSet_CenterPageJustification_clicked();

    void on_PB_PageSet_BottomJustification_clicked();

    void my_slot_Font_Result(const int &font_select, const bool &apply_all);

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void fVKP_Finished(const bool &enter_pressed);

    Ui::HNS_Page_Settings_Page *ui;

    double f_current_pageoff_time, f_current_pageon_time;
    int f_current_font_selection;
};

#endif // HNS_PAGE_SETTINGS_PAGE_H
