#ifndef HNS_MESSAGE_LIBRARY_SELECT_SCREEN_H
#define HNS_MESSAGE_LIBRARY_SELECT_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Message_Library_Select_Screen;
}

class HNS_Message_Library_Select_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Message_Library_Select_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Message_Library_Select_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_Library_Back_clicked();

    void on_PB_Library_Messages_clicked();

    void on_PB_Library_StaticAniImages_clicked();

private:
    Ui::HNS_Message_Library_Select_Screen *ui;
};

#endif // HNS_MESSAGE_LIBRARY_SELECT_SCREEN_H
