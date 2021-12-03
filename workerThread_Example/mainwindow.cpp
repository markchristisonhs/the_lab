#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <graphics.h>
#include <utilities.h>
#include "loadntcip_worker.h"
#include <QDir>
#include <QStandardPaths>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , f_worker(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vector<unsigned char> tempvec(4,0);
    int itemp = 0;
    HNS_ByteArrayToint(tempvec);
    HNS_intToByteArray(itemp);

    HNS_Color dummy;

    ui->PTE_Debug->appendPlainText("Starting program");

    f_worker = new LoadNTCIP_Worker();
    f_worker->moveToThread(&f_worker_thread);
    connect(&f_worker_thread, &QThread::finished, f_worker, &QObject::deleteLater);
    connect(this,&MainWindow::operate, f_worker, &LoadNTCIP_Worker::doWork);
    connect(f_worker,&LoadNTCIP_Worker::resultReady,this,&MainWindow::fNTCIP_Ready);
    f_worker_thread.start();

    emit operate(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "NTCIP" + QDir::separator() + "NTCIP.xml",
                 ":/NTCIP/NTCIP.xml");

    ui->PTE_Debug->appendPlainText("Constructor done");
}

MainWindow::~MainWindow()
{
    f_worker_thread.quit();
    f_worker_thread.wait();
    delete ui;
}

void MainWindow::fNTCIP_Ready()
{
    f_ntcip = f_worker->fGetResult();

    ui->PTE_Debug->appendPlainText("Retrieved NTCIP");
}

void MainWindow::on_PB_Test_clicked()
{
    ui->PTE_Debug->appendPlainText("Button pushed!!!");
}
