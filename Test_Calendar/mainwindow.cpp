#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimeZone>

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


void MainWindow::on_BTN_Test_clicked()
{
    QList<QByteArray> list_of_time_zones = QTimeZone::availableTimeZoneIds(QLocale::UnitedStates);

    for(int i = 0; i<list_of_time_zones.size();i++)
    {
        ui->PTE_Debug->appendPlainText(QString::fromStdString(list_of_time_zones[i].toStdString()));
    }
}

