#ifndef HNS_PIXOUT_SCREEN_H
#define HNS_PIXOUT_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_PixOut_Screen;
}

class HNS_PixOut_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_PixOut_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_PixOut_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_Diagnostic_Back_clicked();

    void on_PB_RunPixelOutTest_clicked();

private:
    Ui::HNS_PixOut_Screen *ui;
};

#endif // HNS_PIXOUT_SCREEN_H
