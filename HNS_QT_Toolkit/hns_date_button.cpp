#include "hns_date_button.h"

#include <QKeyEvent>

HNS_Date_Button::HNS_Date_Button(QWidget *parent) :
    QPushButton(parent)
  , f_timer(nullptr)
  , f_editing_enabled(false)
  , f_blink_on(true)
  , f_default_label("1/1/1970")
{
    fResetText();
    installEventFilter(this);
}

void HNS_Date_Button::fClear()
{
    f_date = QDate();
    f_date_stream.fClear();

    fResetText();
}

void HNS_Date_Button::fResetText()
{
    if(f_editing_enabled)
    {
        setText(QString::fromStdString(f_date_stream.fGetString(f_blink_on)));
    }
    else
    {
        if(f_date.isValid())
        {
            setText(f_date.toString());
        }
        else
        {
            setText(f_default_label);
        }
    }
}

void HNS_Date_Button::fSetEditMode(const bool &edit_mode)
{
    static QFont remember_font = font();

    if(edit_mode != f_editing_enabled)
    {
        if(edit_mode)
        {
            f_timer = new QTimer(this);

            connect(f_timer,SIGNAL(timeout()),this,SLOT(my_slot_TikTok()));
            f_timer->setInterval(1000);
            f_timer->start();
            f_blink_on = true;
            remember_font = font();
            setFont(QFont("Liberation Mono,13"));
            fResetText();
        }
        else
        {
            disconnect(f_timer,nullptr,nullptr,nullptr);
            delete f_timer;
            f_timer = nullptr;
            f_blink_on = true;
            setFont(remember_font);
            QDate temp_date = fGetDateStream();
            if(temp_date.isValid())
            {
                f_date = temp_date;
            }
            fResetText();
        }
    }
    f_editing_enabled = edit_mode;
}

void HNS_Date_Button::fSetDate(const QDate &date)
{
    if(date.isValid())
    {
        f_date = date;
        f_date_stream.fSetDate(date.year(),date.month(),date.day());
        fResetText();
    }
}

QDate HNS_Date_Button::fGetDate()
{
    return f_date;
}

void HNS_Date_Button::fSetDefaultLabel(const QString &label)
{
    f_default_label = label;
}

void HNS_Date_Button::my_slot_key_input(int key)
{
    if(f_editing_enabled)
    {
        if(key == Qt::Key_Backspace)
        {
            f_date_stream.fBackspace();
        }
        else if(key >= Qt::Key_0 && key <= Qt::Key_9)
        {
            f_date_stream.fAdd(static_cast<char>(key));
        }

        setText(QString::fromStdString(f_date_stream.fGetString()));
    }
}

void HNS_Date_Button::my_slot_TikTok()
{
    f_blink_on = !f_blink_on;
    fResetText();
}

bool HNS_Date_Button::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress)
    {
        QKeyEvent *my_event = (QKeyEvent *)event;
        my_slot_key_input(my_event->key());

        return true;
    }

    return false;
}

QDate HNS_Date_Button::fGetDateStream()
{
    QDate result;

    result.setDate(f_date_stream.fGetYear(),f_date_stream.fGetMonth(),f_date_stream.fGetDay());

    return result;
}
