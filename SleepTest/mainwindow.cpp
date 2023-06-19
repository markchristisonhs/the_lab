#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    f_logaline.fSetLogfileLocation(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "log.txt");

    f_logaline.fLogCurrentTime();
    f_logaline.fLogALine("Running main constructor now","Main");

    ui->PTE_Debug->appendPlainText("Running main constructor now");

    QApplication *application = static_cast<QApplication *>(QApplication::instance());

    connect(application,SIGNAL(applicationStateChanged(Qt::ApplicationState)),this,SLOT(fApplicationStateChanged(Qt::ApplicationState)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fApplicationStateChanged(Qt::ApplicationState state)
{
    QString s("Application state has changed to ");

    switch(state)
    {
    case Qt::ApplicationSuspended:
        s += QString("suspended");
        break;
    case Qt::ApplicationHidden:
        s += QString("hidden");
        break;
    case Qt::ApplicationInactive:
        s += QString("inactive");
        break;
    case Qt::ApplicationActive:
        s += QString("active");
        break;
    default:
        s += QString("unkown");
        break;
    }

    f_logaline.fLogALine(s.toStdString(),"fApplicationStateChanged");
    f_logaline.fLogCurrentTime();

    ui->PTE_Debug->appendPlainText(s);
}

