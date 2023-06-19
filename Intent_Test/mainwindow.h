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
    void on_PB_Send_clicked();

    void on_PB_Send_2_clicked();

    void on_PB_Send_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
