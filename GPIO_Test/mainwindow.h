#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtAndroid>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void activityReceiver(int requestCode, int resultCode, const QAndroidJniObject &data);

private slots:
    void on_CBX_GPIO_43_stateChanged(int arg1);

    void on_CBX_GPIO_44_stateChanged(int arg1);

    void on_CBX_GPIO_45_stateChanged(int arg1);

    void on_CBX_GPIO_46_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    void fSendCommand(const int &port, const bool &value);
};
#endif // MAINWINDOW_H
