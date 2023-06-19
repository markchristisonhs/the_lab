#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NTCIP.h>

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
    void on_PB_Save_clicked();

    void on_PB_Load_clicked();

    void on_PB_Dirty_clicked();

private:
    Ui::MainWindow *ui;

    NTCIP_Node f_ntcip;

    void fKludge();
};
#endif // MAINWINDOW_H
