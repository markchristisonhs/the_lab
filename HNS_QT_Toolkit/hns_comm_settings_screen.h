#ifndef HNS_COMM_SETTINGS_SCREEN_H
#define HNS_COMM_SETTINGS_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Comm_Settings_Screen;
}

class HNS_Comm_Settings_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Comm_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Comm_Settings_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdateCommInputs();

private slots:
    void on_PB_CommSet_Save_clicked();

    void on_PB_CommSet_Cancel_clicked();

    void on_PB_CommSet_Back_clicked();

    void LBL_CommSet_CommMode_clicked();
    void LBL_CommSet_IPMode_clicked();
    void LE_CommSet_IpAddr_clicked();
    void LE_CommSet_PortNum_clicked();
    void LE_CommSet_Gateway_clicked();
    void LE_CommSet_DNS_clicked();
    void LE_CommSet_Subnet_clicked();
    void LE_CommSet_IpAddr_editingFinished(const bool &enter_pressed);
    void LE_CommSet_PortNum_editingFinished(const bool &enter_pressed);
    void LE_CommSet_Gateway_editingFinished(const bool &enter_pressed);
    void LE_CommSet_DNS_editingFinished(const bool &enter_pressed);
    void LE_CommSet_Subnet_editingFinished(const bool &enter_pressed);

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void fVKP_Finished(const bool &enter_pressed);

    Ui::HNS_Comm_Settings_Screen *ui;

    HNS_IP_Settings f_ip_settings;
};

#endif // HNS_COMM_SETTINGS_SCREEN_H
