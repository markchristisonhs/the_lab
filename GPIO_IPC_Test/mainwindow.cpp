#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <HNS_GPIO.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PB_Create_clicked()
{
    HNS_GPIO_Request request;
    switch(ui->CMB_Action->currentIndex())
    {
    case 0:
        request.fSetAction(HNS_GPIO_ACTION_SET);
        break;
    case 1:
        request.fSetAction(HNS_GPIO_ACTION_GET);
        break;
    case 2:
        request.fSetAction(HNS_GPIO_ACTION_SETUP);
        break;
    default:
        request.fSetAction(HNS_GPIO_ACTION_NONE);
    }
    request.fSetPort(ui->LE_Port->text().toInt());
    request.fSetValue(ui->LE_Value->text().toInt());

    ui->LE_Command->setText(QString::fromStdString(request.fGetCommandString()));
}


void MainWindow::on_PB_Parse_clicked()
{
    HNS_GPIO_Response response;

    response.fReadResponseString(ui->LE_Response->text().toStdString());

    ui->LE_RespError->setText(QString("%1").arg(response.fGetError()));
    ui->LE_RespPort->setText(QString("%1").arg(response.fGetPort()));
    ui->LE_RespValue->setText(QString("%1").arg(response.fGetValue()));
}

