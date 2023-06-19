#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <CRC.h>

unsigned char getCRC(unsigned char message[], unsigned char length);

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


void MainWindow::on_PB_Generate_clicked()
{
    QString tempstring;
    QTextStream out(&tempstring);
    std::vector <unsigned char> packet(2,0);
    unsigned char dayOfMonth = 0;

    for(dayOfMonth = 1; dayOfMonth <= 31; dayOfMonth++)
    {
        tempstring = "";
        unsigned char pwU8 = 0;
        unsigned char pwL8 = 0;

        packet[0]= dayOfMonth/10;
        packet[1] = dayOfMonth - (packet[0] * 10);
        pwU8 = getCRC(packet.data(),2);

        packet[0]= (dayOfMonth + 15)/20;
        packet[1] = dayOfMonth*2  - (packet[0] * 11);
        pwL8 = getCRC(packet.data(),2);

        out << dayOfMonth << " " << pwU8 << pwL8;

        ui->PTE_Result->appendPlainText(tempstring);
    }
}

unsigned char getCRC(unsigned char message[], unsigned char length)
{
    __uint8_t CRC7_POLY = 0x91;
    __uint8_t i, j, crc = 0;

    for (i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }
    return crc;
}

