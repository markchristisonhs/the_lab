#ifndef HNS_DEBUG_GPIO_SCREEN_H
#define HNS_DEBUG_GPIO_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Debug_GPIO_Screen;
}

class HNS_Debug_GPIO_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Debug_GPIO_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Debug_GPIO_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_Debug_GPIO_Cancel_clicked();

    void on_PB_Debug_GPIO_43_clicked(bool checked);

    void on_PB_Debug_GPIO_44_clicked(bool checked);

    void on_PB_Debug_GPIO_45_clicked(bool checked);

    void on_PB_Debug_GPIO_46_clicked(bool checked);

private:
    Ui::HNS_Debug_GPIO_Screen *ui;
};

#endif // HNS_DEBUG_GPIO_SCREEN_H
