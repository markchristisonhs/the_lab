#include "hns_confirm_screen.h"
#include "ui_hns_confirm_screen.h"

HNS_Confirm_Screen::HNS_Confirm_Screen(QWidget *parent, HNS_Machine_Interface *machine, const QString &title, const QString &confirm_label, const QString &cancel_label) :
    HNS_UI_Widget(parent,machine),
    ui(new Ui::HNS_Confirm_Screen)
{
    ui->setupUi(this);

    ui->LBL_Title->setText(title);
    ui->PB_Confirm->setText(confirm_label);
    ui->PB_Cancel->setText(cancel_label);
}

HNS_Confirm_Screen::~HNS_Confirm_Screen()
{
    delete ui;
}

type_widget HNS_Confirm_Screen::fGetType() const
{
    return HNS_WIDGET_CONFIRM;
}

void HNS_Confirm_Screen::fSetAccess(const type_user_level &)
{

}


void HNS_Confirm_Screen::on_PB_Cancel_clicked()
{
    emit SIG_Confirm_Closed(false);

    deleteLater();
}


void HNS_Confirm_Screen::on_PB_Confirm_clicked()
{
    emit SIG_Confirm_Closed(true);

    deleteLater();
}

