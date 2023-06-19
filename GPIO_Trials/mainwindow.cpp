#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniEnvironment>
#include <HNS_GPIO.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_input_ports({11,12,13,14})
    , f_input_states(4,0)
    , f_gpio(nullptr)
{
    ui->setupUi(this);

    f_gpio = new HNS_GPIO_Android(this);
    connect(f_gpio,SIGNAL(SIG_Input_Changed()),this,SLOT(my_slot_Input_Changed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fSetGPIO_Output(QPushButton *button, const int &port)
{
    int val = -1;

    //turn it on
    if(button->text() == "OFF")
    {
        val = 1;
        button->setText("ON");
    }
    //turn it off
    else
    {
        val = 0;
        button->setText("OFF");
    }

    qDebug() << "Main Window Running GPIO Output for port" << port << "and val" << val;
    f_gpio->fSetGPIO_Output(port,val);
}

void MainWindow::fUpdate()
{
    QVector<HNS_GPIO_Port> ports = QVector<HNS_GPIO_Port>::fromStdVector(f_gpio->fGetGPIO_Ports());
    qDebug() << "Ports is of size" << ports.size();
    for(int i=0;i<ports.size();i++)
    {
        if(ports[i].fGetPortNum() == 11 && ports[i].fGetDirection() == HNS_GPIO_INPUT)
        {
            ui->PB_GPIO_11->setText((ports[i].fGetVal() != 0) ? "ON" : "OFF");
        }
        if(ports[i].fGetPortNum() == 12 && ports[i].fGetDirection() == HNS_GPIO_INPUT)
        {
            ui->PB_GPIO_12->setText((ports[i].fGetVal() != 0) ? "ON" : "OFF");
        }
        if(ports[i].fGetPortNum() == 13 && ports[i].fGetDirection() == HNS_GPIO_INPUT)
        {
            ui->PB_GPIO_13->setText((ports[i].fGetVal() != 0) ? "ON" : "OFF");
        }
        if(ports[i].fGetPortNum() == 14 && ports[i].fGetDirection() == HNS_GPIO_INPUT)
        {
            ui->PB_GPIO_14->setText((ports[i].fGetVal() != 0) ? "ON" : "OFF");
        }
    }
}

void MainWindow::on_PB_GPIO_43_clicked()
{
    fSetGPIO_Output(ui->PB_GPIO_43,43);
}


void MainWindow::on_PB_GPIO_44_clicked()
{
    fSetGPIO_Output(ui->PB_GPIO_44,44);
}


void MainWindow::on_PB_GPIO_45_clicked()
{
    fSetGPIO_Output(ui->PB_GPIO_45,45);
}


void MainWindow::on_PB_GPIO_46_clicked()
{
    fSetGPIO_Output(ui->PB_GPIO_46,46);
}

void MainWindow::on_PB_Get_Inputs_clicked()
{
    fUpdate();
}

void MainWindow::my_slot_Input_Changed()
{
    fUpdate();
}
