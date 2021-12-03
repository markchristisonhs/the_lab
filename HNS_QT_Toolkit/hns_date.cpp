#include "hns_date.h"
#include "ui_hns_date.h"
#include <QTextStream>
#include <QKeyEvent>

HNS_Date::HNS_Date(QWidget *parent):
    QLineEdit(parent),
    ui(new Ui::HNS_Date),
    f_month(-1),
    f_day(-1),
    f_year(-1),
    f_cursor_position(0)
{
    ui->setupUi(this);

    installEventFilter(this);

    fSetDateString();
}

HNS_Date::HNS_Date(const QString &contents,
                   QWidget *parent) :
    QLineEdit(contents,parent),
    ui(new Ui::HNS_Date),
    f_month(-1),
    f_day(-1),
    f_year(-1),
    f_cursor_position(-1)
{
    ui->setupUi(this);

    fSetDateString();
}

void HNS_Date::fClear()
{
    f_month = -1;
    f_day = -1;
    f_year = -1;

    fSetDateString();
}

HNS_Date::~HNS_Date()
{
    delete ui;
}

void HNS_Date::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    case Qt::Key_Slash:
    case Qt::Key_Backspace:
        fKeyPressFSM(event->key());
        break;
    }
}

void HNS_Date::fSetDateString()
{
    QString date_string;
    QTextStream stream(&date_string);

    if(f_month > 0)
    {
        stream << f_month;
    }

    stream << "/";

    if(f_day > 0)
    {
        stream << f_day;
    }

    stream << "/";

    if(f_year >= 0)
    {
        stream << f_year;
    }

    setText(date_string);
    setCursorPosition(0);
}

QString HNS_Date::fCreateDateString(const int &month, const int &day, const int &year)
{
    QString date_string;
    QTextStream stream(&date_string);

    if(month > 0)
    {
        stream << month;
    }

    stream << "/";

    if(day > 0)
    {
        stream << day;
    }

    stream << "/";

    if(year >= 0)
    {
        stream << year;
    }

    return date_string;
}

bool HNS_Date::eventFilter(QObject *obj, QEvent *event)
{
    QLineEdit * w = qobject_cast<QLineEdit*>(obj);
    if(w && event->type() == QEvent::FocusIn)
    {
        QMetaObject::invokeMethod(this, "resetCursor",
                               Qt::QueuedConnection, Q_ARG(QWidget*, w));
    }

    return false;
}

void HNS_Date::fKeyPressFSM(const int &key)
{
    typedef enum
    {
        STATE_MONTH = 0,
        STATE_DAY,
        STATE_YEAR
    } type_key_state;

    static type_key_state state = STATE_MONTH;
    static QString tempstring;
    static int month = -1, day = -1, year = -1;

    switch(state)
    {
    case STATE_MONTH:
        if(key >= Qt::Key_0 && key <= Qt::Key_9)
        {
            tempstring += QKeySequence(key).toString();
            month = tempstring.toInt();
            setText(fCreateDateString(month,day,year));
        }
        else if(key == Qt::Key_Backspace)
        {
            if(!tempstring.isEmpty())
            {
                tempstring.chop(1);
                if(tempstring.isEmpty())
                {
                    month = -1;
                }
                else
                {
                    month = tempstring.toInt();
                }
                setText(fCreateDateString(month,day,year));
            }
        }
        break;
    case STATE_DAY:
        break;
    case STATE_YEAR:
        break;
    }
}
