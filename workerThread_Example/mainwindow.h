#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NTCIP.h>
#include <QThread>
#include "loadntcip_worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QThread f_worker_thread;
    LoadNTCIP_Worker *f_worker;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void fNTCIP_Ready();

signals:
    void operate(const QString &, const QString &);

private slots:
    void on_PB_Test_clicked();

private:
    Ui::MainWindow *ui;

    NTCIP_Node f_ntcip;
};
#endif // MAINWINDOW_H
