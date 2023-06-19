#ifndef HNS_RS485_DEBUG_SCREEN_H
#define HNS_RS485_DEBUG_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_RS485_Debug_Screen;
}

class HNS_RS485_Debug_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_RS485_Debug_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_RS485_Debug_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_Db485_Cancel_clicked();

private:
    Ui::HNS_RS485_Debug_Screen *ui;
};

#endif // HNS_RS485_DEBUG_SCREEN_H
