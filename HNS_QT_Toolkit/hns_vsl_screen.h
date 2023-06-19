#ifndef HNS_VSL_SCREEN_H
#define HNS_VSL_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <typedefs.h>

namespace Ui {
class HNS_VSL_Screen;
}

class HNS_VSL_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_VSL_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_VSL_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fSetSignBrd(const HNS_SignBoard &sign_board);

    int fGetBeaconMode();

private slots:
    void on_PB_VSL_Up_clicked();

    void on_PB_VSL_Down_clicked();

    void on_PB_VSL_Beacons_clicked();

signals:
    void SIG_VSL_Up_Request();
    void SIG_VSL_Dn_Request();
    void SIG_VSL_Beacon_Request(const int &beacon_mode);

private:
    Ui::HNS_VSL_Screen *ui;
};

#endif // HNS_VSL_SCREEN_H
