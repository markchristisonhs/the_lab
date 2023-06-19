#include "hns_date_widget.h"
#include "ui_hns_date_widget.h"

#include <QTimer>
#include <QDateTime>

HNS_Date_Widget::HNS_Date_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_Date_Widget)
  , f_timer(new QTimer(this))
{
    ui->setupUi(this);

    fTickTock();
    connect(f_timer,SIGNAL(timeout()),this,SLOT(fTickTock()));
    f_timer->setInterval(1000);
    f_timer->start();
}

HNS_Date_Widget::~HNS_Date_Widget()
{
    delete ui;
}

void HNS_Date_Widget::fTickTock()
{
    ui->LBL_Date->setText(QDateTime::currentDateTime().toString("MM/dd/yyyy"));
}
