#ifndef HNS_VKP_H
#define HNS_VKP_H

#include <QWidget>

namespace Ui {
class HNS_VKP;
}

class HNS_VKP : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_VKP(QWidget *parent = nullptr);
    ~HNS_VKP();

    void setFocusObject(QObject *object);
    QObject *getFocusObject() const;

    bool fGetEnterPressed() const;

    void fSetTimeMode(const bool &time_mode);

public slots:
    void show();

private slots:
    void on_TB_7_clicked();

    void on_TB_8_clicked();

    void on_TB_9_clicked();

    void on_TB_4_clicked();

    void on_TB_5_clicked();

    void on_TB_6_clicked();

    void on_TB_1_clicked();

    void on_TB_2_clicked();

    void on_TB_3_clicked();

    void on_TB_0_clicked();

    void on_TB_Decimal_clicked();

    void on_TB_Cancel_clicked();

    void on_TB_Enter_clicked();

    void on_TB_Delete_clicked();

    void on_TB_Special_clicked();

signals:

    void closeRequest();

    void SIG_AM_PM_Pressed();

    void SIG_Key_Pressed(const int key);

private:
    void keyClicked(const int &key);

    Ui::HNS_VKP *ui;

    QObject *f_focus_object;

    bool f_enter_pressed;

    bool f_time_mode;
};

#endif // HNS_VKP_H
