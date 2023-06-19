#include "hns_login_screen.h"
#include "ui_hns_login_screen.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDateTime>

HNS_Login_Screen::HNS_Login_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Login_Screen)
  , f_scene(nullptr)
  , f_first_input(true)
{
    ui->setupUi(this);

    f_scene = new QGraphicsScene(this);
    QImage icon = QImage(":/logo/HSConnectControllerLogo-Blue.png");
    icon = icon.scaled(175,175);
    f_scene->addPixmap(QPixmap::fromImage(icon))->setOffset(0,0);

    ui->GV_Logo->setScene(f_scene);
    ui->GV_Logo->setSceneRect(0,0,175,175);
    f_scene->update();
    ui->GV_Logo->update();

    fShowKB(ui->LE_Username,false);

    ui->LE_Username->installEventFilter(this);
    ui->LE_Password->installEventFilter(this);

    ui->LBL_Debug->hide();
}

HNS_Login_Screen::~HNS_Login_Screen()
{
    delete ui;
}

void HNS_Login_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Login_Screen::fGetType() const
{
    return HNS_WIDGET_LOGIN;
}

void HNS_Login_Screen::fSetSignName(const QString &sign_name)
{
    ui->LBL_Sign->setText(sign_name);
}

void HNS_Login_Screen::fSetVersion(const QString &version)
{
    ui->LBL_Version->setText(version);
}

void HNS_Login_Screen::fSetPowerData(const double &batt_voltage, const double &solar_current, const double &solar_voltage)
{
    ui->LBL_Sol->setText(QString("Sol %1A").arg(solar_current, 0, 'f',1));
    ui->LBL_SolV->setText(QString("Sol %1V").arg(solar_voltage, 0, 'f',1));
    ui->LBL_Batt->setText(QString("Batt %1V").arg(batt_voltage, 0, 'f',1));
}

void HNS_Login_Screen::fSetNTCIP_Waiting(const bool &waiting)
{
    if(waiting)
    {
        ui->LBL_NTCIP_Waiting->show();
        ui->PB_LogIn->setText("WORKING");
        ui->LBL_Version->hide();
    }
    else
    {
        ui->LBL_NTCIP_Waiting->hide();
        ui->PB_LogIn->setText("LOGIN");
        ui->LBL_Version->show();

        if(f_machine->fGetSignCfg().fGetUIMode() == HNS_UI_MODE_TM)
        {
            emit SIG_Login_Request(ui->LE_Username->text(),ui->LE_Password->text());
        }
    }
}

void HNS_Login_Screen::fRejectLogin()
{
    ui->LE_Password->setEchoMode(QLineEdit::Normal);
    ui->LE_Password->setText("Wrong user/pass");
    ui->LE_Username->setFocus();
    ui->LE_Username->setText("");
    fChangeKBFocus(ui->LE_Username);
}

void HNS_Login_Screen::fShowDebug(const QString &text)
{
    ui->LBL_Debug->show();
    ui->LBL_Debug->setText(text);
}

void HNS_Login_Screen::on_PB_LogIn_clicked()
{
    emit SIG_Login_Request(ui->LE_Username->text(),ui->LE_Password->text());
}

bool HNS_Login_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->LE_Username && event->type() == QEvent::FocusIn)
    {
        if(f_first_input)
        {
            ui->LE_Username->setText("");
            f_first_input = false;
        }
        fChangeKBFocus(ui->LE_Username);
    }
    else if(obj == ui->LE_Password && event->type() == QEvent::FocusIn)
    {
        ui->LE_Password->setEchoMode(QLineEdit::Password);
        ui->LE_Password->setText("");
        fChangeKBFocus(ui->LE_Password);
    }
    return false;
}


void HNS_Login_Screen::on_LE_Username_textEdited(const QString &)
{
    QString tempstring = ui->LE_Username->text();
    if(f_first_input)
    {
        //remove "username"
        ui->LE_Username->setText(tempstring.remove(0,8));
        f_first_input = false;
    }
}


void HNS_Login_Screen::on_LE_Username_editingFinished()
{
    ui->LE_Password->setFocus();
    fChangeKBFocus(ui->LE_Password);
    ui->LE_Password->setText("");
    ui->LE_Password->setEchoMode(QLineEdit::Password);
}


void HNS_Login_Screen::on_LE_Password_editingFinished()
{
    on_PB_LogIn_clicked();
}

