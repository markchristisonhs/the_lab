#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>

using namespace std;
using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_port(nullptr)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();

    for(int i=0;i<port_list.size();i++)
    {
        ui->CMB_Ports->addItem(port_list.at(i).portName());
    }

    f_ioio.fConnectComms(bind(&MainWindow::fSendData,this,_1,_2));

    f_ioio.fAddPort(0);
    f_ioio.fAddPort(1);
}

MainWindow::~MainWindow()
{
    if(ui->PB_Connect->text() == "Disconnect")
    {
        on_PB_Connect_clicked();
    }
    delete ui;
}

void MainWindow::fSendData(void *data, const int &size)
{
    if(f_port != nullptr && size > 0)
    {
        QByteArray temp_array;
        temp_array = QByteArray(reinterpret_cast<const char*>(data),size);
        f_port->write(temp_array);
    }
}

void MainWindow::on_PB_Connect_clicked()
{
    if(ui->PB_Connect->text() == "Connect")
    {
        QString result;
        QSerialPortInfo port_info(ui->CMB_Ports->currentText());
        result = port_info.systemLocation();

        if(!result.isEmpty())
        {
            f_port = new QSerialPort(this);

            connect(f_port,SIGNAL(readyRead()),this,SLOT(IncomingData()));

            f_port->setPortName(result);
            f_port->setBaudRate(QSerialPort::Baud9600);
            f_port->setDataBits(QSerialPort::Data8);
            f_port->setParity(QSerialPort::NoParity);
            f_port->setStopBits(QSerialPort::OneStop);
            f_port->setFlowControl(QSerialPort::NoFlowControl);
            f_port->open(QIODevice::ReadWrite);

            fInitControls();
            ui->PB_Connect->setText("Disconnect");
        }
    }
    else
    {
        disconnect(f_port,nullptr,this,nullptr);
        delete f_port;
        f_port = nullptr;

        ui->PB_Connect->setText("Connect");
    }
}

void MainWindow::fInitControls()
{
    ui->PB_In0->setEnabled(true);
    ui->PB_In1->setEnabled(true);

    ui->PB_Out0->setEnabled(true);
    ui->PB_Out1->setEnabled(true);

    ui->CB_Val0->setEnabled(false);
    ui->CB_Val1->setEnabled(false);
}

void MainWindow::fDisableControls()
{
    ui->PB_In0->setEnabled(false);
    ui->PB_In1->setEnabled(false);

    ui->PB_Out0->setEnabled(false);
    ui->PB_Out1->setEnabled(false);

    ui->CB_Val0->setEnabled(false);
    ui->CB_Val1->setEnabled(false);
}


void MainWindow::on_PB_In0_clicked()
{
    ui->PB_In0->setChecked(true);
    ui->PB_Out0->setChecked(false);

    ui->CB_Val0->setEnabled(false);

    f_ioio.fSetDigitalIn(0);
}


void MainWindow::on_PB_In1_clicked()
{
    ui->PB_In1->setChecked(true);
    ui->PB_Out1->setChecked(false);

    ui->CB_Val1->setEnabled(false);

    f_ioio.fSetDigitalIn(1);
}


void MainWindow::on_PB_Out0_clicked()
{
    ui->PB_In0->setChecked(false);
    ui->PB_Out0->setChecked(true);

    ui->CB_Val0->setEnabled(true);

    f_ioio.fSetDigitalOut(0,IOIO_VALUE_FALSE,IOIO_OUTPUT_MODE_NORMAL);
}


void MainWindow::on_PB_Out1_clicked()
{
    ui->PB_In1->setChecked(false);
    ui->PB_Out1->setChecked(true);

    ui->CB_Val1->setEnabled(true);

    f_ioio.fSetDigitalOut(1,IOIO_VALUE_FALSE,IOIO_OUTPUT_MODE_NORMAL);
}


void MainWindow::on_CB_Val0_stateChanged(int arg1)
{
    int value = IOIO_VALUE_UNDEFINED;
    if(arg1 == Qt::Checked)
    {
        value = IOIO_VALUE_TRUE;
    }
    else if(arg1 == Qt::Unchecked)
    {
        value = IOIO_VALUE_FALSE;
    }

    f_ioio.fSetValue(0,value);
}


void MainWindow::on_CB_Val1_stateChanged(int arg1)
{
    int value = IOIO_VALUE_UNDEFINED;
    if(arg1 == Qt::Checked)
    {
        value = IOIO_VALUE_TRUE;
    }
    else if(arg1 == Qt::Unchecked)
    {
        value = IOIO_VALUE_FALSE;
    }

    f_ioio.fSetValue(1,value);
}

void MainWindow::IncomingData()
{
    QByteArray temp_array;

    if(f_port != nullptr)
    {
        temp_array = f_port->readAll();

        f_ioio.fReceiveData(vector<unsigned char> (temp_array.begin(),temp_array.end()));
    }
}

