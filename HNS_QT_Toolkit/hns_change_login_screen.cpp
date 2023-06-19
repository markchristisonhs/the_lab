#include "hns_change_login_screen.h"
#include "ui_hns_change_login_screen.h"

HNS_Change_Login_Screen::HNS_Change_Login_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_Change_Login_Screen)
{
    ui->setupUi(this);

    f_temp_pass = f_machine->fGetLoginInfo();

    ui->LE_ChangeLogin_Pass->installEventFilter(this);
    ui->LE_ChangeLogin_UserName->installEventFilter(this);
    installEventFilter(this);

    ui->LBL_ChangeLogin_PassReset->hide();
}

HNS_Change_Login_Screen::~HNS_Change_Login_Screen()
{
    delete ui;
}

void HNS_Change_Login_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Change_Login_Screen::fGetType() const
{
    return HNS_WIDGET_CHANGE_LOGIN;
}

void HNS_Change_Login_Screen::on_PB_ChangeLogin_Save_clicked()
{
    emit SIG_Change_Login_Request(f_temp_pass);

    deleteLater();
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_Cancel_clicked()
{
    deleteLater();
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_ChUserPass_clicked()
{
    ui->PB_ChangeLogin_ChUserPass->setChecked(true);
    ui->PB_ChangeLogin_ChAdminPass->setChecked(false);
    ui->LE_ChangeLogin_Pass->setText("");
    fCloseKeyboards();
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_ChAdminPass_clicked()
{
    ui->PB_ChangeLogin_ChUserPass->setChecked(false);
    ui->PB_ChangeLogin_ChAdminPass->setChecked(true);
    ui->LE_ChangeLogin_Pass->setText("");
    fCloseKeyboards();
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_ChUserName_clicked()
{
    if(ui->PB_ChangeLogin_ChUserName->text() == "CHANGE")
    {
        ui->PB_ChangeLogin_ChUserName->setText("CONFIRM");
    }
    else if(ui->PB_ChangeLogin_ChUserName->text() == "CONFIRM")
    {
        f_temp_pass.fSetUserName(ui->LE_ChangeLogin_UserName->text());
        ui->LE_ChangeLogin_UserName->setText("");
        ui->PB_ChangeLogin_ChUserName->setText("CHANGE");
    }
    else
    {
        ui->PB_ChangeLogin_ChUserName->setText("CHANGE");
    }
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_ChPass_clicked()
{
    if(ui->PB_ChangeLogin_ChPass->text() == "CHANGE")
    {
        ui->PB_ChangeLogin_ChPass->setText("CONFIRM");
    }
    else if(ui->PB_ChangeLogin_ChPass->text() == "CONFIRM")
    {
        if(ui->PB_ChangeLogin_ChUserPass->isChecked())
        {
            f_temp_pass.fSetUserPass(ui->LE_ChangeLogin_Pass->text());
            ui->LBL_ChangeLogin_PassReset->setText("USER PASS CHANGED");
        }
        else if(ui->PB_ChangeLogin_ChAdminPass->isChecked())
        {
            f_temp_pass.fSetAdminPass(ui->LE_ChangeLogin_Pass->text());
            ui->LBL_ChangeLogin_PassReset->setText("ADMIN PASS CHANGED");
        }
        ui->LBL_ChangeLogin_PassReset->show();
        ui->PB_ChangeLogin_ChPass->setText("CHANGE");
    }
    else
    {
        ui->PB_ChangeLogin_ChPass->setText("CHANGE");
    }
}


void HNS_Change_Login_Screen::on_PB_ChangeLogin_Reset_clicked()
{
    f_temp_pass = f_machine->fGetDefaultLogin();
    ui->LBL_ChangeLogin_PassReset->show();
    ui->LBL_ChangeLogin_PassReset->setText("PASSWORD RESET");
}

bool HNS_Change_Login_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_ChangeLogin_Pass)
    {
        fToggleKB(ui->LE_ChangeLogin_Pass,true);
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_ChangeLogin_UserName)
    {
        fToggleKB(ui->LE_ChangeLogin_UserName,true);
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKB();
        return true;
    }
    else if (obj == ui->LE_ChangeLogin_Pass && event->type() == QEvent::FocusOut)
    {
        fHideKB();
        return true;
    }
    else if (obj == ui->LE_ChangeLogin_UserName && event->type() == QEvent::FocusOut)
    {
        fHideKB();
        return true;
    }

    return false;
}

