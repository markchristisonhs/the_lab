#include "hns_message_library.h"
#include "ui_hns_message_library.h"

#include <hns_qt_toolkit.h>
#include <QMouseEvent>
#include <HNS_NTCIP_Datatypes.h>
#include <HNS_NTCIP_Enums.h>

#include <hns_confirm_screen.h>

using namespace std;

HNS_Message_Library::HNS_Message_Library(QWidget *parent, HNS_Machine_Interface *machine, const type_MsgLib_Mode &lib_mode) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Message_Library)
  , f_msglib_mode(lib_mode)
  , f_messagelibrary_page(0)
  , f_factoryChecked(true)
  , f_customChecked(true)
  , f_selectedCustMsgIndex(-1)
  , f_startPos(0,0)
  , f_endPos(0, 0)
{
    ui->setupUi(this);

    ui->GV_MessageLibrary_Preview1->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview1->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview2->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview2->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview3->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview3->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview4->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview4->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview5->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview5->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview6->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview6->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview7->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview7->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview8->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview8->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_MessageLibrary_Preview9->fSetFonts(*f_machine->fGetFonts());
    ui->GV_MessageLibrary_Preview9->fSetGraphics(*f_machine->fGetGraphics());

    ui->LE_Search->installEventFilter(this);
    installEventFilter(this);

    connect(ui->GV_MessageLibrary_Preview1,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview1_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview2,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview2_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview3,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview3_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview4,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview4_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview5,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview5_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview6,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview6_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview7,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview7_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview8,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview8_clicked(QMouseEvent *)));
    connect(ui->GV_MessageLibrary_Preview9,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_MessageLibrary_Preview9_clicked(QMouseEvent *)));

    fSetMsgLibMode(lib_mode);
    fSetLibrarySelection(1);
    fSelectPreview(1);
    fUpdate();
}

HNS_Message_Library::~HNS_Message_Library()
{
    delete ui;
}

void HNS_Message_Library::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Message_Library::fGetType() const
{
    return HNS_WIDGET_MESSAGE_LIBRARY;
}

void HNS_Message_Library::on_PB_MsgLib_Back_clicked()
{
    deleteLater();
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview1_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview1->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview1->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview1->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview1->fIsSelected())
    {
        fSelectPreview(1);

        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(1);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview2_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview2->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview2->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview2->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview2->fIsSelected())
    {
        fSelectPreview(2);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(2);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview3_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview3->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview3->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview3->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview3->fIsSelected())
    {
        fSelectPreview(3);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(3);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview4_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview4->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview4->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview4->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview4->fIsSelected())
    {
        fSelectPreview(4);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(4);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview5_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview5->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview5->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview5->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview5->fIsSelected())
    {
        fSelectPreview(5);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(5);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview6_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview6->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview6->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview6->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview6->fIsSelected())
    {
        fSelectPreview(6);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(6);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview7_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview7->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview7->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview7->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview7->fIsSelected())
    {
        fSelectPreview(7);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(7);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview8_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview8->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview8->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview8->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview8->fIsSelected())
    {
        fSelectPreview(8);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview9->fSelectPage(0);
        fSetLibrarySelection(8);
    }
}

void HNS_Message_Library::my_slot_GV_MessageLibrary_Preview9_clicked(QMouseEvent *event)
{
    HNS_Message2 temp_message;
    temp_message = ui->GV_MessageLibrary_Preview9->fGetCurrentMessage();
    f_selectedCustMsgIndex = f_machine->fGetChangableIndex(temp_message);
    f_selectedCustMsgIndex == -1 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();

    if(event != nullptr)
    {
        f_startPos.setX(ui->GV_MessageLibrary_Preview9->x()  + event->pos().x());
        f_startPos.setY(ui->GV_MessageLibrary_Preview9->y() + event->pos().y());
    }

    if(!ui->GV_MessageLibrary_Preview9->fIsSelected())
    {
        fSelectPreview(9);

        ui->GV_MessageLibrary_Preview1->fSelectPage(0);
        ui->GV_MessageLibrary_Preview2->fSelectPage(0);
        ui->GV_MessageLibrary_Preview3->fSelectPage(0);
        ui->GV_MessageLibrary_Preview4->fSelectPage(0);
        ui->GV_MessageLibrary_Preview5->fSelectPage(0);
        ui->GV_MessageLibrary_Preview6->fSelectPage(0);
        ui->GV_MessageLibrary_Preview7->fSelectPage(0);
        ui->GV_MessageLibrary_Preview8->fSelectPage(0);
        fSetLibrarySelection(9);
    }
}

void HNS_Message_Library::fSetLibrarySelection(const int &select)
{
    ui->GV_MessageLibrary_Preview1->fSelect(select == 1);
    ui->GV_MessageLibrary_Preview2->fSelect(select == 2);
    ui->GV_MessageLibrary_Preview3->fSelect(select == 3);
    ui->GV_MessageLibrary_Preview4->fSelect(select == 4);
    ui->GV_MessageLibrary_Preview5->fSelect(select == 5);
    ui->GV_MessageLibrary_Preview6->fSelect(select == 6);
    ui->GV_MessageLibrary_Preview7->fSelect(select == 7);
    ui->GV_MessageLibrary_Preview8->fSelect(select == 8);
    ui->GV_MessageLibrary_Preview9->fSelect(select == 9);
}

int HNS_Message_Library::fGetLibrarySelection(HNS_Message2 *message, HNS_NTCIP_MessageActivationCode *act_code)
{
    if(ui->GV_MessageLibrary_Preview1->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview1->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview1->fGetActivationCode();
        }
        return 1;
    }
    else if(ui->GV_MessageLibrary_Preview2->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview2->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview2->fGetActivationCode();
        }
        return 2;
    }
    else if(ui->GV_MessageLibrary_Preview3->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview3->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview3->fGetActivationCode();
        }
        return 3;
    }
    else if(ui->GV_MessageLibrary_Preview4->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview4->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview4->fGetActivationCode();
        }
        return 4;
    }
    else if(ui->GV_MessageLibrary_Preview5->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview5->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview5->fGetActivationCode();
        }
        return 5;
    }
    else if(ui->GV_MessageLibrary_Preview6->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview6->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview6->fGetActivationCode();
        }
        return 6;
    }
    else if(ui->GV_MessageLibrary_Preview7->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview7->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview7->fGetActivationCode();
        }
        return 7;
    }
    else if(ui->GV_MessageLibrary_Preview8->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview8->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview8->fGetActivationCode();
        }
        return 8;
    }
    else if(ui->GV_MessageLibrary_Preview9->fIsSelected())
    {
        if(message != nullptr)
        {
            *message = ui->GV_MessageLibrary_Preview9->fGetCurrentMessage();
        }
        if(act_code != nullptr)
        {
            *act_code = ui->GV_MessageLibrary_Preview9->fGetActivationCode();
        }
        return 9;
    }
    else
    {
        return 0;
    }
}

void HNS_Message_Library::fSetMsgLibMode(const type_MsgLib_Mode &mode)
{
    f_msglib_mode = mode;

    switch(mode)
    {
    case HNS_MSGLIB_MESSAGES:
        ui->LBL_MsgLib_Title->setText("SAVED MESSAGE LIBRARY");
        ui->PB_MsgLib_Pick1->setText("VIEW ANIMATED\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick2->setText("VIEW STATIC\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick3->setText("VIEW COMBINED\nLIBRARY");
        if(HNS_Message2::fGetDefaultSignBoardInfo().fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
        {
            ui->PB_MsgLib_Pick2->setText("VIEW MESSAGE\nLIBRARY");
            ui->PB_MsgLib_Pick1->hide();
            ui->PB_MsgLib_Pick3->hide();
        }
        else {
            ui->PB_MsgLib_Pick2->setText("VIEW STATIC\nIMAGE LIBRARY");
            ui->PB_MsgLib_Pick1->show();
            ui->PB_MsgLib_Pick3->show();
        }
        break;
    case HNS_MSGLIB_STATIC:
        ui->LBL_MsgLib_Title->setText("SAVED STATIC IMAGE LIBRARY");
        ui->PB_MsgLib_Pick1->setText("VIEW ANIMATED\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick2->setText("VIEW MESSAGE\nLIBRARY");
        ui->PB_MsgLib_Pick3->setText("VIEW COMBINED\nLIBRARY");
        ui->PB_MsgLib_Pick3->show();
        break;
    case HNS_MSGLIB_ANIMATED:
        ui->LBL_MsgLib_Title->setText("SAVED ANIMATED IMAGE LIBRARY");
        ui->PB_MsgLib_Pick1->setText("VIEW STATIC\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick2->setText("VIEW MESSAGE\nLIBRARY");
        ui->PB_MsgLib_Pick3->setText("VIEW COMBINED\nLIBRARY");
        ui->PB_MsgLib_Pick3->show();
        break;
    case HNS_MSGLIB_COMBINED:
        ui->LBL_MsgLib_Title->setText("SAVED COMBINED IMAGE LIBRARY");
        ui->PB_MsgLib_Pick1->setText("VIEW ANIMATED\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick2->setText("VIEW STATIC\nIMAGE LIBRARY");
        ui->PB_MsgLib_Pick3->setText("VIEW MESSAGE\nLIBRARY");
        ui->PB_MsgLib_Pick3->show();
    }
}

void HNS_Message_Library::fUpdate(const int &update_flags)
{
    if(update_flags & HNS_DATA_MESSAGE_CHANGE)
    {
        HNS_Message2 temp_message(HNS_Message2::fGetDefaultSignBoardInfo());
        HNS_NTCIP_MessageActivationCode temp_act_code;
        QString tempstring;
        ui->PB_MsgLib_Custom->setChecked(f_customChecked);
        ui->PB_MsgLib_Fact->setChecked(f_factoryChecked);
        int start = f_messagelibrary_page * 9;
        int end = start + 9;

        if(f_messagelibrary_page == 0)
        {
            ui->PB_MsgLib_Prev->hide();
        }

        if(f_msglib_mode == HNS_MSGLIB_MESSAGES)
        {
            ui->LE_Search->show();
            ui->PB_MsgLib_Fact->show();

            f_machine->fGetNumMessages(NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE, "") <= 0 ?
                        ui->PB_MsgLib_Custom->hide():
                        ui->PB_MsgLib_Custom->show();
            int num_changeable_messages = f_customChecked ? f_machine->fGetNumMessages(NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE, f_searchString) : 0;
            int num_permanent_messages = f_factoryChecked ? f_machine->fGetNumMessages(NTCIP_MESSAGE::MEM_TYPE_PERMANENT, f_searchString) : 0;

            if((num_changeable_messages + num_permanent_messages)<=9)
            {
                f_messagelibrary_page = 0;
                ui->PB_MsgLib_Prev->hide();
                ui->PB_MsgLib_Next->hide();
            }
            else if(((f_messagelibrary_page + 1) * 9) < num_changeable_messages + num_permanent_messages)
            {
                ui->PB_MsgLib_Next->show();
            }
            for (int i=start;i<end;i++)
            {
                if(i < num_changeable_messages)
                {
                    temp_message = f_machine->fGetMessage2(NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE,i, f_searchString, &temp_act_code, HNS_MESSAGE_SORT_DATE_NEWEST_FIRST);
                }
                else if((i-num_changeable_messages) < num_permanent_messages)
                {
                    temp_message = f_machine->fGetMessage2(NTCIP_MESSAGE::MEM_TYPE_PERMANENT,(i-num_changeable_messages), f_searchString, &temp_act_code);
                    tempstring = QString::fromStdString(temp_message.fGetMULTI());
                }
                else
                {
                    temp_message = HNS_Message2(HNS_Message2::fGetDefaultSignBoardInfo());
                    temp_act_code = HNS_NTCIP_MessageActivationCode();
                }

                tempstring = QString::fromStdString(temp_message.fGetMULTI());

                switch(i%9)
                {
                case 0:
                    ui->GV_MessageLibrary_Preview1->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview1->fSetActivationCode(temp_act_code);
                    break;
                case 1:
                    ui->GV_MessageLibrary_Preview2->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview2->fSetActivationCode(temp_act_code);
                    break;
                case 2:
                    ui->GV_MessageLibrary_Preview3->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview3->fSetActivationCode(temp_act_code);
                    break;
                case 3:
                    ui->GV_MessageLibrary_Preview4->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview4->fSetActivationCode(temp_act_code);
                    break;
                case 4:
                    ui->GV_MessageLibrary_Preview5->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview5->fSetActivationCode(temp_act_code);
                    break;
                case 5:
                    ui->GV_MessageLibrary_Preview6->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview6->fSetActivationCode(temp_act_code);
                    break;
                case 6:
                    ui->GV_MessageLibrary_Preview7->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview7->fSetActivationCode(temp_act_code);
                    break;
                case 7:
                    ui->GV_MessageLibrary_Preview8->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview8->fSetActivationCode(temp_act_code);
                    break;
                case 8:
                    ui->GV_MessageLibrary_Preview9->fSetMessage(temp_message);
                    ui->GV_MessageLibrary_Preview9->fSetActivationCode(temp_act_code);
                    break;
                }
            }

            if(f_machine->fGetAccess() < HNS_VIEWER)
            {
                num_changeable_messages<=0 ? ui->PB_MsgLib_Del->hide() : ui->PB_MsgLib_Del->show();
                num_changeable_messages<=0 ? ui->PB_MsgLib_Del_All->hide() : ui->PB_MsgLib_Del_All->show();

                fGetLibrarySelection(nullptr,&temp_act_code);
                if(temp_act_code.fGetMemoryType() == NTCIP_MESSAGE::MEM_TYPE_PERMANENT)
                {
                    ui->PB_MsgLib_Del->hide();
                }
            }
            else
            {
                ui->PB_MsgLib_Del->hide();
                ui->PB_MsgLib_Del_All->hide();
            }
        }
        else if(f_msglib_mode == HNS_MSGLIB_STATIC)
        {
            ui->LE_Search->hide();
            ui->PB_MsgLib_Del->hide();
            ui->PB_MsgLib_Custom->hide();
            ui->PB_MsgLib_Fact->hide();
            ui->PB_MsgLib_Next->show();
            int num_graphics = f_machine->fGetGraphics()->size();
            QString multi;
            HNS_Graphic temp_graphic;

            for(int i=start;i<end;i++)
            {
                if(i < num_graphics)
                {
                    temp_graphic = f_machine->fGetGraphicByIndex(i);
                    multi = QString("[g%1]").arg(temp_graphic.fGetGraphicNumber());
                    temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,temp_message.fGetMULTI(),"127.0.0.1");
                }
                else
                {
                    multi = "";
                    temp_act_code = HNS_NTCIP_MessageActivationCode();
                }

                switch(i%9)
                {
                case 0:
                    ui->GV_MessageLibrary_Preview1->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview1->fSetActivationCode(temp_act_code);
                    break;
                case 1:
                    ui->GV_MessageLibrary_Preview2->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview2->fSetActivationCode(temp_act_code);
                    break;
                case 2:
                    ui->GV_MessageLibrary_Preview3->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview3->fSetActivationCode(temp_act_code);
                    break;
                case 3:
                    ui->GV_MessageLibrary_Preview4->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview4->fSetActivationCode(temp_act_code);
                    break;
                case 4:
                    ui->GV_MessageLibrary_Preview5->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview5->fSetActivationCode(temp_act_code);
                    break;
                case 5:
                    ui->GV_MessageLibrary_Preview6->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview6->fSetActivationCode(temp_act_code);
                    break;
                case 6:
                    ui->GV_MessageLibrary_Preview7->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview7->fSetActivationCode(temp_act_code);
                    break;
                case 7:
                    ui->GV_MessageLibrary_Preview8->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview8->fSetActivationCode(temp_act_code);
                    break;
                case 8:
                    ui->GV_MessageLibrary_Preview9->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview9->fSetActivationCode(temp_act_code);
                    break;
                }
            }
        }
        else if(f_msglib_mode == HNS_MSGLIB_ANIMATED)
        {
            ui->LE_Search->hide();
            ui->PB_MsgLib_Del->hide();
            ui->PB_MsgLib_Custom->hide();
            ui->PB_MsgLib_Fact->hide();
            ui->PB_MsgLib_Next->hide();
            int num_animations = f_machine->fGetAnimations()->size();
            vector <int> animation;
            QString multi;

            for(int i=start;i<end;i++)
            {
                if(i<num_animations)
                {
                    animation = f_machine->fGetAnimations()->at(i);
                    multi = AnimationToMulti(animation,1000);
                    temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,multi.toStdString(),"127.0.0.1");
                }
                else
                {
                    multi = "";
                    temp_act_code = HNS_NTCIP_MessageActivationCode();
                }

                switch(i%9)
                {
                case 0:
                    ui->GV_MessageLibrary_Preview1->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview1->fSetActivationCode(temp_act_code);
                    break;
                case 1:
                    ui->GV_MessageLibrary_Preview2->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview2->fSetActivationCode(temp_act_code);
                    break;
                case 2:
                    ui->GV_MessageLibrary_Preview3->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview3->fSetActivationCode(temp_act_code);
                    break;
                case 3:
                    ui->GV_MessageLibrary_Preview4->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview4->fSetActivationCode(temp_act_code);
                    break;
                case 4:
                    ui->GV_MessageLibrary_Preview5->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview5->fSetActivationCode(temp_act_code);
                    break;
                case 5:
                    ui->GV_MessageLibrary_Preview6->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview6->fSetActivationCode(temp_act_code);
                    break;
                case 6:
                    ui->GV_MessageLibrary_Preview7->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview7->fSetActivationCode(temp_act_code);
                    break;
                case 7:
                    ui->GV_MessageLibrary_Preview8->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview8->fSetActivationCode(temp_act_code);
                    break;
                case 8:
                    ui->GV_MessageLibrary_Preview9->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview9->fSetActivationCode(temp_act_code);
                    break;
                }
            }
        }
        else if(f_msglib_mode == HNS_MSGLIB_COMBINED)
        {
            ui->LE_Search->hide();
            ui->PB_MsgLib_Del->hide();
            ui->PB_MsgLib_Custom->hide();
            ui->PB_MsgLib_Fact->hide();
            ui->PB_MsgLib_Next->show();
            int num_static = f_machine->fGetGraphics()->size();
            int num_ani = num_static + f_machine->fGetAnimations()->size();
            QString multi;
            vector <int> animation;
            HNS_Graphic temp_graphic;

            for(int i=start;i<end;i++)
            {
                if(i < num_static)
                {
                    temp_graphic = f_machine->fGetGraphicByIndex(i);
                    multi = QString("[g%1]").arg(temp_graphic.fGetGraphicNumber());
                    temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,multi.toStdString(),"127.0.0.1");
                }
                else if(i < num_ani)
                {
                    animation = f_machine->fGetAnimations()->at(i - num_static);
                    multi = AnimationToMulti(animation,1000);
                    temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,multi.toStdString(),"127.0.0.1");
                }
                else
                {
                    multi = "";
                    temp_act_code = HNS_NTCIP_MessageActivationCode();
                }

                switch(i%9)
                {
                case 0:
                    ui->GV_MessageLibrary_Preview1->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview1->fSetActivationCode(temp_act_code);
                    break;
                case 1:
                    ui->GV_MessageLibrary_Preview2->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview2->fSetActivationCode(temp_act_code);
                    break;
                case 2:
                    ui->GV_MessageLibrary_Preview3->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview3->fSetActivationCode(temp_act_code);
                    break;
                case 3:
                    ui->GV_MessageLibrary_Preview4->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview4->fSetActivationCode(temp_act_code);
                    break;
                case 4:
                    ui->GV_MessageLibrary_Preview5->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview5->fSetActivationCode(temp_act_code);
                    break;
                case 5:
                    ui->GV_MessageLibrary_Preview6->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview6->fSetActivationCode(temp_act_code);
                    break;
                case 6:
                    ui->GV_MessageLibrary_Preview7->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview7->fSetActivationCode(temp_act_code);
                    break;
                case 7:
                    ui->GV_MessageLibrary_Preview8->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview8->fSetActivationCode(temp_act_code);
                    break;
                case 8:
                    ui->GV_MessageLibrary_Preview9->fSetMessage(multi);
                    ui->GV_MessageLibrary_Preview9->fSetActivationCode(temp_act_code);
                    break;
                }
            }
        }

        if(f_machine->fGetAccess() < HNS_VIEWER)
        {
            ui->PB_MsgLib_InsSelected->show();
        }
        else
        {
            ui->PB_MsgLib_InsSelected->hide();
        }
    }

    HNS_UI_Widget::fUpdate(update_flags);
}


void HNS_Message_Library::on_PB_MsgLib_Prev_clicked()
{
    if(f_messagelibrary_page > 0)
    {
        f_messagelibrary_page--;
        fUpdate();
        fSetLibrarySelection(1);
    }
    if(f_msglib_mode != HNS_MSGLIB_ANIMATED) {
        if(f_messagelibrary_page == 0)
        {
            ui->PB_MsgLib_Prev->hide();
        }
        ui->PB_MsgLib_Next->show();
    }
}


void HNS_Message_Library::on_PB_MsgLib_Next_clicked()
{
    if(f_msglib_mode == HNS_MSGLIB_MESSAGES)
    {
        int num_changeable_messages = f_customChecked ? f_machine->fGetNumMessages(NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE, f_searchString) : 0;
        int num_permanent_messages = f_factoryChecked ? f_machine->fGetNumMessages(NTCIP_MESSAGE::MEM_TYPE_PERMANENT, f_searchString) : 0;
        if(((f_messagelibrary_page + 1) * 9) < num_changeable_messages + num_permanent_messages)
        {
            f_messagelibrary_page++;
            fUpdate();
            fSetLibrarySelection(1);
        } else {
            ui->PB_MsgLib_Next->hide();
        }
        ui->PB_MsgLib_Prev->show();
    }
    else if(f_msglib_mode == HNS_MSGLIB_STATIC)
    {
        int num_graphics = f_machine->fGetGraphics()->size();

        if(((f_messagelibrary_page + 1) * 9) < num_graphics)
        {
            f_messagelibrary_page++;
            fUpdate();
            fSetLibrarySelection(1);
        } else {
            ui->PB_MsgLib_Next->hide();
        }
        ui->PB_MsgLib_Prev->show();
    }
    else if(f_msglib_mode == HNS_MSGLIB_ANIMATED)
    {
        int num_animations = f_machine->fGetAnimations()->size();

        if(((f_messagelibrary_page + 1) * 9) < num_animations)
        {
            f_messagelibrary_page++;
            fUpdate();
            fSetLibrarySelection(1);
        }
        ui->PB_MsgLib_Next->hide();
        ui->PB_MsgLib_Prev->hide();
    }
    else if(f_msglib_mode == HNS_MSGLIB_COMBINED)
    {
        int num_msg = f_machine->fGetGraphics()->size() + f_machine->fGetAnimations()->size();
        if(((f_messagelibrary_page + 1) * 9) < num_msg)
        {
            f_messagelibrary_page++;
            fUpdate();
            fSetLibrarySelection(1);
        } else {
            ui->PB_MsgLib_Next->hide();
        }
        ui->PB_MsgLib_Prev->show();
    }
}


void HNS_Message_Library::on_PB_MsgLib_InsSelected_clicked()
{
    HNS_Message2 temp_message(HNS_Message2::fGetDefaultSignBoardInfo());
    HNS_NTCIP_MessageActivationCode temp_act_code;
    QString tempstring;

    fGetLibrarySelection(&temp_message,&temp_act_code);
    tempstring = QString::fromStdString(temp_message.fGetMULTI());

    HNS_NTCIP_MessageIDCode id_code;

    id_code.fSetCRC(temp_act_code.fGetCRCAsVector());
    id_code.fSetMemoryType(temp_act_code.fGetMemoryType());
    id_code.fSetMessageNumber(temp_act_code.fGetMessageNo());

    emit SIG_Message(tempstring,id_code,HNS_PLAY_MESSAGE);

    deleteLater();
}


void HNS_Message_Library::on_PB_MsgLib_Pick1_clicked()
{
    switch(f_msglib_mode)
    {
    case HNS_MSGLIB_MESSAGES:
        fSetMsgLibMode(HNS_MSGLIB_ANIMATED);
        break;
    case HNS_MSGLIB_STATIC:
        fSetMsgLibMode(HNS_MSGLIB_ANIMATED);
        break;
    case HNS_MSGLIB_ANIMATED:
        fSetMsgLibMode(HNS_MSGLIB_STATIC);
        break;
    case HNS_MSGLIB_COMBINED:
        fSetMsgLibMode(HNS_MSGLIB_ANIMATED);
        break;
    }
    fUpdate();
}


void HNS_Message_Library::on_PB_MsgLib_Pick2_clicked()
{
    switch(f_msglib_mode)
    {
    case HNS_MSGLIB_MESSAGES:
        if(HNS_Message2::fGetDefaultSignBoardInfo().fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
            fSetMsgLibMode(HNS_MSGLIB_MESSAGES);
        else
            fSetMsgLibMode(HNS_MSGLIB_STATIC);
        break;
    case HNS_MSGLIB_STATIC:
        fSetMsgLibMode(HNS_MSGLIB_MESSAGES);
        break;
    case HNS_MSGLIB_ANIMATED:
        fSetMsgLibMode(HNS_MSGLIB_MESSAGES);
        break;
    case HNS_MSGLIB_COMBINED:
        fSetMsgLibMode(HNS_MSGLIB_STATIC);
        break;
    }
    fUpdate();
}


void HNS_Message_Library::on_PB_MsgLib_Pick3_clicked()
{
    switch(f_msglib_mode)
    {
    case HNS_MSGLIB_MESSAGES:
        fSetMsgLibMode(HNS_MSGLIB_COMBINED);
        break;
    case HNS_MSGLIB_STATIC:
        fSetMsgLibMode(HNS_MSGLIB_COMBINED);
        break;
    case HNS_MSGLIB_ANIMATED:
        fSetMsgLibMode(HNS_MSGLIB_COMBINED);
        break;
    case HNS_MSGLIB_COMBINED:
        fSetMsgLibMode(HNS_MSGLIB_MESSAGES);
        break;
    }
    fUpdate();
}


void HNS_Message_Library::on_PB_MsgLib_Custom_clicked()
{
    f_customChecked = !f_customChecked;
    fUpdate();
}


void HNS_Message_Library::on_PB_MsgLib_Fact_clicked()
{
    f_factoryChecked = !f_factoryChecked;
    fUpdate();
}


void HNS_Message_Library::on_PB_MsgLib_Del_All_clicked()
{
    HNS_Confirm_Screen *temp_widget = new HNS_Confirm_Screen(this,f_machine,"ARE YOU SURE YOU WISH TO DELETE ALL USER MESSAGES?","ACCEPT","CANCEL");

    fShowScreen(temp_widget);

    connect(temp_widget,SIGNAL(SIG_Confirm_Closed(bool)),this,SLOT(my_slot_Del_All_confirmation(bool)));
}

void HNS_Message_Library::my_slot_Del_All_confirmation(const bool &confirmed)
{
    if(confirmed)
    {
        HNS_NTCIP_MessageIDCode id_code; //doesn't matter, just need this to put in the signal.  For a delete all, this is ignored

//        emit SIG_Message_Library_Delete_Request(id_code,true);
        emit SIG_Delete_Request(id_code,true);
    }
}


void HNS_Message_Library::on_PB_MsgLib_Del_clicked()
{
    HNS_Message2 temp_message(HNS_Message2::fGetDefaultSignBoardInfo());
    HNS_NTCIP_MessageActivationCode temp_act_code;
    HNS_NTCIP_MessageIDCode id_code;

    fGetLibrarySelection(&temp_message,&temp_act_code);

    id_code.fSetCRC(temp_act_code.fGetCRCAsVector());
    id_code.fSetMemoryType(temp_act_code.fGetMemoryType());
    id_code.fSetMessageNumber(temp_act_code.fGetMessageNo());

    if(temp_act_code.fGetMemoryType() == NTCIP_MESSAGE::MEM_TYPE_CHANGEABLE)
    {
        emit SIG_Delete_Request(id_code,false);
    }
}

bool HNS_Message_Library::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress && obj == ui->LE_Search)
    {
        fToggleKB(ui->LE_Search,true);
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKB();
        ui->LE_Search->clearFocus();

        //When the mouse pressed inside the partucler widget PGE_MsgLib to get the start point
        QMouseEvent *eventM = static_cast<QMouseEvent*>(event);
        f_startPos.setX(eventM->pos().x());
        f_startPos.setY(eventM->pos().y());
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease && obj == this)
    {
        //When the mouse released inside the partucler widget PGE_MsgLib to get the end point
        QMouseEvent *eventM = static_cast<QMouseEvent*>(event);
        f_endPos.setX(eventM->pos().x());
        f_endPos.setY(eventM->pos().y());
        swipeTriggred(f_startPos, f_endPos);
    }
    return false;
}

/**
 * @brief MainWindow::swipeTriggred - This function is used to handle the
 * left, right, up and down swipe momevement inside message lib window.
 * @param startPos - Mouse pressed start position point
 * @param endPos - Mouse released end position point
 */
void HNS_Message_Library::swipeTriggred(const QPoint &startPos, const QPoint &endPos)
{
    if ((startPos.x() > endPos.x() && startPos.x() - endPos.x() >100)
            || (startPos.y() > endPos.y() && startPos.y() - endPos.y() >100))
    {
         on_PB_MsgLib_Next_clicked();
    }
    else if ((startPos.x() < endPos.x() &&  endPos.x() - startPos.x() >100)
             ||(startPos.y() < endPos.y() &&  endPos.y() - startPos.y() >100))
    {
        on_PB_MsgLib_Prev_clicked();
    }
}

void HNS_Message_Library::fSelectPreview(const int &index)
{
    ui->GV_MessageLibrary_Preview1->fStartStopPreview((index == 1), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview2->fStartStopPreview((index == 2), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview3->fStartStopPreview((index == 3), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview4->fStartStopPreview((index == 4), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview5->fStartStopPreview((index == 5), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview6->fStartStopPreview((index == 6), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview7->fStartStopPreview((index == 7), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview8->fStartStopPreview((index == 8), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
    ui->GV_MessageLibrary_Preview9->fStartStopPreview((index == 9), QDateTime::currentMSecsSinceEpoch(), ((f_msglib_mode == HNS_MSGLIB_ANIMATED) ? 500 : 1000));
}


void HNS_Message_Library::on_LE_Search_textEdited(const QString &arg1)
{
    f_searchString = arg1.toStdString();
    fUpdate();
}

