#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hns_aux_test_thread.h"
#include "hns_aux_input.h"

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
//    void my_slot_Aux_Thread_Finish();

    void on_SLD_Time_valueChanged(int value);

    void on_LE_Time_editingFinished();

    void on_PB_Change1_clicked();

    void on_PB_Change2_clicked();

    void on_PB_Change3_clicked();

    void on_PB_Change4_clicked();

    void on_CBX_Input1_stateChanged(int arg1);

    void on_CBX_Input2_stateChanged(int arg1);

    void on_CBX_Input3_stateChanged(int arg1);

    void on_CBX_Input4_stateChanged(int arg1);

    void on_PB_RunMessage_clicked();

    void on_PB_Blank_clicked();

    void on_PB_Reset_clicked();

private:
    void fCreateInputs();
    HNS_NTCIP_MessageActivationCode fGetRunningActivationCode();

    Ui::MainWindow *ui;
//    HNS_Aux_Test_Thread *f_aux_thread;
//    HNS_Aux_Inputs f_inputs;
    HNS_Aux_Inputs *f_inputs;
};
#endif // MAINWINDOW_H
