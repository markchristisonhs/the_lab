#include "hns_font_settings_screen.h"
#include "ui_hns_font_settings_screen.h"

HNS_Font_Settings_Screen::HNS_Font_Settings_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_Font_Mode &font_mode) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Font_Settings_Screen)
{
    ui->setupUi(this);

    fUpdateFonts();

    switch(font_mode)
    {
    case HNS_FONT_EDIT:
        ui->PB_FontSet_ApplyAfter->show();
        ui->PB_FontSet_ApplyAll->show();
        ui->PB_FontSet_Save->hide();
        ui->PB_FontSet_Cancel->hide();
        break;
    case HNS_FONT_SETTINGS:
        ui->PB_FontSet_ApplyAfter->hide();
        ui->PB_FontSet_ApplyAll->hide();
        ui->PB_FontSet_Save->show();
        ui->PB_FontSet_Cancel->show();
        break;
    }

    connect(ui->GV_Fonts_Preview1,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview1_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview2,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview2_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview3,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview3_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview4,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview4_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview5,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview5_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview6,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview6_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview7,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview7_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview8,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview8_clicked(QMouseEvent *)));
    connect(ui->GV_Fonts_Preview9,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_GV_Font_Preview9_clicked(QMouseEvent *)));
}

HNS_Font_Settings_Screen::~HNS_Font_Settings_Screen()
{
    delete ui;
}

void HNS_Font_Settings_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Font_Settings_Screen::fGetType() const
{
    return HNS_WIDGET_FONT_SETTINGS;
}

void HNS_Font_Settings_Screen::on_PB_FontSet_Save_clicked()
{
    if(fGetFontSelection() > 0)
    {
        emit SIG_Font_Result(fGetFontSelection(),true);
    }
    deleteLater();
}


void HNS_Font_Settings_Screen::on_PB_FontSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Font_Settings_Screen::on_PB_FontSet_Back_clicked()
{
    deleteLater();
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview1_clicked(QMouseEvent *)
{
    fSetFontSelection(1);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview2_clicked(QMouseEvent *)
{
    fSetFontSelection(2);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview3_clicked(QMouseEvent *)
{
    fSetFontSelection(3);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview4_clicked(QMouseEvent *)
{
    fSetFontSelection(4);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview5_clicked(QMouseEvent *)
{
    fSetFontSelection(5);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview6_clicked(QMouseEvent *)
{
    fSetFontSelection(6);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview7_clicked(QMouseEvent *)
{
    fSetFontSelection(7);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview8_clicked(QMouseEvent *)
{
    fSetFontSelection(8);
}

void HNS_Font_Settings_Screen::my_slot_GV_Font_Preview9_clicked(QMouseEvent *)
{
    fSetFontSelection(9);
}


void HNS_Font_Settings_Screen::on_PB_FontSet_ApplyAll_clicked()
{
    if(fGetFontSelection() > 0)
    {
        emit SIG_Font_Result(fGetFontSelection(),true);
    }

    deleteLater();
}


void HNS_Font_Settings_Screen::on_PB_FontSet_ApplyAfter_clicked()
{
    if(fGetFontSelection() > 0)
    {
        emit SIG_Font_Result(fGetFontSelection(),false);
    }

    deleteLater();
}

void HNS_Font_Settings_Screen::fUpdateFonts()
{
    ui->GV_Fonts_Preview1->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview2->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview3->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview4->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview5->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview6->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview7->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview8->fSetFonts(*f_machine->fGetFonts());
    ui->GV_Fonts_Preview9->fSetFonts(*f_machine->fGetFonts());

    for(size_t i=0;i<9;i++)
    {
        switch(i)
        {
        case 0:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName1->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview1->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview1->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview1->show();
            }
            else
            {
                ui->LBL_FontName1->hide();
                ui->GV_Fonts_Preview1->hide();
            }
            break;
        case 1:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName2->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview2->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview2->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview2->show();
            }
            else
            {
                ui->LBL_FontName2->hide();
                ui->GV_Fonts_Preview2->hide();
            }
            break;
        case 2:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName3->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview3->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview3->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview3->show();
            }
            else
            {
                ui->LBL_FontName3->hide();
                ui->GV_Fonts_Preview3->hide();
            }
            break;
        case 3:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName4->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview4->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview4->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview4->show();
            }
            else
            {
                ui->LBL_FontName4->hide();
                ui->GV_Fonts_Preview4->hide();
            }
            break;
        case 4:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName5->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview5->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview5->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview5->show();
            }
            else
            {
                ui->LBL_FontName5->hide();
                ui->GV_Fonts_Preview5->hide();
            }
            break;
        case 5:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName6->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview6->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview6->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview6->show();
            }
            else
            {
                ui->LBL_FontName6->hide();
                ui->GV_Fonts_Preview6->hide();
            }
            break;
        case 6:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName7->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview7->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview7->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview7->show();
            }
            else
            {
                ui->LBL_FontName7->hide();
                ui->GV_Fonts_Preview7->hide();
            }
            break;
        case 7:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName8->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview8->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview8->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview8->show();
            }
            else
            {
                ui->LBL_FontName8->hide();
                ui->GV_Fonts_Preview8->hide();
            }
            break;
        case 8:
            if(i<f_machine->fGetFonts()->size())
            {
                ui->LBL_FontName9->setText(QString::fromStdString(f_machine->fGetFonts()->at(i).fGetFontName()));
                ui->GV_Fonts_Preview9->fSetFonts(*f_machine->fGetFonts());
                ui->GV_Fonts_Preview9->fGenerateFontTest(i+1);
                ui->GV_Fonts_Preview9->show();
            }
            else
            {
                ui->LBL_FontName9->hide();
                ui->GV_Fonts_Preview9->hide();
            }
            break;
        }
    }
}

void HNS_Font_Settings_Screen::fSetFontSelection(const int &select)
{
    ui->GV_Fonts_Preview1->fSelect(select == 1);
    ui->GV_Fonts_Preview2->fSelect(select == 2);
    ui->GV_Fonts_Preview3->fSelect(select == 3);
    ui->GV_Fonts_Preview4->fSelect(select == 4);
    ui->GV_Fonts_Preview5->fSelect(select == 5);
    ui->GV_Fonts_Preview6->fSelect(select == 6);
    ui->GV_Fonts_Preview7->fSelect(select == 7);
    ui->GV_Fonts_Preview8->fSelect(select == 8);
    ui->GV_Fonts_Preview9->fSelect(select == 9);
}

int HNS_Font_Settings_Screen::fGetFontSelection()
{
    if(ui->GV_Fonts_Preview1->fIsSelected())
    {
        return 1;
    }
    else if(ui->GV_Fonts_Preview2->fIsSelected())
    {
        return 2;
    }
    else if(ui->GV_Fonts_Preview3->fIsSelected())
    {
        return 3;
    }
    else if(ui->GV_Fonts_Preview4->fIsSelected())
    {
        return 4;
    }
    else if(ui->GV_Fonts_Preview5->fIsSelected())
    {
        return 5;
    }
    else if(ui->GV_Fonts_Preview6->fIsSelected())
    {
        return 6;
    }
    else if(ui->GV_Fonts_Preview7->fIsSelected())
    {
        return 7;
    }
    else if(ui->GV_Fonts_Preview8->fIsSelected())
    {
        return 8;
    }
    else if(ui->GV_Fonts_Preview9->fIsSelected())
    {
        return 9;
    }
    else
    {
        return 0;
    }
}

