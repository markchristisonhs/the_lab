#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QStandardPaths>
#include <QSettings>

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


void MainWindow::on_PB_Save_clicked()
{
    QString part_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString path = part_path + QDir::separator() + "config.ini";

    if(!QFile(path).exists())
    {
        if(!QFileInfo::exists(part_path))
        {
            QDir dir = QDir(part_path);
            dir.mkpath(part_path);
        }
    }

    QSettings settingsfile(path,QSettings::IniFormat);

    QStringList thing_list = {"milk","eggs","bacon"};

    settingsfile.setValue("Test/Things",thing_list);

    settingsfile.setValue("Test/One_Thing","cow");

    settingsfile.sync();
}


void MainWindow::on_PB_Load_clicked()
{
    QString part_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString path = part_path + QDir::separator() + "config.ini";

    if(!QFile(path).exists())
    {
        if(!QFileInfo::exists(part_path))
        {
            QDir dir = QDir(part_path);
            dir.mkpath(part_path);
        }
    }

    QSettings settingsfile(path,QSettings::IniFormat);

    QStringList thing_list;

    thing_list = settingsfile.value("Test/Things").toStringList();

    thing_list = settingsfile.value("Test/One_Thing").toStringList();

    settingsfile.sync();
}

