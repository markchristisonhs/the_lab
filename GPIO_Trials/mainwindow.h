#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include "hns_gpio_service_interface.h"
#include "hns_gpio_android.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fStartService();

    void fRetrieveNewInput(const int &port, const int &val);

private slots:
    void on_PB_GPIO_43_clicked();

    void on_PB_GPIO_44_clicked();

    void on_PB_GPIO_45_clicked();

    void on_PB_GPIO_46_clicked();

    void on_PB_Get_Inputs_clicked();

    void my_slot_Input_Changed();

private:
    void fSetGPIO_Output(const int &port, const int &val);
    void fSetGPIO_Output(QPushButton *button, const int &port);
    int fGetGPIO_Input(const int &port = -1);
    void fUpdate();
    Ui::MainWindow *ui;

    QVector<int> f_input_ports;
    QVector<int> f_input_states;

    HNS_GPIO_Android *f_gpio;
};
#endif // MAINWINDOW_H
