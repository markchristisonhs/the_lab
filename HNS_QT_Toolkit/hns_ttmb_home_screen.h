#ifndef HNS_TTMB_HOME_SCREEN_H
#define HNS_TTMB_HOME_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_TTMB_Home_Screen;
}

class HNS_TTMB_Home_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_TTMB_Home_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_TTMB_Home_Screen();

    type_widget fGetType() const {return HNS_WIDGET_HOME_TTMB;}
    void fSetAccess(const type_user_level &access);

    void fSetSignBrd(const HNS_SignBoard &sign_board);
private slots:
    void on_PB_Raise_Sign_clicked();

    void on_PB_Blank_Sign_clicked();

    void on_PB_Blank_Lower_clicked();

    void on_PB_LeftArrow_clicked();

    void on_PB_RightArrow_clicked();

    void on_PB_DoubleArrow_clicked();

    void on_PB_StoppedTraffic_clicked();

    void on_PB_RoadWork_clicked();

    void on_PB_PaintCrew_clicked();

    void on_PB_Custom1_clicked();

    void on_PB_Custom2_clicked();

    void on_PB_Custom3_clicked();

    void on_PB_Custom4_clicked();

    void on_PB_SetCustom_clicked();

private:
    Ui::HNS_TTMB_Home_Screen *ui;

signals:
    void SIG_Msg_Stop_Request();
};

#endif // HNS_TTMB_HOME_SCREEN_H
