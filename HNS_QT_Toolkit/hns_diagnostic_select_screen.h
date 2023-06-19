#ifndef HNS_DIAGNOSTIC_SELECT_SCREEN_H
#define HNS_DIAGNOSTIC_SELECT_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Diagnostic_Select_Screen;
}

class HNS_Diagnostic_Select_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Diagnostic_Select_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Diagnostic_Select_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_Diagnostic_Select_Cancel_clicked();

    void on_PB_Diagnostic_Select_PixOut_clicked();

    void on_PB_Diagnostic_Select_TestPatterns_clicked();

private:
    Ui::HNS_Diagnostic_Select_Screen *ui;
};

#endif // HNS_DIAGNOSTIC_SELECT_SCREEN_H
