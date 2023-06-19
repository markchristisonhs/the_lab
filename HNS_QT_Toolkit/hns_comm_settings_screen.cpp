#include "hns_comm_settings_screen.h"
#include "ui_hns_comm_settings_screen.h"

HNS_Comm_Settings_Screen::HNS_Comm_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Comm_Settings_Screen)
{
    ui->setupUi(this);

    ui->LBL_CommSet_CommMode->installEventFilter(this);
    ui->LBL_CommSet_IPMode->installEventFilter(this);

    ui->LE_CommSet_IPAddr->installEventFilter(this);
    ui->LE_CommSet_PortNum->installEventFilter(this);
    ui->LE_CommSet_Gateway->installEventFilter(this);
    ui->LE_CommSet_DNS->installEventFilter(this);
    ui->LE_CommSet_Subnet->installEventFilter(this);
    installEventFilter(this);

    ui->LBL_CommSet_CommMode->hide();
    ui->LBL_CommSet_IPMode->hide();
    ui->LE_CommSet_DNS->hide();
    ui->LE_CommSet_Gateway->hide();
    ui->LE_CommSet_IPAddr->hide();
    ui->LE_CommSet_Subnet->hide();

    ui->LBL_CommSet_CommType_Title->hide();
    ui->LBL_CommSet_DNS_Title->hide();
    ui->LBL_CommSet_Gateway_Title->hide();
    ui->LBL_CommSet_IPAddr_Title->hide();
    ui->LBL_CommSet_IPMode_Title->hide();
    ui->LBL_CommSet_Subnet_Title->hide();

    f_ip_settings = f_machine->fGetIPSettings();

    fUpdateCommInputs();
}

HNS_Comm_Settings_Screen::~HNS_Comm_Settings_Screen()
{
    delete ui;
}

void HNS_Comm_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Comm_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_COMM_SETTINGS;
}

void HNS_Comm_Settings_Screen::fUpdateCommInputs()
{
    switch(f_ip_settings.fGetCommType())
    {
    case HNS_COMM_ETHERNET:
        ui->LBL_CommSet_CommMode->setText("ETHERNET");
        break;
    case HNS_COMM_ETHERNET_SERIAL:
        ui->LBL_CommSet_CommMode->setText("SERIAL AND\nETHERNET");
        break;
    case HNS_COMM_SERIAL:
        ui->LBL_CommSet_CommMode->setText("SERIAL");
        break;
    }

    switch(f_ip_settings.fGetAddressType())
    {
    case HNS_COMM_DYNAMIC:
        ui->LBL_CommSet_IPMode->setText("DYNAMIC");
        break;
    case HNS_COMM_STATIC:
        ui->LBL_CommSet_IPMode->setText("STATIC");
        break;
    }

    ui->LE_CommSet_IPAddr->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetIpAddress())));
    ui->LE_CommSet_Gateway->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetGateway())));
    ui->LE_CommSet_Subnet->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetSubnetMask())));
    ui->LE_CommSet_DNS->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetDNSServer())));
    ui->LE_CommSet_PortNum->setText(QString("%1").arg(f_ip_settings.fGetPort()));
}

void HNS_Comm_Settings_Screen::on_PB_CommSet_Save_clicked()
{
    emit SIG_Comm_Change_Request(f_ip_settings);

    deleteLater();
}


void HNS_Comm_Settings_Screen::on_PB_CommSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Comm_Settings_Screen::on_PB_CommSet_Back_clicked()
{
    deleteLater();
}

void HNS_Comm_Settings_Screen::LBL_CommSet_CommMode_clicked()
{
    if(ui->LBL_CommSet_CommMode->text() == "SERIAL AND\nETHERNET")
    {
        ui->LBL_CommSet_CommMode->setText("SERIAL");
        f_ip_settings.fSetCommType(HNS_COMM_SERIAL);

        ui->LBL_CommSet_IPMode->setEnabled(false);
        ui->LE_CommSet_IPAddr->setEnabled(false);
        ui->LE_CommSet_Gateway->setEnabled(false);
        ui->LE_CommSet_DNS->setEnabled(false);
        ui->LE_CommSet_PortNum->setEnabled(false);
        ui->LE_CommSet_Subnet->setEnabled(false);
    }
    else if(ui->LBL_CommSet_CommMode->text() == "SERIAL")
    {
        ui->LBL_CommSet_CommMode->setText("ETHERNET");
        f_ip_settings.fSetCommType(HNS_COMM_ETHERNET);

        ui->LBL_CommSet_IPMode->setEnabled(true);
        ui->LE_CommSet_IPAddr->setEnabled(true);
        ui->LE_CommSet_Gateway->setEnabled(true);
        ui->LE_CommSet_DNS->setEnabled(true);
        ui->LE_CommSet_PortNum->setEnabled(true);
        ui->LE_CommSet_Subnet->setEnabled(true);
    }
    else
    {
        ui->LBL_CommSet_CommMode->setText("SERIAL AND\nETHERNET");
        f_ip_settings.fSetCommType(HNS_COMM_ETHERNET_SERIAL);

        ui->LBL_CommSet_IPMode->setEnabled(true);
        ui->LE_CommSet_IPAddr->setEnabled(true);
        ui->LE_CommSet_Gateway->setEnabled(true);
        ui->LE_CommSet_DNS->setEnabled(true);
        ui->LE_CommSet_PortNum->setEnabled(true);
        ui->LE_CommSet_Subnet->setEnabled(true);
    }
}

void HNS_Comm_Settings_Screen::LBL_CommSet_IPMode_clicked()
{
    if(ui->LBL_CommSet_IPMode->text() == "STATIC")
    {
        ui->LBL_CommSet_IPMode->setText("DYNAMIC");
        f_ip_settings.fSetAddressType(HNS_COMM_DYNAMIC);
    }
    else
    {
        ui->LBL_CommSet_IPMode->setText("STATIC");
        f_ip_settings.fSetAddressType(HNS_COMM_STATIC);
    }
}

void HNS_Comm_Settings_Screen::LE_CommSet_IpAddr_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_CommSet_IPAddr->setText("");
    }
    fToggleKP(ui->LE_CommSet_IPAddr,370,220);
}

void HNS_Comm_Settings_Screen::LE_CommSet_PortNum_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_CommSet_PortNum->setText("");
    }
    fToggleKP(ui->LE_CommSet_PortNum,640,220);
}

void HNS_Comm_Settings_Screen::LE_CommSet_Gateway_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_CommSet_Gateway->setText("");
    }
    fToggleKP(ui->LE_CommSet_Gateway,280,420);
}

void HNS_Comm_Settings_Screen::LE_CommSet_DNS_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_CommSet_DNS->setText("");
    }
    fToggleKP(ui->LE_CommSet_DNS,90,420);
}

void HNS_Comm_Settings_Screen::LE_CommSet_Subnet_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_CommSet_Subnet->setText("");
    }
    fToggleKP(ui->LE_CommSet_Subnet,370,420);
}

void HNS_Comm_Settings_Screen::LE_CommSet_IpAddr_editingFinished(const bool &enter_pressed)
{
    if(!enter_pressed)
    {
        ui->LE_CommSet_IPAddr->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetIpAddress())));

    }
    else
    {
        f_ip_settings.fSetIpAddress(ui->LE_CommSet_IPAddr->text().toStdString());
        ui->LE_CommSet_IPAddr->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetIpAddress())));
    }
}

void HNS_Comm_Settings_Screen::LE_CommSet_PortNum_editingFinished(const bool &enter_pressed)
{
    if(!enter_pressed)
    {
        ui->LE_CommSet_PortNum->setText(QString("%1").arg(f_ip_settings.fGetPort()));
    }
    else
    {
        f_ip_settings.fSetPort(ui->LE_CommSet_PortNum->text().toInt());
        ui->LE_CommSet_PortNum->setText(QString("%1").arg(f_ip_settings.fGetPort()));
    }
}

void HNS_Comm_Settings_Screen::LE_CommSet_Gateway_editingFinished(const bool &enter_pressed)
{
    if(!enter_pressed)
    {
        ui->LE_CommSet_Gateway->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetGateway())));

    }
    else
    {
        f_ip_settings.fSetGateway(ui->LE_CommSet_Gateway->text().toStdString());
        ui->LE_CommSet_Gateway->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetGateway())));
    }
}

void HNS_Comm_Settings_Screen::LE_CommSet_DNS_editingFinished(const bool &enter_pressed)
{
    if(!enter_pressed)
    {
        ui->LE_CommSet_DNS->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetDNSServer())));

    }
    else
    {
        f_ip_settings.fSetDNSServer(ui->LE_CommSet_DNS->text().toStdString());
        ui->LE_CommSet_DNS->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetDNSServer())));
    }
}

void HNS_Comm_Settings_Screen::LE_CommSet_Subnet_editingFinished(const bool &enter_pressed)
{
    if(!enter_pressed)
    {
        ui->LE_CommSet_Subnet->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetSubnetMask())));

    }
    else
    {
        f_ip_settings.fSetSubnetMask(ui->LE_CommSet_Subnet->text().toStdString());
        ui->LE_CommSet_Subnet->setText(QString::fromStdString(HNS_IP_Settings::fIptoString(f_ip_settings.fGetSubnetMask())));
    }
}

bool HNS_Comm_Settings_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->LBL_CommSet_CommMode)
    {
        LBL_CommSet_CommMode_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LBL_CommSet_IPMode && ui->LBL_CommSet_IPMode->isEnabled())
    {
        LBL_CommSet_IPMode_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LBL_CommSet_CommMode)
    {
        LBL_CommSet_CommMode_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LBL_CommSet_IPMode && ui->LBL_CommSet_IPMode->isEnabled())
    {
        LBL_CommSet_IPMode_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_CommSet_IPAddr && ui->LE_CommSet_IPAddr->isEnabled())
    {
        LE_CommSet_IpAddr_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_CommSet_PortNum && ui->LE_CommSet_PortNum->isEnabled())
    {
        LE_CommSet_PortNum_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_CommSet_Gateway && ui->LE_CommSet_Gateway->isEnabled())
    {
        LE_CommSet_Gateway_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_CommSet_DNS && ui->LE_CommSet_DNS->isEnabled())
    {
        LE_CommSet_DNS_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_CommSet_Subnet && ui->LE_CommSet_Subnet->isEnabled())
    {
        LE_CommSet_Gateway_clicked();
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKP();
        return true;
    }
    return false;
}

void HNS_Comm_Settings_Screen::fVKP_Finished(const bool &enter_pressed)
{
    if(fGetKPFocus() == ui->LE_CommSet_IPAddr)
    {
        LE_CommSet_IpAddr_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_CommSet_PortNum)
    {
        LE_CommSet_PortNum_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_CommSet_Gateway)
    {
        LE_CommSet_Gateway_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_CommSet_DNS)
    {
        LE_CommSet_DNS_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_CommSet_Subnet)
    {
        LE_CommSet_Subnet_editingFinished(enter_pressed);
    }
}

