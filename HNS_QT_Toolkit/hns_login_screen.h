#ifndef HNS_LOGIN_SCREEN_H
#define HNS_LOGIN_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <QGraphicsScene>

namespace Ui {
class HNS_Login_Screen;
}

class HNS_Login_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Login_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Login_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fSetSignName(const QString &sign_name);
    void fSetVersion(const QString &version);

    void fSetPowerData(const double &batt_voltage, const double &solar_current, const double &solar_voltage);

    void fSetNTCIP_Waiting(const bool &waiting);

    void fRejectLogin();

    void fShowDebug(const QString &text);

private slots:
    void on_PB_LogIn_clicked();

    void on_LE_Username_textEdited(const QString &arg1);

    void on_LE_Username_editingFinished();

    void on_LE_Password_editingFinished();

signals:
    void SIG_Login_Request(const QString &username, const QString &pass);

private:
    bool eventFilter(QObject *obj, QEvent *event);

    Ui::HNS_Login_Screen *ui;

    QGraphicsScene *f_scene;
    bool f_first_input;
};

#endif // HNS_LOGIN_SCREEN_H
