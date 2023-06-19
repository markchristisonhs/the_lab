#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <hns_time_input_stream.h>
#include <QDebug>
#include <QTimeZone>

#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_widget_in_work(nullptr)
{
    HNS_Time_Input_Stream f_dummy;
    HNS_Date_Input_Stream f_dummy2;

    ui->setupUi(this);

    ui->GV_VKP->fSetTimeMode(true);

    ui->GV_VKP->hide();

    ui->PB_Time->fResetText();
    ui->PB_Date->fResetText();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PB_Time_clicked()
{
    if(ui->GV_VKP->isHidden())
    {
        f_widget_in_work = ui->PB_Time;
        ui->GV_VKP->show();
        ui->PB_Time->fClear();
        ui->PB_Time->fSetEditMode(true);
        ui->GV_VKP->fSetTimeMode(true);
        connect(ui->GV_VKP,SIGNAL(SIG_Key_Pressed(int)),ui->PB_Time,SLOT(my_slot_key_input(int)));
        connect(ui->GV_VKP, SIGNAL(SIG_AM_PM_Pressed()),this,SLOT(my_slot_AM_PM_Pressed()));
        connect(ui->GV_VKP,SIGNAL(closeRequest()),this,SLOT(my_slot_VKP_Close()));
    }
}

void MainWindow::my_slot_VKP_Close()
{
    QTime time_result;
    QDate date_result;

    if(f_widget_in_work == ui->PB_Date)
    {
        date_result = ui->PB_Date->fGetDate();
    }
    if(f_widget_in_work == ui->PB_Time)
    {
        time_result = ui->PB_Time->fGetTime();
    }

    ui->GV_VKP->hide();
    ui->PB_Time->fSetEditMode(false);
    ui->PB_Date->fSetEditMode(false);
    disconnect(ui->GV_VKP, nullptr,nullptr,nullptr);
}

void MainWindow::my_slot_AM_PM_Pressed()
{
    ui->PB_Time->fToggleAMPM();
}


void MainWindow::on_PB_Date_clicked()
{
    if(ui->GV_VKP->isHidden())
    {
        f_widget_in_work = ui->PB_Date;
        ui->GV_VKP->show();
        ui->PB_Date->fClear();
        ui->PB_Date->fSetEditMode(true);
        ui->GV_VKP->fSetTimeMode(false);
        connect(ui->GV_VKP,SIGNAL(SIG_Key_Pressed(int)),ui->PB_Date,SLOT(my_slot_key_input(int)));
        connect(ui->GV_VKP,SIGNAL(closeRequest()),this,SLOT(my_slot_VKP_Close()));
    }
}


void MainWindow::on_PB_Test_clicked()
{
//    static bool toggle = false;

//    QFont test_font = ui->PB_Date->font();

//    qDebug() << "Font is " << test_font;

//    if(toggle)
//    {
//        ui->PB_Date->setFont(QFont("Liberation Mono,11"));
//    }
//    else
//    {
//        ui->PB_Date->setFont(QFont("Lato,11"));
//    }
//    toggle = !toggle;

    QString time_zone_string = "America/Chicago";
    QTimeZone temp_tz(time_zone_string.toUtf8());
    QDateTime date_time;
    date_time.setTimeZone(temp_tz);
    date_time.setDate(ui->PB_Date->fGetDate());
    date_time.setTime(ui->PB_Time->fGetTime());
    qint64 epoch = date_time.toSecsSinceEpoch();

    qDebug() << date_time;
    qDebug() << epoch;
}

struct myclass {
    bool operator() (pair<int,size_t> i, pair<int,size_t> j) {return (i.first < j.first);}
} myobject;


void MainWindow::on_PB_Sort_clicked()
{
    vector<int> myvector = {32,71,12,45,26,80,53,33};
    vector<pair<int,size_t> > paired_vector(myvector.size());
    for(size_t ui=0;ui<myvector.size();ui++)
    {
        paired_vector[ui].first = myvector[ui];
        paired_vector[ui].second = ui;
    }

    sort(paired_vector.begin(),paired_vector.end(),myobject);

    QString tempstring;
    for(size_t ui=0;ui<myvector.size();ui++)
    {
        tempstring += QString::number(paired_vector[ui].first) + ":" + QString::number(paired_vector[ui].second) + " / ";
    }

    qDebug() << tempstring;
}

