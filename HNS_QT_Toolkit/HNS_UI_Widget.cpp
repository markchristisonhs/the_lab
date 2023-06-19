#include "HNS_UI_Widget.h"
#include <QToolButton>
#include <QSpinBox>
#include <QStringList>
#include <QStringListModel>

HNS_UI_Widget::HNS_UI_Widget(QWidget *parent, HNS_Machine_Interface *machine) :
    QWidget(parent)
  , f_shown_screen(nullptr)
  , f_machine(machine)
  , f_vkb(nullptr)
  , f_vkp(nullptr)
  , f_calendar(nullptr)
  , f_yearView(nullptr)
{
}

HNS_UI_Widget::~HNS_UI_Widget()
{
}

void HNS_UI_Widget::my_Slot_Shown_Screen_Destroyed(QObject *)
{
    f_shown_screen = nullptr;
}

void HNS_UI_Widget::my_slot_close_VKP()
{
    emit SIG_VKP_Closed(f_vkp->fGetEnterPressed());

    fHideKP();
}

void HNS_UI_Widget::my_slot_VKP_destroyed(QObject *)
{
    emit SIG_VKP_Closed(f_vkp->fGetEnterPressed());
    fVKP_Finished(f_vkp->fGetEnterPressed());
}

void HNS_UI_Widget::my_slot_Date_Selected()
{
    fCalendar_Finished(f_calendar->selectedDate());

    fHideCalendar();
}

void HNS_UI_Widget::my_slot_Calendar_Destroyed(QObject *)
{
    fCalendar_Finished(QDate());
}

bool HNS_UI_Widget::fShowScreen(HNS_UI_Widget *screen, const int &x, const int &y)
{
    if(screen != nullptr && f_shown_screen == nullptr)
    {
        fCloseKeyboards();

        f_shown_screen = screen;

        f_shown_screen->show();
        f_shown_screen->move(x,y);
        f_shown_screen->setAutoFillBackground(true);
        f_shown_screen->raise();

        connect(f_shown_screen,SIGNAL(destroyed(QObject*)),this,SLOT(my_Slot_Shown_Screen_Destroyed(QObject*)));
        connect(f_shown_screen,SIGNAL(SIG_GO_HOME()),this,SLOT(my_slot_Go_Home()));
        connect(f_shown_screen,SIGNAL(SIG_Message(QString,HNS_NTCIP_MessageIDCode,type_ui_message_mode)),this,SLOT(my_slot_Message(QString,HNS_NTCIP_MessageIDCode,type_ui_message_mode)));
        connect(f_shown_screen,SIGNAL(SIG_Delete_Request(HNS_NTCIP_MessageIDCode,bool)),this,SLOT(my_slot_Delete_Request(HNS_NTCIP_MessageIDCode,bool)));
        connect(f_shown_screen,SIGNAL(SIG_Schedule(HNS_Schedule,int,bool)),this,SLOT(my_slot_Schedule(HNS_Schedule,int,bool)));
        connect(f_shown_screen,SIGNAL(SIG_Delete_Schedule_Request(int)),this,SLOT(my_slot_Delete_Schedule_Request(int)));
        connect(f_shown_screen,SIGNAL(SIG_Run_PixOut()),this,SLOT(my_slot_Run_PixOut()));
        connect(f_shown_screen,SIGNAL(SIG_Lock_Request()),this,SLOT(my_slot_Lock_Request()));
        connect(f_shown_screen,SIGNAL(SIG_Reset_Request()),this,SLOT(my_slot_Reset_Request()));
        connect(f_shown_screen,SIGNAL(SIG_Update_Request()),this,SLOT(my_slot_Update_Request()));
        connect(f_shown_screen,SIGNAL(SIG_Change_Login_Request(HNS_Security)),this,SLOT(my_slot_Change_Login_Request(HNS_Security)));
        connect(f_shown_screen,SIGNAL(SIG_Trailer_Change_Request(int)),this,SLOT(my_slot_Trailer_Change_Request(int)));
        connect(f_shown_screen,SIGNAL(SIG_PageSettings_Change_Request(type_justification_line,type_justification_page,double,double,int)),
                this,SLOT(my_slot_PageSettings_Change_Request(type_justification_line,type_justification_page,double,double,int)));
        connect(f_shown_screen,SIGNAL(SIG_DateTime_Change_Request(QDate,QTime,QString)),this,SLOT(my_slot_DateTime_Change_Request(QDate,QTime,QString)));
        connect(f_shown_screen,SIGNAL(SIG_Brightness_Change_Request(type_brightness_mode,int)),this,SLOT(my_slot_Brightness_Change_Request(type_brightness_mode,int)));
        connect(f_shown_screen,SIGNAL(SIG_Strobe_Test()),this,SLOT(my_slot_Strobe_Test()));
        connect(f_shown_screen,SIGNAL(SIG_Strobe_Change_Request(HNS_Radar_Strobe)),this,SLOT(my_slot_Strobe_Change_Request(HNS_Radar_Strobe)));
        connect(f_shown_screen,SIGNAL(SIG_Radar_Change_Request(type_radar_mode,type_radar_units,int,HNS_Radar_Police_Speed,QString,QString,QString,QString,QString)),
                this,SLOT(my_slot_Radar_Change_Request(type_radar_mode,type_radar_units,int,HNS_Radar_Police_Speed,QString,QString,QString,QString,QString)));
        connect(f_shown_screen,SIGNAL(SIG_Comm_Change_Request(HNS_IP_Settings)),this,SLOT(my_slot_Comm_Change_Request(HNS_IP_Settings)));
        connect(f_shown_screen,SIGNAL(SIG_GPIO_Change_Request(int,int)),this,SLOT(my_slot_GPIO_Change_Request(int,int)));
        connect(f_shown_screen,SIGNAL(SIG_TM_Custom_Msg_Change_Request(int,HNS_NTCIP_MessageIDCode)),this,SLOT(my_slot_TM_Custom_Msg_Change_Request(int,HNS_NTCIP_MessageIDCode)));
//        connect(f_shown_screen,SIGNAL(SIG_AuxIO_Change_Request(QVector,QVector,QVector)),this,SLOT(my_slot_AuxIO_Change_Request(QVector,QVector,QVector)));
        connect(f_shown_screen,&HNS_UI_Widget::SIG_AuxIO_Change_Request,this,&HNS_UI_Widget::my_slot_AuxIO_Change_Request);

        return true;
    }
    else
    {
        return false;
    }
}

void HNS_UI_Widget::fToggleKB(QObject *obj, const bool &narrow_mode)
{
    if(fIsKBVisible())
    {
        fHideKB();
    }
    else
    {
        fShowKB(obj,narrow_mode);
    }
}

void HNS_UI_Widget::fShowKB(QObject *obj, const bool &narrow_mode)
{
    fShowKB(obj,0,489,narrow_mode);
}

void HNS_UI_Widget::fShowKB(QObject *obj, const int &x, const int &y, const bool &narrow_mode)
{
    fHideKB();

    f_vkb = new HNS_VKB(this);
    f_vkb->setFocusObject(obj);
    f_vkb->show();
    f_vkb->fSetNarrowMode(x,y,narrow_mode);
    f_vkb->raise();
}

void HNS_UI_Widget::fHideKB()
{
    if(fIsKBVisible())
    {
        emit SIG_VKB_Closed();

        delete f_vkb;
        f_vkb = nullptr;
    }
}

void HNS_UI_Widget::fChangeKBFocus(QObject *obj)
{
    if(fIsKBVisible())
    {
        f_vkb->setFocusObject(obj);
    }
}

QObject *HNS_UI_Widget::fGetKBFocus()
{
    if(fIsKBVisible())
    {
        return f_vkb->getFocusObject();
    }
    else
    {
        return nullptr;
    }
}

bool HNS_UI_Widget::fIsKBVisible()
{
    return f_vkb != nullptr;
}

void HNS_UI_Widget::fToggleKP(QObject *obj, const int &x, const int &y, const bool &time_mode)
{
    if(fIsKPVisible())
    {
        fHideKP();
    }
    else
    {
        fShowKP(obj,x,y,time_mode);
    }
}

void HNS_UI_Widget::fShowKP(QObject *obj, const int &x, const int &y, const bool &time_mode)
{
    fHideKP();

    f_vkp = new HNS_VKP(this);
    f_vkp->setFocusObject(obj);
    f_vkp->show();
    f_vkp->move(x,y);
    f_vkp->raise();
    f_vkp->fSetTimeMode(time_mode);

    connect(f_vkp,SIGNAL(closeRequest()),this,SLOT(my_slot_close_VKP()));
    connect(f_vkp,SIGNAL(SIG_AM_PM_Pressed()),this,SLOT(my_slot_AMPM_Toggle()));
    connect(f_vkp,SIGNAL(destroyed(QObject*)),this,SLOT(my_slot_VKP_destroyed(QObject*)));

}

void HNS_UI_Widget::fHideKP()
{
    if(fIsKPVisible())
    {
        emit SIG_VKP_Closed(f_vkp->fGetEnterPressed());

        delete f_vkp;
        f_vkp = nullptr;
    }
}

void HNS_UI_Widget::fChangeKPFocus(QObject *obj)
{
    if(fIsKPVisible())
    {
        f_vkp->setFocusObject(obj);
    }
}

QObject *HNS_UI_Widget::fGetKPFocus()
{
    if(fIsKPVisible())
    {
        return f_vkp->getFocusObject();
    }
    else
    {
        return nullptr;
    }
}

bool HNS_UI_Widget::fIsKPVisible()
{
    return f_vkp != nullptr;
}

void HNS_UI_Widget::fCloseKeyboards()
{
    fHideKB();
    fHideKP();
    fHideCalendar();
}

void HNS_UI_Widget::fShowCalendar(const QDate &date, const int &x, const int &y)
{
    fHideCalendar();

    f_calendar = new QCalendarWidget(this);

    f_calendar->setSelectedDate(date);
    initYearPopup(f_calendar);

    f_calendar->move(x,y);
    f_calendar->resize(570,214);
    f_calendar->show();
    f_calendar->raise();
    f_calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    f_calendar->installEventFilter(this);
    connect(f_calendar,&QCalendarWidget::clicked, this, &HNS_UI_Widget::my_slot_Date_Selected);
    connect(f_calendar,&QCalendarWidget::destroyed, this, &HNS_UI_Widget::my_slot_Calendar_Destroyed);
}

void HNS_UI_Widget::fHideCalendar()
{
    if(fIsCalendarVisible())
    {
        delete f_yearView;
        f_yearView = nullptr;

        delete f_calendar;
        f_calendar = nullptr;
    }
}

bool HNS_UI_Widget::fIsCalendarVisible()
{
    return f_calendar != nullptr;
}

void HNS_UI_Widget::initYearPopup(QCalendarWidget *calendar)
{
    QObjectList myList = calendar->children();
    for(auto obj: myList)
    {
        if(obj->objectName()=="qt_calendar_navigationbar"){
            QObjectList myLists = obj->children();
            QToolButton *qCalToolBtn = obj->findChild<QToolButton*>("qt_calendar_yearbutton");
            QSpinBox *qCalSpinBox = obj->findChild<QSpinBox*>("qt_calendar_yearedit");
            connect(qCalToolBtn, &QToolButton::clicked,this, [=]{
                qCalSpinBox->hide();
                int currentYear = QDate::currentDate().year();
                QStringList list;
                //To display the previous year to next 4 year.
                for (int i = -1; i<=3; i++)
                    list.append(QString::number(currentYear + i));
                QStringListModel *model = new QStringListModel(this);
                model->setStringList(list);
                if(!f_yearView) {
                    f_yearView = new QListView(calendar);
                    f_yearView->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
                    QFont font = f_yearView->font();
                    font.setPointSize(16);
                    f_yearView->setFont(font);
                    f_yearView->setFixedSize(75,100);
                    f_yearView->installEventFilter(this);
                    f_yearView->setModel(model);
                    connect(f_yearView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                            [=](){
                        QModelIndex ind = f_yearView->currentIndex();
                        qCalToolBtn->setText(ind.data(Qt::DisplayRole).toString());
                        QDate tempDate = QDate(ind.data(Qt::DisplayRole).toInt(),calendar->selectedDate().month(), calendar->selectedDate().day());
                        calendar->setSelectedDate(tempDate);
                        f_yearView->hide();
                    });
                }
                f_yearView->move((calendar->x()+306),(calendar->y()+38));
                f_yearView->show();
            });
        }
    }
}
