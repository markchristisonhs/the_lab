#include "hns_message_edit_screen.h"
#include "ui_hns_message_edit_screen.h"

#include "hns_preview_screen.h"
#include "hns_font_settings_screen.h"
#include "hns_message_library.h"
#include "hns_direct_multi_screen.h"

using namespace std;

HNS_Message_Edit_Screen::HNS_Message_Edit_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_edit_mode &edit_mode, const QString &multi) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Message_Edit_Screen)
  , f_edit_mode(edit_mode)
{
    QString tempstring;

    ui->setupUi(this);

    f_current_pageon_time = HNS_Message2::fGetDefaultPageTimeOn();
    f_current_pageoff_time = HNS_Message2::fGetDefaultPageTimeOff();

    tempstring = QString("%1").arg(f_current_pageon_time,0,'f',1);
    ui->LE_Edit_PageOnTime->setText(tempstring);

    tempstring = QString("%1").arg(f_current_pageoff_time,0,'f',1);
    ui->LE_Edit_PageOffTime->setText(tempstring);

    switch(HNS_Message2::fGetDefaultLineJustification())
    {
    case HNS_JUSTIFICATION_LEFT:
        ui->PB_Edit_LeftJustify->setChecked(true);
        ui->PB_Edit_CenterLineJustify->setChecked(false);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_LINE_CENTER:
        ui->PB_Edit_LeftJustify->setChecked(false);
        ui->PB_Edit_CenterLineJustify->setChecked(true);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_RIGHT:
        ui->PB_Edit_LeftJustify->setChecked(false);
        ui->PB_Edit_CenterLineJustify->setChecked(false);
        ui->PB_Edit_RightJustify->setChecked(true);
        break;
    case HNS_JUSTIFICATION_FULL:
        ui->PB_Edit_LeftJustify->setChecked(false);
        ui->PB_Edit_CenterLineJustify->setChecked(true);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    }

    switch(HNS_Message2::fGetDefaultPageJustification())
    {
    case HNS_JUSTIFICATION_TOP:
        ui->PB_Edit_TopJustify->setChecked(true);
        ui->PB_Edit_CenterPageJustify->setChecked(false);
        ui->PB_Edit_BottomJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_PAGE_CENTER:
        ui->PB_Edit_TopJustify->setChecked(false);
        ui->PB_Edit_CenterPageJustify->setChecked(true);
        ui->PB_Edit_BottomJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_BOTTOM:
        ui->PB_Edit_TopJustify->setChecked(false);
        ui->PB_Edit_CenterPageJustify->setChecked(false);
        ui->PB_Edit_BottomJustify->setChecked(true);
        break;
    }

    ui->GV_Edit_Preview->installEventFilter(this);
    ui->LE_Edit_PageOffTime->installEventFilter(this);
    ui->LE_Edit_PageOnTime->installEventFilter(this);
    installEventFilter(this);
    ui->GV_Edit_Preview->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Edit_Preview->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_Edit_Preview->fSetMessage(multi);
    ui->GV_Edit_Preview->fSelectPage(ui->GV_Edit_Preview->fTotalPage()-1);

    if(!f_machine->fGetRadarConnected())
    {
        ui->PGE_Radar_Info->hide();
    }
    ui->PGE_Radar_Info->fSetRadarInfo(f_machine->fGetRadarMode(),f_machine->fGetRadarLegacySpeed(),f_machine->fGetRadarPoliceSpeed(),f_machine->fGetStrobeSettings());

    connect(ui->GV_Edit_Preview,SIGNAL(fLineJustificationChanged(const type_justification_line &)),this,SLOT(GV_Edit_LineJustificationChanged(const type_justification_line &)));
    connect(ui->GV_Edit_Preview,SIGNAL(fPageJustificationChanged(const type_justification_page &)),this,SLOT(GV_Edit_PageJustificationChanged(const type_justification_page &)));
    connect(ui->GV_Edit_Preview,SIGNAL(fNewPageCreated()),this,SLOT(my_slot_EditPreview_NewPageCreated()));
    connect(ui->GV_Edit_Preview,SIGNAL(fFlashStateChanged()),this,SLOT(my_slot_Edit_Flash_Changed()));

    fUpdate();
}

HNS_Message_Edit_Screen::~HNS_Message_Edit_Screen()
{
    delete ui;
}

type_widget HNS_Message_Edit_Screen::fGetType() const
{
    return HNS_WIDGET_MESSAGE_EDIT;
}

void HNS_Message_Edit_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

bool HNS_Message_Edit_Screen::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->GV_Edit_Preview)
    {
        fToggleKB(ui->GV_Edit_Preview,true);

        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Edit_PageOffTime)
    {
        on_LE_Edit_PageOffTime_clicked();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_Edit_PageOnTime)
    {
        on_LE_Edit_PageOnTime_clicked();
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        fHideKB();
        return true;
    }

    else if(event->type() == QEvent::MouseButtonPress && obj != ui->GV_Edit_Preview)
    {
        fHideKB();
    }

    return false;
}

void HNS_Message_Edit_Screen::on_PB_Edit_Back_clicked()
{
    deleteLater();
}


void HNS_Message_Edit_Screen::on_PB_Edit_Cancel_clicked()
{
    deleteLater();
}


void HNS_Message_Edit_Screen::on_PB_Edit_Preview_clicked()
{
    fHideKB();
    type_preview_mode preview_mode = HNS_PREVIEW_NORMAL;

    if(f_edit_mode == HNS_EDIT_PICKER)
    {
        preview_mode = HNS_PREVIEW_PICKER;
    }
    HNS_Preview_Screen *temp_widget = new HNS_Preview_Screen(this,
                                                             f_machine,
                                                             preview_mode,
                                                             QString::fromStdString(ui->GV_Edit_Preview->fGetCurrentMessage().fGetMULTI()));
    fShowScreen(temp_widget);

    connect(temp_widget,SIGNAL(SIG_Preview_Accepted()),this,SLOT(my_slot_Preview_Accepted()));
    connect(temp_widget,SIGNAL(SIG_Preview_Add_Library()),this,SLOT(my_slot_Preview_Add_Library()));
}


void HNS_Message_Edit_Screen::on_PB_Edit_FontOptions_clicked()
{
    fHideKB();
    HNS_Font_Settings_Screen *temp_widget = new HNS_Font_Settings_Screen(this, f_machine, HNS_FONT_EDIT);
    fShowScreen(temp_widget);

    connect(temp_widget,SIGNAL(SIG_Font_Result(int,bool)),this,SLOT(my_slot_Font_Selected(int,bool)));
}


void HNS_Message_Edit_Screen::on_PB_Edit_AddMessage_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_MESSAGES);

    fShowScreen(temp_widget);
}


void HNS_Message_Edit_Screen::on_PB_Edit_AddStaticImage_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_STATIC);

    fShowScreen(temp_widget);
}


void HNS_Message_Edit_Screen::on_PB_Edit_AddAnimatedImage_clicked()
{
    HNS_Message_Library *temp_widget = new HNS_Message_Library(this, f_machine, HNS_MSGLIB_ANIMATED);

    fShowScreen(temp_widget);
}


void HNS_Message_Edit_Screen::on_PB_Edit_Direct_MULTI_clicked()
{
    HNS_Direct_Multi_Screen *temp_widget = new HNS_Direct_Multi_Screen(this, f_machine, QString::fromStdString(ui->GV_Edit_Preview->fGetCurrentMessage().fGetMULTI()));

    fShowScreen(temp_widget);

    connect(temp_widget,SIGNAL(SIG_Direct_MULTI_Result(QString)),this,SLOT(my_slot_Direct_Multi_Finished(QString)));
}

void HNS_Message_Edit_Screen::on_PB_Edit_LeftJustify_clicked()
{
    ui->GV_Edit_Preview->fSetLineJustification(HNS_JUSTIFICATION_LEFT);
    ui->PB_Edit_LeftJustify->setChecked(true);
    ui->PB_Edit_CenterLineJustify->setChecked(false);
    ui->PB_Edit_RightJustify->setChecked(false);
}


void HNS_Message_Edit_Screen::on_PB_Edit_CenterLineJustify_clicked()
{
    ui->GV_Edit_Preview->fSetLineJustification(HNS_JUSTIFICATION_LINE_CENTER);
    ui->PB_Edit_LeftJustify->setChecked(false);
    ui->PB_Edit_CenterLineJustify->setChecked(true);
    ui->PB_Edit_RightJustify->setChecked(false);
}


void HNS_Message_Edit_Screen::on_PB_Edit_RightJustify_clicked()
{
    ui->GV_Edit_Preview->fSetLineJustification(HNS_JUSTIFICATION_RIGHT);
    ui->PB_Edit_LeftJustify->setChecked(false);
    ui->PB_Edit_CenterLineJustify->setChecked(false);
    ui->PB_Edit_RightJustify->setChecked(true);
}


void HNS_Message_Edit_Screen::on_PB_Edit_TopJustify_clicked()
{
    ui->GV_Edit_Preview->fSetPageJustification(HNS_JUSTIFICATION_TOP);
    ui->PB_Edit_TopJustify->setChecked(true);
    ui->PB_Edit_CenterPageJustify->setChecked(false);
    ui->PB_Edit_BottomJustify->setChecked(false);
}


void HNS_Message_Edit_Screen::on_PB_Edit_CenterPageJustify_clicked()
{
    ui->GV_Edit_Preview->fSetPageJustification(HNS_JUSTIFICATION_PAGE_CENTER);
    ui->PB_Edit_TopJustify->setChecked(false);
    ui->PB_Edit_CenterPageJustify->setChecked(true);
    ui->PB_Edit_BottomJustify->setChecked(false);
}


void HNS_Message_Edit_Screen::on_PB_Edit_BottomJustify_clicked()
{
    ui->GV_Edit_Preview->fSetPageJustification(HNS_JUSTIFICATION_BOTTOM);
    ui->PB_Edit_TopJustify->setChecked(false);
    ui->PB_Edit_CenterPageJustify->setChecked(false);
    ui->PB_Edit_BottomJustify->setChecked(true);
}


void HNS_Message_Edit_Screen::on_PB_Edit_InsertRadarSpeed_clicked()
{
    fHideKB();
    ui->GV_Edit_Preview->fAddText("[f6]");
}


void HNS_Message_Edit_Screen::on_PB_Edit_FlashText_clicked()
{
    ui->GV_Edit_Preview->fToggleFlashTag(1.0,1.0,true);
}

void HNS_Message_Edit_Screen::my_slot_Direct_Multi_Finished(const QString &multi)
{
    ui->GV_Edit_Preview->fSetMessage(multi);
    fUpdate();
}

void HNS_Message_Edit_Screen::my_slot_Font_Selected(const int &font_select, const bool &apply_all)
{
    ui->GV_Edit_Preview->fChangeFont(font_select,apply_all);
    fUpdate();
}

void HNS_Message_Edit_Screen::my_slot_Preview_Accepted()
{
    emit SIG_Message(QString::fromStdString(ui->GV_Edit_Preview->fGetCurrentMessage().fGetMULTI()),HNS_NTCIP_MessageIDCode(),HNS_ADD_AND_PLAY);

    deleteLater();
}

void HNS_Message_Edit_Screen::my_slot_Preview_Add_Library()
{
    emit SIG_Message(QString::fromStdString(ui->GV_Edit_Preview->fGetCurrentMessage().fGetMULTI()),HNS_NTCIP_MessageIDCode(),HNS_ADD_ONLY);

    deleteLater();
}


void HNS_Message_Edit_Screen::on_PB_Edit_AddPage_clicked()
{
    fHideKB();
    ui->GV_Edit_Preview->fAddPage();
    fUpdate();
}


void HNS_Message_Edit_Screen::on_PB_Edit_RemovePage_clicked()
{
    ui->GV_Edit_Preview->fRemovePage();
    fUpdate();
}

void HNS_Message_Edit_Screen::on_LE_Edit_PageOffTime_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Edit_PageOffTime->clear();
        fShowKP(ui->LE_Edit_PageOffTime,480,440);
    }
    else
    {
        //already open.  Do I close it or transfer control to this control?
        if(fGetKPFocus() == ui->LE_Edit_PageOffTime)
        {
            LE_Edit_PageOffTime_editingFinished(false);
            fHideKP();
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Edit_PageOnTime)
            {
                LE_Edit_PageOnTime_editingFinished(false);
            }
            ui->LE_Edit_PageOffTime->clear();
            fChangeKPFocus(ui->LE_Edit_PageOffTime);
        }
    }
}

void HNS_Message_Edit_Screen::on_LE_Edit_PageOnTime_clicked()
{
    if(!fIsKPVisible())
    {
        ui->LE_Edit_PageOnTime->clear();
        fToggleKP(ui->LE_Edit_PageOnTime,340,440);
    }
    else
    {
        if(fGetKPFocus() == ui->LE_Edit_PageOnTime)
        {
            LE_Edit_PageOnTime_editingFinished(false);
            fToggleKP(ui->LE_Edit_PageOnTime,140,0);
        }
        else
        {
            if(fGetKPFocus() == ui->LE_Edit_PageOffTime)
            {
                LE_Edit_PageOffTime_editingFinished(false);
            }
            ui->LE_Edit_PageOnTime->clear();
            fChangeKPFocus(ui->LE_Edit_PageOnTime);
        }
    }
}


void HNS_Message_Edit_Screen::LE_Edit_PageOnTime_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(ui->LE_Edit_PageOnTime->text().isEmpty() || !enter_pressed)
    {
        tempstring = QString("%1").arg(f_current_pageon_time,0,'f',1);
        ui->LE_Edit_PageOnTime->setText(tempstring);
    }
    else
    {
        bool ok = true;
        double temp;

        tempstring = ui->LE_Edit_PageOnTime->text();
        temp = tempstring.toDouble(&ok);

        if(ok)
        {
            f_current_pageon_time = temp;
            ui->GV_Edit_Preview->fSetPageTimes(f_current_pageon_time,f_current_pageoff_time);
            tempstring = QString("%1").arg(f_current_pageon_time,0,'f',1);
            ui->LE_Edit_PageOnTime->setText(tempstring);
        }
        else
        {
            tempstring = QString("%1").arg(f_current_pageon_time,0,'f',1);
            ui->LE_Edit_PageOnTime->setText(tempstring);
        }
    }
}


void HNS_Message_Edit_Screen::LE_Edit_PageOffTime_editingFinished(const bool &enter_pressed)
{
    QString tempstring;
    if(ui->LE_Edit_PageOnTime->text().isEmpty() || !enter_pressed)
    {
        tempstring = QString("%1").arg(f_current_pageoff_time,0,'f',1);
        ui->LE_Edit_PageOffTime->setText(tempstring);
    }
    else
    {
        bool ok = true;
        double temp;

        tempstring = ui->LE_Edit_PageOffTime->text();
        temp = tempstring.toDouble(&ok);

        if(ok)
        {
            f_current_pageoff_time = temp;
            ui->GV_Edit_Preview->fSetPageTimes(f_current_pageon_time,f_current_pageoff_time);
            tempstring = QString("%1").arg(f_current_pageoff_time,0,'f',1);
            ui->LE_Edit_PageOffTime->setText(tempstring);
        }
        else
        {
            tempstring = QString("%1").arg(f_current_pageoff_time,0,'f',1);
            ui->LE_Edit_PageOffTime->setText(tempstring);
        }
    }
}

void HNS_Message_Edit_Screen::fUpdate(const int &update_flags)
{
    QString tempstring;
    int current_page = ui->GV_Edit_Preview->fGetCurrentPage();

    ui->GV_Edit_Preview->fGetPageTimes(f_current_pageon_time,f_current_pageoff_time);

    tempstring = QString("%1").arg(f_current_pageon_time,0,'f',1);
    ui->LE_Edit_PageOnTime->setText(tempstring);

    tempstring = QString("%1").arg(f_current_pageoff_time,0,'f',1);
    ui->LE_Edit_PageOffTime->setText(tempstring);

    if(f_edit_mode == HNS_EDIT_NORMAL)
    {
        ui->LBL_Edit_Title->setText(QString("NEW MESSAGE - PAGE %1").arg(current_page+1));
    }
    else
    {
        ui->LBL_Edit_Title->setText(QString("EDIT CURRENT MESSAGE - PAGE %1").arg(current_page+1));
    }

    if(HNS_Message2::fGetDefaultSignBoardInfo().fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        ui->PB_Edit_AddStaticImage->hide();
        ui->PB_Edit_AddAnimatedImage->hide();
    }
    else
    {
        ui->PB_Edit_AddStaticImage->show();
        ui->PB_Edit_AddAnimatedImage->show();
    }

    if(ui->GV_Edit_Preview->fTotalPage() <= 1)
    {
        ui->PB_Edit_Prev->hide();
        ui->PB_Edit_Next->hide();
    }
    if((ui->GV_Edit_Preview->fGetCurrentPage() + 1) == ui->GV_Edit_Preview->fTotalPage() &&
            ui->GV_Edit_Preview->fTotalPage() > 1)
    {
        ui->PB_Edit_Next->hide();
        ui->PB_Edit_Prev->show();
    }
    if(ui->GV_Edit_Preview->fGetCurrentPage()==0 && ui->GV_Edit_Preview->fTotalPage() > 1)
    {
        ui->PB_Edit_Prev->hide();
        ui->PB_Edit_Next->show();
    }
    if(((ui->GV_Edit_Preview->fGetCurrentPage() + 1) < ui->GV_Edit_Preview->fTotalPage()) &&
           (ui->GV_Edit_Preview->fGetCurrentPage()  >  0))
    {
        ui->PB_Edit_Next->show();
        ui->PB_Edit_Prev->show();
    }

    ui->PB_Edit_FlashText->setChecked(ui->GV_Edit_Preview->fIsFlashTagOpen());

    HNS_UI_Widget::fUpdate(update_flags);
}


void HNS_Message_Edit_Screen::on_PB_Edit_Prev_clicked()
{
    ui->GV_Edit_Preview->fPrevPage();
    fUpdate();
}


void HNS_Message_Edit_Screen::on_PB_Edit_Next_clicked()
{
    ui->GV_Edit_Preview->fNextPage();
    fUpdate();
}

void HNS_Message_Edit_Screen::my_slot_Edit_Flash_Changed()
{
    ui->PB_Edit_FlashText->setChecked(ui->GV_Edit_Preview->fIsFlashTagOpen());
}

void HNS_Message_Edit_Screen::GV_Edit_LineJustificationChanged(const type_justification_line &line_justification)
{
    switch(line_justification)
    {
    case HNS_JUSTIFICATION_LEFT:
        ui->PB_Edit_LeftJustify->setChecked(true);
        ui->PB_Edit_CenterLineJustify->setChecked(false);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_LINE_CENTER:
        ui->PB_Edit_LeftJustify->setChecked(false);
        ui->PB_Edit_CenterLineJustify->setChecked(true);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_RIGHT:
        ui->PB_Edit_LeftJustify->setChecked(false);
        ui->PB_Edit_CenterLineJustify->setChecked(false);
        ui->PB_Edit_RightJustify->setChecked(true);
        break;
    case HNS_JUSTIFICATION_FULL:
        ui->PB_Edit_LeftJustify->setChecked(true);
        ui->PB_Edit_CenterLineJustify->setChecked(false);
        ui->PB_Edit_RightJustify->setChecked(false);
        break;
    }
    fUpdate();
}

void HNS_Message_Edit_Screen::GV_Edit_PageJustificationChanged(const type_justification_page &page_justification)
{
    switch(page_justification)
    {
    case HNS_JUSTIFICATION_TOP:
        ui->PB_Edit_TopJustify->setChecked(true);
        ui->PB_Edit_CenterPageJustify->setChecked(false);
        ui->PB_Edit_BottomJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_PAGE_CENTER:
        ui->PB_Edit_TopJustify->setChecked(false);
        ui->PB_Edit_CenterPageJustify->setChecked(true);
        ui->PB_Edit_BottomJustify->setChecked(false);
        break;
    case HNS_JUSTIFICATION_BOTTOM:
        ui->PB_Edit_TopJustify->setChecked(false);
        ui->PB_Edit_CenterPageJustify->setChecked(false);
        ui->PB_Edit_BottomJustify->setChecked(true);
        break;
    }
    fUpdate();
}

void HNS_Message_Edit_Screen::my_slot_EditPreview_NewPageCreated()
{
    fUpdate();
}

void HNS_Message_Edit_Screen::my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &, const type_ui_message_mode &)
{
    ui->GV_Edit_Preview->fInsertMessage(multi);
    fUpdate();
}

void HNS_Message_Edit_Screen::fVKP_Finished(const bool &enter_pressed)
{
    if(fGetKPFocus() == ui->LE_Edit_PageOffTime)
    {
        LE_Edit_PageOffTime_editingFinished(enter_pressed);
    }
    else if(fGetKPFocus() == ui->LE_Edit_PageOnTime)
    {
        LE_Edit_PageOnTime_editingFinished(enter_pressed);
    }
}

