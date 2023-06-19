#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_Time_clicked();

    void my_slot_VKP_Close();

    void my_slot_AM_PM_Pressed();

    void on_PB_Date_clicked();

    void on_PB_Test_clicked();

    void on_PB_Sort_clicked();

private:
    Ui::MainWindow *ui;

    QWidget *f_widget_in_work;
};
#endif // MAINWINDOW_H
