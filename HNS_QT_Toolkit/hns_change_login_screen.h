#ifndef HNS_CHANGE_LOGIN_SCREEN_H
#define HNS_CHANGE_LOGIN_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Change_Login_Screen;
}

class HNS_Change_Login_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Change_Login_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Change_Login_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_ChangeLogin_Save_clicked();

    void on_PB_ChangeLogin_Cancel_clicked();

    void on_PB_ChangeLogin_ChUserPass_clicked();

    void on_PB_ChangeLogin_ChAdminPass_clicked();

    void on_PB_ChangeLogin_ChUserName_clicked();

    void on_PB_ChangeLogin_ChPass_clicked();

    void on_PB_ChangeLogin_Reset_clicked();

private:
    bool eventFilter(QObject *watched, QEvent *event);

    Ui::HNS_Change_Login_Screen *ui;

    HNS_Security f_temp_pass;
};

#endif // HNS_CHANGE_LOGIN_SCREEN_H
