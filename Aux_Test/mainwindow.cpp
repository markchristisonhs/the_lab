#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <structures.h>
#include <QDebug>

using namespace NTCIP_MESSAGE;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_inputs(nullptr)
{
    ui->setupUi(this);

    fCreateInputs();

    HNS_IP_Settings dummy_ip;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SLD_Time_valueChanged(int value)
{
    ui->LE_Time->setText(QString("%1").arg(value));

    HNS_NTCIP_MessageActivationCode result_code;
    int action = HNS_AUX_ACTION_NONE;

    f_inputs->fIsRunning(ui->SLD_Time->value(),&result_code,action);

    if(action != HNS_AUX_ACTION_NONE && result_code != fGetRunningActivationCode())
    {
        ui->LE_MessageNo_Current->setText(QString("%1").arg(result_code.fGetMessageNo()));
        ui->LE_MemoryType_Current->setText(QString("%1").arg(result_code.fGetMemoryType()));
    }
}


void MainWindow::on_LE_Time_editingFinished()
{
    int time = ui->LE_Time->text().toInt();
    ui->SLD_Time->setSliderPosition(time);
}


void MainWindow::on_PB_Change1_clicked()
{
    HNS_NTCIP_MessageIDCode id_code;
    int itemp;

    itemp = 2;
    itemp = ui->LE_MemoryType1->text().toInt();
    id_code.fSetMemoryType(static_cast<type_ntcip_message_memory_type>(itemp));

    itemp = 1;
    itemp = ui->LE_MessageNo1->text().toInt();
    id_code.fSetMessageNumber(itemp);

    itemp = 10;
    itemp = ui->LE_Duration1->text().toInt();

    f_inputs->fSetInput(0,id_code,ui->CBX_Enable1->isChecked(),itemp);
}


void MainWindow::on_PB_Change2_clicked()
{
    HNS_NTCIP_MessageIDCode id_code;
    int itemp;

    itemp = 2;
    itemp = ui->LE_MemoryType2->text().toInt();
    id_code.fSetMemoryType(static_cast<type_ntcip_message_memory_type>(itemp));

    itemp = 1;
    itemp = ui->LE_MessageNo2->text().toInt();
    id_code.fSetMessageNumber(itemp);

    itemp = 10;
    itemp = ui->LE_Duration2->text().toInt();

    f_inputs->fSetInput(1,id_code,ui->CBX_Enable2->isChecked(),itemp);
}


void MainWindow::on_PB_Change3_clicked()
{
    HNS_NTCIP_MessageIDCode id_code;
    int itemp;

    itemp = 2;
    itemp = ui->LE_MemoryType3->text().toInt();
    id_code.fSetMemoryType(static_cast<type_ntcip_message_memory_type>(itemp));

    itemp = 1;
    itemp = ui->LE_MessageNo3->text().toInt();
    id_code.fSetMessageNumber(itemp);

    itemp = 10;
    itemp = ui->LE_Duration3->text().toInt();

    f_inputs->fSetInput(2,id_code,ui->CBX_Enable3->isChecked(),itemp);
}


void MainWindow::on_PB_Change4_clicked()
{
    HNS_NTCIP_MessageIDCode id_code;
    int itemp;

    itemp = 2;
    itemp = ui->LE_MemoryType4->text().toInt();
    id_code.fSetMemoryType(static_cast<type_ntcip_message_memory_type>(itemp));

    itemp = 1;
    itemp = ui->LE_MessageNo4->text().toInt();
    id_code.fSetMessageNumber(itemp);

    itemp = 10;
    itemp = ui->LE_Duration4->text().toInt();

    f_inputs->fSetInput(3,id_code,ui->CBX_Enable4->isChecked(),itemp);
}


void MainWindow::on_CBX_Input1_stateChanged(int arg1)
{
    int val = 0;

    if(f_inputs != nullptr)
    {
        if(arg1 == Qt::Checked)
        {
            val = 1;
        }
        f_inputs->fRecvInput(val,ui->SLD_Time->value(),11,fGetRunningActivationCode());
    }
}


void MainWindow::on_CBX_Input2_stateChanged(int arg1)
{
    int val = 0;
    if(f_inputs != nullptr)
    {
        if(arg1 == Qt::Checked)
        {
            val = 1;
        }
        f_inputs->fRecvInput(val,ui->SLD_Time->value(),12,fGetRunningActivationCode());
    }
}


void MainWindow::on_CBX_Input3_stateChanged(int arg1)
{
    int val = 0;
    if(f_inputs != nullptr)
    {
        if(arg1 == Qt::Checked)
        {
            val = 1;
        }
        f_inputs->fRecvInput(val,ui->SLD_Time->value(),13,fGetRunningActivationCode());
    }
}


void MainWindow::on_CBX_Input4_stateChanged(int arg1)
{
    int val = 0;
    if(f_inputs != nullptr)
    {
        if(arg1 == Qt::Checked)
        {
            val = 1;
        }
        f_inputs->fRecvInput(val,ui->SLD_Time->value(),14,fGetRunningActivationCode());
    }
}


void MainWindow::on_PB_RunMessage_clicked()
{
    ui->LE_MessageNo_Current->setText(ui->LE_MessageNo_Main->text());
    ui->LE_MemoryType_Current->setText(ui->LE_MemoryType_Main->text());
    if(f_inputs != nullptr)
    {
        f_inputs->fCancel();
    }
}


void MainWindow::on_PB_Blank_clicked()
{
    ui->LE_MessageNo_Current->setText("127");
    ui->LE_MemoryType_Current->setText("7");
    if(f_inputs != nullptr)
    {
        f_inputs->fCancel();
    }
}


void MainWindow::on_PB_Reset_clicked()
{
    ui->SLD_Time->setValue(0);
    if(f_inputs != nullptr)
    {
        delete f_inputs;
        f_inputs = nullptr;
    }
    fCreateInputs();
}

void MainWindow::fCreateInputs()
{
    f_inputs = (HNS_Aux_Inputs *) new HNS_Aux_Inputs_PennStyle({11,12,13,14},0);

    on_PB_Change1_clicked();
    on_PB_Change2_clicked();
    on_PB_Change3_clicked();
    on_PB_Change4_clicked();
}

HNS_NTCIP_MessageActivationCode MainWindow::fGetRunningActivationCode()
{
    HNS_NTCIP_MessageActivationCode result;

    int itemp = 2;
    itemp = ui->LE_MemoryType_Current->text().toInt();
    result.fSetMemoryType(static_cast<type_ntcip_message_memory_type>(itemp));

    itemp = 1;
    itemp = ui->LE_MessageNo_Current->text().toInt();
    result.fSetMessageNo(itemp);

    result.fSetCRC(0);
    result.fSetDuration(0xFFFF);
    result.fSetSrcAddr("127.0.0.1");

    return result;
}
