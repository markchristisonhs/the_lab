#include "hns_message_library_select_screen.h"
#include "ui_hns_message_library_select_screen.h"

#include "hns_message_library.h"

HNS_Message_Library_Select_Screen::HNS_Message_Library_Select_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Message_Library_Select_Screen)
{
    ui->setupUi(this);
}

HNS_Message_Library_Select_Screen::~HNS_Message_Library_Select_Screen()
{
    delete ui;
}

void HNS_Message_Library_Select_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Message_Library_Select_Screen::fGetType() const
{
    return HNS_WIDGET_MESSAGE_LIBRARY_SELECT;
}

void HNS_Message_Library_Select_Screen::on_PB_Library_Back_clicked()
{
    deleteLater();
}


void HNS_Message_Library_Select_Screen::on_PB_Library_Messages_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);
}


void HNS_Message_Library_Select_Screen::on_PB_Library_StaticAniImages_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_COMBINED);

    fShowScreen(temp_widget);
}

