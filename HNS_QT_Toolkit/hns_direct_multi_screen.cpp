#include "hns_direct_multi_screen.h"
#include "ui_hns_direct_multi_screen.h"

using namespace std;

HNS_Direct_Multi_Screen::HNS_Direct_Multi_Screen(QWidget *parent, HNS_Machine_Interface *machine, const QString &input) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Direct_Multi_Screen)
  , f_current_multi_error(HNS_MULTI_SYNTAX_ERROR_NONE)
  , f_confirm(false)
  , f_max_num_pages(3)
{
    ui->setupUi(this);

    fShowKB(ui->PTE_Input,0,280,true);
    ui->PTE_Input->appendPlainText(input);
}

HNS_Direct_Multi_Screen::~HNS_Direct_Multi_Screen()
{
    delete ui;
}

type_widget HNS_Direct_Multi_Screen::fGetType() const
{
    return HNS_WIDGET_DIRECT_MULTI;
}

void HNS_Direct_Multi_Screen::fSetAccess(const type_user_level &)
{

}

QString HNS_Direct_Multi_Screen::fGetText() const
{
    return ui->PTE_Input->toPlainText();
}

void HNS_Direct_Multi_Screen::on_PB_OK_clicked()
{
    f_confirm = true;

    emit SIG_Direct_MULTI_Result(ui->PTE_Input->toPlainText());

    deleteLater();
}


void HNS_Direct_Multi_Screen::on_PB_Cancel_clicked()
{
    deleteLater();
}

void HNS_Direct_Multi_Screen::on_PTE_Input_textChanged()
{
    string tempstring = ui->PTE_Input->toPlainText().toStdString();
    HNS_Message2 message;
    HNS_SignBoard signboard;
    size_t num_pages;

    message.fSetMULTI(tempstring,f_machine->fGetFonts(),f_machine->fGetGraphics(),nullptr,&f_current_multi_error,nullptr,f_max_num_pages);
    num_pages = message.fGetNumPages();
    signboard = message.fGetPage(num_pages - 1).fGetSignBoard(0,true);

    ui->GV_Preview->fSetSignBoard(signboard);

    ui->PTE_Errors->clear();
    QString error_string;

    switch(f_current_multi_error)
    {
    case HNS_MULTI_SYNTAX_ERROR_OTHER:
        error_string = "Unkown error detected";
        break;
    case HNS_MULTI_SYNTAX_ERROR_NONE:
        error_string = "String OK";
        break;
    case HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAG:
        error_string = "Unsupported tag";
        break;
    case HNS_MULTI_SYNTAX_ERROR_UNSUPPORTEDTAGVALUE:
        error_string = "Unsupported tag value";
        break;
    case HNS_MULTI_SYNTAX_ERROR_TEXTTOOBIG:
        error_string = "Text too big";
        break;
    case HNS_MULTI_SYNTAX_ERROR_FONTNOTDEFINED:
        error_string = "Unsupported font";
        break;
    case HNS_MULTI_SYNTAX_ERROR_CHARACTERNOTDEFINED:
        error_string = "Unsupported character";
        break;
    case HNS_MULTI_SYNTAX_ERROR_FIELDDEVICENOTEXIST:
        error_string = "Field tag references an invalid device";
        break;
    case HNS_MULTI_SYNTAX_ERROR_FIELDDEVICEERROR:
        error_string = "Field tag error";
        break;
    case HNS_MULTI_SYNTAX_ERROR_FLASHREGIONERROR:
        error_string = "Flash tag error";
        break;
    case HNS_MULTI_SYNTAX_ERROR_TAGCONFLICT:
        error_string = "Tag conflict";
        break;
    case HNS_MULTI_SYNTAX_ERROR_TOOMANYPAGES:
        error_string = "Too many pages";
        break;
    case HNS_MULTI_SYNTAX_ERROR_FONTVERSIONID:
        error_string = "Font version ID incorrect";
        break;
    case HNS_MULTI_SYNTAX_ERROR_GRAPHICID:
        error_string = "Graphic ID error";
        break;
    case HNS_MULTI_SYNTAX_ERROR_GRAPHICNOTDEFINED:
        error_string = "Undefined graphic";
        break;
    default:
        break;
    }

    ui->PB_OK->setEnabled(f_current_multi_error == HNS_MULTI_SYNTAX_ERROR_NONE);

    ui->PTE_Errors->appendPlainText(error_string);
}

