#ifndef HNS_STATUS_SCREEN_H
#define HNS_STATUS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Status_Screen;
}

class HNS_Status_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Status_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Status_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_Status_Back_clicked();

private:
    Ui::HNS_Status_Screen *ui;
};

#endif // HNS_STATUS_SCREEN_H
