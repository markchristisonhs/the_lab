#include "hns_diagnostic_select_screen.h"
#include "ui_hns_diagnostic_select_screen.h"

#include "hns_pixout_screen.h"
#include "hns_test_pattern_screen.h"

HNS_Diagnostic_Select_Screen::HNS_Diagnostic_Select_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Diagnostic_Select_Screen)
{
    ui->setupUi(this);
}

HNS_Diagnostic_Select_Screen::~HNS_Diagnostic_Select_Screen()
{
    delete ui;
}

void HNS_Diagnostic_Select_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Diagnostic_Select_Screen::fGetType() const
{
    return HNS_WIDGET_DIAGNOSTIC_SELECT;
}

void HNS_Diagnostic_Select_Screen::on_PB_Diagnostic_Select_Cancel_clicked()
{
    deleteLater();
}


void HNS_Diagnostic_Select_Screen::on_PB_Diagnostic_Select_PixOut_clicked()
{
    fShowScreen(new HNS_PixOut_Screen(this, f_machine));
}


void HNS_Diagnostic_Select_Screen::on_PB_Diagnostic_Select_TestPatterns_clicked()
{
    fShowScreen(new HNS_Test_Pattern_Screen(this, f_machine));
}

