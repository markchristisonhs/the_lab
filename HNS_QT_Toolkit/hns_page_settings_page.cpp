#include "hns_page_settings_page.h"
#include "ui_hns_page_settings_page.h"

#include "hns_font_settings_screen.h"

HNS_Page_Settings_Page::HNS_Page_Settings_Page(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Page_Settings_Page)
{
    ui->setupUi(this);

    ui->LE_PageSet_PageOff->installEventFilter(this);
    ui->LE_PageSet_PageOn->installEventFilter(this);
    installEventFilter(this);

    f_current_pageoff_time = HNS_Message2::fGetDefaultPageTimeOff();
    f_current_pageon_time = HNS_Message2::fGetDefaultPageTimeOn();
    f_current_font_selection = HNS_Message2::fGetDefaultFont();
    ui->LE_PageSet_PageOff->setText(QString("%1 Sec").arg(HNS_Message2::fGetDefaultPageTimeOff(),0,'f',1));

    ui->LE_PageSet_PageOn->setText(QString("%1 Sec").arg(HNS_Message2::fGetDefaultPageTimeOn(),0,'f',1));

    switch(HNS_Message2::fGetDefaultLineJustification())
    {
    case HNS_JUSTIFICATION_LEFT:
        ui->PB_PageSet_LeftJustification->setChecked(true);
        ui->PB_PageSet_CenterLineJustification->setChecked(false);
        ui->PB_PageSet_RightJustification->setChecked(false);
        break;
    case HNS_JUSTIFICATION_FULL:
    case HNS_JUSTIFICATION_LINE_CENTER:
        ui->PB_PageSet_LeftJustification->setChecked(false);
        ui->PB_PageSet_CenterLineJustification->setChecked(true);
        ui->PB_PageSet_RightJustification->setChecked(false);
        break;
    case HNS_JUSTIFICATION_RIGHT:
        ui->PB_PageSet_LeftJustification->setChecked(false);
        ui->PB_PageSet_CenterLineJustification->setChecked(false);
        ui->PB_PageSet_RightJustification->setChecked(true);
        break;
    }

    switch(HNS_Message2::fGetDefaultPageJustification())
    {
    case HNS_JUSTIFICATION_BOTTOM:
        ui->PB_PageSet_BottomJustification->setChecked(true);
        ui->PB_PageSet_CenterPageJustification->setChecked(false);
        ui->PB_PageSet_TopJustification->setChecked(false);
        break;
    case HNS_JUSTIFICATION_PAGE_CENTER:
        ui->PB_PageSet_BottomJustification->setChecked(false);
        ui->PB_PageSet_CenterPageJustification->setChecked(true);
        ui->PB_PageSet_TopJustification->setChecked(false);
        break;
    case HNS_JUSTIFICATION_TOP:
        ui->PB_PageSet_BottomJustification->setChecked(false);
        ui->PB_PageSet_CenterPageJustification->setChecked(false);
        ui->PB_PageSet_TopJustification->setChecked(true);
        break;
    }

    fUpdate();
}

HNS_Page_Settings_Page::~HNS_Page_Settings_Page()
{
    delete ui;
}

void HNS_Page_Settings_Page::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Page_Settings_Page::fGetType() const
{
    return HNS_WIDGET_PAGE_SETTINGS;
}

void HNS_Page_Settings_Page::on_PB_PageSet_Save_clicked()
{
    type_justification_line justification_line;
    type_justification_page justification_page;

    if(ui->PB_PageSet_LeftJustification->isChecked())
    {
        justification_line = HNS_JUSTIFICATION_LEFT;
    }
    else if(ui->PB_PageSet_CenterLineJustification->isChecked())
    {
        justification_line = HNS_JUSTIFICATION_LINE_CENTER;
    }
    else if(ui->PB_PageSet_RightJustification->isChecked())
    {
        justification_line = HNS_JUSTIFICATION_RIGHT;
    }

    if(ui->PB_PageSet_TopJustification->isChecked())
    {
        justification_page = HNS_JUSTIFICATION_TOP;
    }
    else if(ui->PB_PageSet_CenterPageJustification->isChecked())
    {
        justification_page = HNS_JUSTIFICATION_PAGE_CENTER;
    }
    else if(ui->PB_PageSet_BottomJustification->isChecked())
    {
        justification_page = HNS_JUSTIFICATION_BOTTOM;
    }

    emit SIG_PageSettings_Change_Request(justification_line,
                                         justification_page,
                                         f_current_pageon_time,
                                         f_current_pageoff_time,
                                         f_current_font_selection);

    deleteLater();
}


void HNS_Page_Settings_Page::on_PB_PageSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Page_Settings_Page::on_PB_PageSet_Back_clicked()
{
    deleteLater();
}


void HNS_Page_Settings_Page::on_PB_PageSet_PickFont_clicked()
{
    HNS_Font_Settings_Screen *temp_widget = new HNS_Font_Settings_Screen(this,f_machine,HNS_FONT_SETTINGS);
    fShowScreen(temp_widget);

    temp_widget->fSetFontSelection(f_current_font_selection);

    connect(temp_widget,SIGNAL(SIG_Font_Result(int,bool)),this,SLOT(my_slot_Font_Result(int,bool)));
}


void HNS_Page_Settings_Page::on_PB_PageSet_LeftJustification_clicked()
{
    ui->PB_PageSet_LeftJustification->setChecked(true);
    ui->PB_PageSet_CenterLineJustification->setChecked(false);
    ui->PB_PageSet_RightJustification->setChecked(false);
}


void HNS_Page_Settings_Page::on_PB_PageSet_CenterLineJustification_clicked()
{
    ui->PB_PageSet_LeftJustification->setChecked(false);
    ui->PB_PageSet_CenterLineJustification->setChecked(true);
    ui->PB_PageSet_RightJustification->setChecked(false);
}


void HNS_Page_Settings_Page::on_PB_PageSet_RightJustification_clicked()
{
    ui->PB_PageSet_LeftJustification->setChecked(false);
    ui->PB_PageSet_CenterLineJustification->setChecked(false);
    ui->PB_PageSet_RightJustification->setChecked(true);
}


void HNS_Page_Settings_Page::on_PB_PageSet_TopJustification_clicked()
{
    ui->PB_PageSet_BottomJustification->setChecked(false);
    ui->PB_PageSet_CenterPageJustification->setChecked(false);
    ui->PB_PageSet_TopJustification->setChecked(true);
}


void HNS_Page_Settings_Page::on_PB_PageSet_CenterPageJustification_clicked()
{
    ui->PB_PageSet_BottomJustification->setChecked(false);
    ui->PB_PageSet_CenterPageJustification->setChecked(true);
    ui->PB_PageSet_TopJustification->setChecked(false);
}


void HNS_Page_Settings_Page::on_PB_PageSet_BottomJustification_clicked()
{
    ui->PB_PageSet_BottomJustification->setChecked(true);
    ui->PB_PageSet_CenterPageJustification->setChecked(false);
    ui->PB_PageSet_TopJustification->setChecked(false);
}

void HNS_Page_Settings_Page::my_slot_Font_Result(const int &font_select, const bool &)
{
    f_current_font_selection = font_select;
}

bool HNS_Page_Settings_Page::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_PageSet_PageOff)
    {
        if(fIsKPVisible())
        {
            QLineEdit *widget = (QLineEdit *)fGetKPFocus();
            if(widget != nullptr)
            {
                widget->clearFocus();
            }
            fHideKP();
        }
        ui->LE_PageSet_PageOff->setText("");
        fToggleKP(ui->LE_PageSet_PageOff,280,280);
        return true;
    }
    else if(event->type() == QEvent::MouseButtonPress && obj == ui->LE_PageSet_PageOn)
    {
        if(fIsKPVisible())
        {
            QLineEdit *widget = (QLineEdit *)fGetKPFocus();
            if(widget != nullptr)
            {
                widget->clearFocus();
            }
            fHideKP();
        }
        ui->LE_PageSet_PageOn->setText("");
        fToggleKP(ui->LE_PageSet_PageOn,710,280);
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress && obj == this)
    {
        QLineEdit *widget = (QLineEdit *)fGetKPFocus();
        if(widget != nullptr)
        {
            widget->clearFocus();
        }
        fHideKP();
        return true;
    }
    return false;
}

void HNS_Page_Settings_Page::fVKP_Finished(const bool &enter_pressed)
{
    QString tempstring,tempstring2;
    if(fGetKPFocus() == ui->LE_PageSet_PageOff)
    {
        if(!enter_pressed)
        {
            ui->LE_PageSet_PageOff->setText(QString("%1 Sec").arg(f_current_pageoff_time,0,'f',1));
        }
        else
        {
            tempstring = ui->LE_PageSet_PageOff->text();
            f_current_pageoff_time = ui->LE_PageSet_PageOff->text().toDouble();
            tempstring2 = QString("%1 Sec").arg(f_current_pageoff_time,0,'f',1);
            ui->LE_PageSet_PageOff->setText(QString("%1 Sec").arg(f_current_pageoff_time,0,'f',1));
        }
    }
    else if(fGetKPFocus() == ui->LE_PageSet_PageOn)
    {
        if(!enter_pressed)
        {
            ui->LE_PageSet_PageOn->setText(QString("%1 Sec").arg(f_current_pageon_time,0,'f',1));
        }
        else
        {
            f_current_pageon_time = ui->LE_PageSet_PageOn->text().toDouble();
            tempstring = QString("%1 Sec").arg(f_current_pageon_time,0,'f',1);
            ui->LE_PageSet_PageOn->setText(QString("%1 Sec").arg(f_current_pageon_time,0,'f',1));
        }
    }
}

