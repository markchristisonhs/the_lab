#include "hns_vkp.h"
#include "ui_hns_vkp.h"
#include <QKeyEvent>

HNS_VKP::HNS_VKP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_VKP),
    f_focus_object(nullptr),
    f_enter_pressed(false),
    f_time_mode(false)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StyledBackground);

    fSetTimeMode(f_time_mode);
}

HNS_VKP::~HNS_VKP()
{
    delete ui;
}

void HNS_VKP::setFocusObject(QObject *object)
{
    f_focus_object = object;
}

QObject *HNS_VKP::getFocusObject() const
{
    return f_focus_object;
}

bool HNS_VKP::fGetEnterPressed() const
{
    return f_enter_pressed;
}

void HNS_VKP::fSetTimeMode(const bool &time_mode)
{
    f_time_mode = time_mode;

    if(f_time_mode)
    {
        ui->TB_Decimal->setText(":");
        ui->TB_Special->show();
        ui->TB_Special->setText("AM\nPM");
    }
    else
    {
        ui->TB_Decimal->setText(".");
        ui->TB_Special->hide();
    }
}

void HNS_VKP::show()
{
    f_enter_pressed = false;
    QWidget::show();
}

void HNS_VKP::on_TB_7_clicked()
{
    keyClicked(Qt::Key_7);
}

void HNS_VKP::on_TB_8_clicked()
{
    keyClicked(Qt::Key_8);
}

void HNS_VKP::on_TB_9_clicked()
{
    keyClicked(Qt::Key_9);
}

void HNS_VKP::on_TB_4_clicked()
{
    keyClicked(Qt::Key_4);
}

void HNS_VKP::on_TB_5_clicked()
{
    keyClicked(Qt::Key_5);
}

void HNS_VKP::on_TB_6_clicked()
{
    keyClicked(Qt::Key_6);
}

void HNS_VKP::on_TB_1_clicked()
{
    keyClicked(Qt::Key_1);
}

void HNS_VKP::on_TB_2_clicked()
{
    keyClicked(Qt::Key_2);
}

void HNS_VKP::on_TB_3_clicked()
{
    keyClicked(Qt::Key_3);
}

void HNS_VKP::on_TB_0_clicked()
{
    keyClicked(Qt::Key_0);
}

void HNS_VKP::on_TB_Decimal_clicked()
{
    if(f_time_mode)
    {
        keyClicked(Qt::Key_Colon);
    }
    else
    {
        keyClicked(Qt::Key_Period);
    }
}

void HNS_VKP::on_TB_Cancel_clicked()
{
    f_enter_pressed = false;

    emit closeRequest();
}

void HNS_VKP::on_TB_Enter_clicked()
{
    keyClicked(Qt::Key_Enter);

    f_enter_pressed = true;

    emit closeRequest();
}

void HNS_VKP::keyClicked(const int &key)
{
    int shifted_key = key;

    QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress,shifted_key,Qt::NoModifier,QString(key));
    if(f_focus_object != nullptr)
    {
        QGuiApplication::postEvent(f_focus_object,event);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, shifted_key, Qt::NoModifier,QString(key));
        QGuiApplication::postEvent(f_focus_object, releaseEvent);
    }

    emit SIG_Key_Pressed(shifted_key);
}

void HNS_VKP::on_TB_Delete_clicked()
{
    keyClicked(Qt::Key_Backspace);
}

void HNS_VKP::on_TB_Special_clicked()
{
    if(f_time_mode)
    {
        emit SIG_AM_PM_Pressed();
    }
}

