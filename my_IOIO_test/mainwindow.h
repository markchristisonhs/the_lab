#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <ioio.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fSendData(void *data, const int &size);

private slots:
    void on_PB_Connect_clicked();

    void on_PB_In0_clicked();

    void on_PB_In1_clicked();

    void on_PB_Out0_clicked();

    void on_PB_Out1_clicked();

    void on_CB_Val0_stateChanged(int arg1);

    void on_CB_Val1_stateChanged(int arg1);

    void IncomingData();

private:
    Ui::MainWindow *ui;
    QSerialPort *f_port;

    IOIO f_ioio;

    void fInitControls();
    void fDisableControls();
};
#endif // MAINWINDOW_H
