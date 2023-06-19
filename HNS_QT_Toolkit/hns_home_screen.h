#ifndef HNS_HOME_SCREEN_H
#define HNS_HOME_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <vector>
#include <fonts.h>
#include <graphics.h>
#include <MessageBoard.h>

namespace Ui {
class HNS_Home_Screen;
}

class HNS_Home_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Home_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Home_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fSetLogo(const QImage &logo);

    void fSetSignBrd(const HNS_SignBoard &sign_board);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private:
    Ui::HNS_Home_Screen *ui;
    type_user_level f_current_access;

    void getActiveSchedule();

private slots:
    void my_Slot_Home_Preview_clicked();

    void on_PB_Home_EditMessage_clicked();

    void on_PB_Home_CreateMessage_clicked();

    void on_PB_Home_StopMessage_clicked();

    void on_PB_Home_NewSchedule_clicked();

signals:
    void SIG_Message_Preview_Touched();
    void SIG_Msg_Stop_Request();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // HNS_HOME_SCREEN_H
