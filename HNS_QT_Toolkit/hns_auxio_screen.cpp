#include "hns_auxio_screen.h"
#include "ui_hns_auxio_screen.h"

#include "hns_message_library.h"

HNS_AuxIO_Screen::HNS_AuxIO_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_AuxIO_Screen)
  , f_index(-1)
{
    ui->setupUi(this);

    f_msgs_multi = f_machine->fGetAUXIOMessagesMULTI();
    f_msgs_multi.resize(4);
    f_msgs = f_machine->fGetAuxIOMessages();
    f_msgs.resize(4);
    f_enables = f_machine->fGetAUXIOEnables();
    f_enables.resize(4);
    f_durations = f_machine->fGetAUXIODurations();
    f_durations.resize(4);

    ui->GV_Aux1->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Aux1->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Aux2->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Aux2->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Aux3->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Aux3->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Aux4->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Aux4->fSetGraphics(*f_machine->fGetGraphics());

    ui->LE_Duration1->installEventFilter(this);
    ui->LE_Duration2->installEventFilter(this);
    ui->LE_Duration3->installEventFilter(this);
    ui->LE_Duration4->installEventFilter(this);

    fUpdate();
}

HNS_AuxIO_Screen::~HNS_AuxIO_Screen()
{
    delete ui;
}

void HNS_AuxIO_Screen::my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &)
{
    f_msgs[f_index] = id_code;
    f_msgs_multi[f_index] = multi.toStdString();

    f_index = -1;

    fUpdate();
}

void HNS_AuxIO_Screen::fUpdate(const int &update_flags)
{
    if(update_flags & HNS_DATA_NEW_AUXIO)
    {
        ui->GV_Aux1->fSetMessage(QString::fromStdString(f_msgs_multi[0]));
        ui->GV_Aux2->fSetMessage(QString::fromStdString(f_msgs_multi[1]));
        ui->GV_Aux3->fSetMessage(QString::fromStdString(f_msgs_multi[2]));
        ui->GV_Aux4->fSetMessage(QString::fromStdString(f_msgs_multi[3]));

        ui->PB_Enable1->setChecked(f_enables[0]);
        ui->PB_Enable2->setChecked(f_enables[1]);
        ui->PB_Enable3->setChecked(f_enables[2]);
        ui->PB_Enable4->setChecked(f_enables[3]);

        ui->LE_Duration1->setText(QString::number(f_durations[0]));
        ui->LE_Duration2->setText(QString::number(f_durations[1]));
        ui->LE_Duration3->setText(QString::number(f_durations[2]));
        ui->LE_Duration4->setText(QString::number(f_durations[3]));
    }
}

bool HNS_AuxIO_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Duration1)
    {
        LE_Duration1_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Duration2)
    {
        LE_Duration2_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Duration3)
    {
        LE_Duration3_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Duration4)
    {
        LE_Duration4_clicked();
        return true;
    }
    return false;
}

void HNS_AuxIO_Screen::on_PB_Set_Msg1_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 0;
}


void HNS_AuxIO_Screen::on_PB_Enable1_clicked()
{
    f_enables[0] = ui->PB_Enable1->isChecked();
    fUpdate();
}


void HNS_AuxIO_Screen::on_PB_Set_Msg2_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 1;
}


void HNS_AuxIO_Screen::on_PB_Enable2_clicked()
{
    f_enables[1] = ui->PB_Enable2->isChecked();
    fUpdate();
}


void HNS_AuxIO_Screen::on_PB_Set_Msg3_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 2;
}


void HNS_AuxIO_Screen::on_PB_Enable3_clicked()
{
    f_enables[2] = ui->PB_Enable3->isChecked();
    fUpdate();
}


void HNS_AuxIO_Screen::on_PB_Set_Msg4_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);

    f_index = 3;
}


void HNS_AuxIO_Screen::on_PB_Enable4_clicked()
{
    f_enables[3] = ui->PB_Enable4->isChecked();
    fUpdate();
}


void HNS_AuxIO_Screen::on_PB_Save_clicked()
{
    QVector<HNS_NTCIP_MessageIDCode> temp_msgs = QVector<HNS_NTCIP_MessageIDCode>::fromStdVector(f_msgs);
    QVector<int> temp_durations = QVector<int>::fromStdVector(f_durations);
    QVector<unsigned char> temp_enables = QVector<unsigned char>::fromStdVector(f_enables);

    emit SIG_AuxIO_Change_Request(temp_msgs,temp_durations,temp_enables);

    deleteLater();
}


void HNS_AuxIO_Screen::on_PB_Back_clicked()
{
    deleteLater();
}

void HNS_AuxIO_Screen::LE_Duration1_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Duration1->clear();
        fShowKP(ui->LE_Duration1,200,340);
    }
    else
    {
        //already open.  Do I close it or transfer control to this control?
        if(fGetKPFocus() == ui->LE_Duration1)
        {
            LE_Duration1_editingFinished(false);
            fHideKP();
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Duration2)
            {
                LE_Duration2_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration3)
            {
                LE_Duration3_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration4)
            {
                LE_Duration4_editingFinished(false);
            }
            fCloseKeyboards();
            fShowKP(ui->LE_Duration1,200,340);
            ui->LE_Duration1->clear();
        }
    }
}

void HNS_AuxIO_Screen::LE_Duration2_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Duration2->clear();
        fShowKP(ui->LE_Duration2,620,340);
    }
    else
    {
        //already open.  Do I close it or transfer control to this control?
        if(fGetKPFocus() == ui->LE_Duration2)
        {
            LE_Duration2_editingFinished(false);
            fHideKP();
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Duration1)
            {
                LE_Duration1_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration3)
            {
                LE_Duration3_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration4)
            {
                LE_Duration4_editingFinished(false);
            }
            fCloseKeyboards();
            fShowKP(ui->LE_Duration2,620,340);
            ui->LE_Duration2->clear();
        }
    }
}

void HNS_AuxIO_Screen::LE_Duration3_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Duration3->clear();
        fShowKP(ui->LE_Duration3,30,450);
    }
    else
    {
        //already open.  Do I close it or transfer control to this control?
        if(fGetKPFocus() == ui->LE_Duration3)
        {
            LE_Duration3_editingFinished(false);
            fHideKP();
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Duration1)
            {
                LE_Duration1_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration2)
            {
                LE_Duration2_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration4)
            {
                LE_Duration4_editingFinished(false);
            }
            fCloseKeyboards();
            fShowKP(ui->LE_Duration3,30,450);
            ui->LE_Duration3->clear();
        }
    }
}

void HNS_AuxIO_Screen::LE_Duration4_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Duration4->clear();
        fShowKP(ui->LE_Duration4,450,450);
    }
    else
    {
        //already open.  Do I close it or transfer control to this control?
        if(fGetKPFocus() == ui->LE_Duration4)
        {
            LE_Duration4_editingFinished(false);
            fHideKP();
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Duration1)
            {
                LE_Duration1_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration2)
            {
                LE_Duration2_editingFinished(false);
            }
            else if(fGetKPFocus() == ui->LE_Duration3)
            {
                LE_Duration3_editingFinished(false);
            }
            fCloseKeyboards();
            fShowKP(ui->LE_Duration3,450,450);
            ui->LE_Duration4->clear();
        }
    }
}

void HNS_AuxIO_Screen::LE_Duration1_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(!(ui->LE_Duration1->text().isEmpty() || !enter_pressed))
    {
        bool ok = true;
        int temp;

        tempstring = ui->LE_Duration1->text();
        temp = tempstring.toInt(&ok);

        if(ok)
        {
            f_durations[0] = temp;
        }
    }
    fUpdate();
}

void HNS_AuxIO_Screen::LE_Duration2_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(!(ui->LE_Duration2->text().isEmpty() || !enter_pressed))
    {
        bool ok = true;
        int temp;

        tempstring = ui->LE_Duration2->text();
        temp = tempstring.toInt(&ok);

        if(ok)
        {
            f_durations[1] = temp;
        }
    }
    fUpdate();
}

void HNS_AuxIO_Screen::LE_Duration3_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(!(ui->LE_Duration3->text().isEmpty() || !enter_pressed))
    {
        bool ok = true;
        int temp;

        tempstring = ui->LE_Duration3->text();
        temp = tempstring.toInt(&ok);

        if(ok)
        {
            f_durations[2] = temp;
        }
    }
    fUpdate();
}

void HNS_AuxIO_Screen::LE_Duration4_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(!(ui->LE_Duration4->text().isEmpty() || !enter_pressed))
    {
        bool ok = true;
        int temp;

        tempstring = ui->LE_Duration4->text();
        temp = tempstring.toInt(&ok);

        if(ok)
        {
            f_durations[3] = temp;
        }
    }
    fUpdate();
}

void HNS_AuxIO_Screen::fVKP_Finished(const bool &enter_pressed)
{
    if(fGetKPFocus() == ui->LE_Duration1)
    {
        LE_Duration1_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_Duration2)
    {
        LE_Duration2_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_Duration3)
    {
        LE_Duration3_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_Duration4)
    {
        LE_Duration4_editingFinished(enter_pressed);
    }
}
