#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <logaline_QT.h>

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

    void fApplicationStateChanged(Qt::ApplicationState state);

private:
    Ui::MainWindow *ui;

    HNS_LogALine_QT f_logaline;
};
#endif // MAINWINDOW_H
