#ifndef HNS_VKB_H
#define HNS_VKB_H

#include <QWidget>

typedef enum
{
    HNS_KB_NORMAL = 0,
    HNS_KB_CAPS,
    HNS_KB_SYM
} type_keyboard_mode;

namespace Ui {
class HNS_VKB;
}

class HNS_VKB : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_VKB(QWidget *parent = nullptr);
    ~HNS_VKB();

    void setFocusObject(QObject *object);
    QObject *getFocusObject();

    void fSetNarrowMode(const int &x, const int &y, const bool &mode);

    void fSetKeyMode(const type_keyboard_mode &mode);

private slots:
    void on_PB_Q_clicked();

    void on_PB_W_clicked();

    void on_PB_E_clicked();

    void on_PB_R_clicked();

    void on_PB_T_clicked();

    void on_PB_Y_clicked();

    void on_PB_U_clicked();

    void on_PB_I_clicked();

    void on_PB_O_clicked();

    void on_PB_P_clicked();

    void on_PB_Bkspc_clicked();

    void on_PB_A_clicked();

    void on_PB_S_clicked();

    void on_PB_D_clicked();

    void on_PB_F_clicked();

    void on_PB_G_clicked();

    void on_PB_H_clicked();

    void on_PB_J_clicked();

    void on_PB_K_clicked();

    void on_PB_L_clicked();

    void on_PB_Return_clicked();

    void on_PB_Shift_clicked();

    void on_PB_Z_clicked();

    void on_PB_X_clicked();

    void on_PB_C_clicked();

    void on_PB_V_clicked();

    void on_PB_B_clicked();

    void on_PB_N_clicked();

    void on_PB_M_clicked();

    void on_PB_Semicolon_clicked();

    void on_PB_Apostrophe_clicked();

    void on_PB_Dash_clicked();

    void on_PB_Symbols_clicked();

    void on_PB_BANG_clicked();

    void on_PB_Space_clicked();

    void on_PB_Comma_clicked();

    void on_PB_Period_clicked();

    void on_PB_Ampersand_clicked();

signals:

    void closeRequest();

private:

    void keyClicked(const int &key);

    void fUpdateKB();

    Ui::HNS_VKB *ui;

    QObject *f_focus_object;

    type_keyboard_mode f_mode;
};

#endif // HNS_VKB_H
