#include "hns_time_button.h"

#include <QKeyEvent>

HNS_Time_Button::HNS_Time_Button(QWidget *parent) :
    QPushButton(parent)
  , f_timer(nullptr)
  , f_editing_enabled(false)
  , f_blink_on(true)
  , f_default_label("12:00")
{
    fResetText();
    installEventFilter(this);
}

void HNS_Time_Button::fClear()
{
    f_time_stream.fClear();
    f_time = QTime();
    f_editing_enabled = false;

    fResetText();
}

void HNS_Time_Button::fResetText()
{
    if(f_editing_enabled)
    {
        setText(QString::fromStdString(f_time_stream.fGetString(f_blink_on)));
    }
    else
    {
        if(f_time.isValid())
        {
            setText(f_time.toString("h:mm AP"));
        }
        else
        {
            setText(f_default_label);
        }
    }
}

void HNS_Time_Button::fSetEditMode(const bool &edit_mode, const bool &accept)
{
    static QFont remember_font = font();

    if(edit_mode != f_editing_enabled)
    {
        if(edit_mode)
        {
            f_timer = new QTimer(this);

            connect(f_timer,SIGNAL(timeout()),SLOT(my_slot_TikTok()));
            f_timer->setInterval(1000);
            f_timer->start();
            f_time_stream.fClear();
            f_blink_on = true;
            remember_font = font();
            setFont(QFont("Liberation Mono,13"));
        }
        else
        {
            disconnect(f_timer,nullptr,nullptr,nullptr);
            delete f_timer;
            f_timer = nullptr;
            f_blink_on = true;
            setFont(remember_font);
            if(accept)
            {
                QTime temp_time = fGetTimeStream();
                if(temp_time.isValid())
                {
                    f_time = temp_time;
                }
            }
        }
    }
    f_editing_enabled = edit_mode;
    fResetText();
}

void HNS_Time_Button::fToggleAMPM()
{
    if(f_editing_enabled)
    {
        f_time_stream.fToggleAM_PM();
        fResetText();
    }
}

void HNS_Time_Button::fSetTime(const QTime &time)
{
    int hour = 0,minute = 0;
    bool is_pm = false;
    QString tempstring;

    f_time = time;

    if(f_time.isValid())
    {
        tempstring = time.toString("h ap");
        hour = time.hour();
        minute = time.minute();
        if(hour > 12)
        {
            hour -= 12;
            is_pm = true;
        }
        f_time_stream.fSetTime(hour, minute, is_pm);

        f_time = time;

        fResetText();
    }
}

QTime HNS_Time_Button::fGetTime()
{
    return f_time;
}

void HNS_Time_Button::fSetDefaultLabel(const QString &label)
{
    f_default_label = label;

    fResetText();
}

void HNS_Time_Button::my_slot_key_input(int key)
{
    if(f_editing_enabled)
    {
        if(key == Qt::Key_Backspace)
        {
            f_time_stream.fBackspace();
        }
        else if(key >= Qt::Key_0 && key <= Qt::Key_9)
        {
            f_time_stream.fAdd(static_cast<char>(key));
        }

        setText(QString::fromStdString(f_time_stream.fGetString()));
    }
}

void HNS_Time_Button::my_slot_ampm_toggle()
{
    if(f_editing_enabled)
    {
        fToggleAMPM();
    }
}

void HNS_Time_Button::my_slot_TikTok()
{
    f_blink_on = !f_blink_on;
    fResetText();
}

bool HNS_Time_Button::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress)
    {
        QKeyEvent *my_event = (QKeyEvent *)event;
        my_slot_key_input(my_event->key());

        return true;
    }

    return false;
}

QTime HNS_Time_Button::fGetTimeStream()
{
    QTime result;

    result.setHMS(f_time_stream.fGetHour24(),f_time_stream.fGetMinute(),0);

    return result;
}
