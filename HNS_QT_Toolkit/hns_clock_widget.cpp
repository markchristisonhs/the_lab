#include "hns_clock_widget.h"
#include "ui_hns_clock_widget.h"

#include <QTimer>
#include <QDateTime>

HNS_Clock_Widget::HNS_Clock_Widget(QWidget *parent, const QString &timestring) :
    QWidget(parent),
    ui(new Ui::HNS_Clock_Widget)
  , f_timer(new QTimer(this))
  , f_timestring(timestring)
{
    ui->setupUi(this);

    fTickTock();
    connect(f_timer,SIGNAL(timeout()),this,SLOT(fTickTock()));
    f_timer->setInterval(1000);
    f_timer->start();
}

HNS_Clock_Widget::~HNS_Clock_Widget()
{
    delete ui;
}

void HNS_Clock_Widget::fTickTock()
{
    ui->LBL_Time->setText(QDateTime::currentDateTime().toString(f_timestring));
}
