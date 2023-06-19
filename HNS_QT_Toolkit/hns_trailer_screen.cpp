#include "hns_trailer_screen.h"
#include "ui_hns_trailer_screen.h"

using namespace std;

HNS_Trailer_Screen::HNS_Trailer_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Trailer_Screen)
{
    ui->setupUi(this);

    size_t num_trailers = f_machine->fGetTrailerNames().size();
    int trailer_select = f_machine->fGetSelectedTrailer();
    vector<string> trailer_names = f_machine->fGetTrailerNames();

    if(num_trailers >= 1)
    {
        ui->PB_TrailerSet_Trailer1->show();
        ui->PB_TrailerSet_Trailer1->setText(QString::fromStdString(trailer_names[0]));
        ui->PB_TrailerSet_Trailer1->setChecked(trailer_select == 0);
    }
    else
    {
        ui->PB_TrailerSet_Trailer1->hide();
    }

    if(num_trailers >= 2)
    {
        ui->PB_TrailerSet_Trailer2->show();
        ui->PB_TrailerSet_Trailer2->setText(QString::fromStdString(trailer_names[1]));
        ui->PB_TrailerSet_Trailer2->setChecked(trailer_select == 1);
    }
    else
    {
        ui->PB_TrailerSet_Trailer2->hide();
    }
    if(num_trailers >= 3)
    {
        ui->PB_TrailerSet_Trailer3->show();
        ui->PB_TrailerSet_Trailer3->setText(QString::fromStdString(trailer_names[2]));
        ui->PB_TrailerSet_Trailer3->setChecked(trailer_select == 2);
    }
    else
    {
        ui->PB_TrailerSet_Trailer3->hide();
    }

    if(num_trailers >= 4)
    {
        ui->PB_TrailerSet_Trailer4->show();
        ui->PB_TrailerSet_Trailer4->setText(QString::fromStdString(trailer_names[3]));
        ui->PB_TrailerSet_Trailer4->setChecked(trailer_select == 3);
    }
    else
    {
        ui->PB_TrailerSet_Trailer4->hide();
    }

    if(num_trailers >= 5)
    {
        ui->PB_TrailerSet_Trailer5->show();
        ui->PB_TrailerSet_Trailer5->setText(QString::fromStdString(trailer_names[4]));
        ui->PB_TrailerSet_Trailer5->setChecked(trailer_select == 4);
    }
    else
    {
        ui->PB_TrailerSet_Trailer5->hide();
    }

    if(num_trailers >= 6)
    {
        ui->PB_TrailerSet_Trailer6->show();
        ui->PB_TrailerSet_Trailer6->setText(QString::fromStdString(trailer_names[5]));
        ui->PB_TrailerSet_Trailer6->setChecked(trailer_select == 5);
    }
    else
    {
        ui->PB_TrailerSet_Trailer6->hide();
    }

    if(num_trailers >= 7)
    {
        ui->PB_TrailerSet_Trailer7->show();
        ui->PB_TrailerSet_Trailer7->setText(QString::fromStdString(trailer_names[6]));
        ui->PB_TrailerSet_Trailer7->setChecked(trailer_select == 6);
    }
    else
    {
        ui->PB_TrailerSet_Trailer7->hide();
    }

    if(num_trailers >= 8)
    {
        ui->PB_TrailerSet_Trailer8->show();
        ui->PB_TrailerSet_Trailer8->setText(QString::fromStdString(trailer_names[7]));
        ui->PB_TrailerSet_Trailer8->setChecked(trailer_select == 7);
    }
    else
    {
        ui->PB_TrailerSet_Trailer8->hide();
    }

    if(num_trailers >= 9)
    {
        ui->PB_TrailerSet_Trailer9->show();
        ui->PB_TrailerSet_Trailer9->setText(QString::fromStdString(trailer_names[8]));
        ui->PB_TrailerSet_Trailer9->setChecked(trailer_select == 8);
    }
    else
    {
        ui->PB_TrailerSet_Trailer9->hide();
    }

    if(num_trailers >= 10)
    {
        ui->PB_TrailerSet_Trailer10->show();
        ui->PB_TrailerSet_Trailer10->setText(QString::fromStdString(trailer_names[9]));
        ui->PB_TrailerSet_Trailer10->setChecked(trailer_select == 9);
    }
    else
    {
        ui->PB_TrailerSet_Trailer10->hide();
    }
}

HNS_Trailer_Screen::~HNS_Trailer_Screen()
{
    delete ui;
}

void HNS_Trailer_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Trailer_Screen::fGetType() const
{
    return HNS_WIDGET_TRAILER;
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Save_clicked()
{
    bool to_delete = fGetSelectedTrailer() == (f_machine->fGetSelectedTrailer() +1);
    emit SIG_Trailer_Change_Request(fGetSelectedTrailer());
    if(to_delete)
    {
        deleteLater();
    }
}


void HNS_Trailer_Screen::on_PB_TrailerSet_Cancel_clicked()
{
    deleteLater();
}


void HNS_Trailer_Screen::on_PB_TrailerSet_Back_clicked()
{
    deleteLater();
}

void HNS_Trailer_Screen::fSelectTrailer(const int &select)
{
    ui->PB_TrailerSet_Trailer1->setChecked(select == 1);
    ui->PB_TrailerSet_Trailer2->setChecked(select == 2);
    ui->PB_TrailerSet_Trailer3->setChecked(select == 3);
    ui->PB_TrailerSet_Trailer4->setChecked(select == 4);
    ui->PB_TrailerSet_Trailer5->setChecked(select == 5);
    ui->PB_TrailerSet_Trailer6->setChecked(select == 6);
    ui->PB_TrailerSet_Trailer7->setChecked(select == 7);
    ui->PB_TrailerSet_Trailer8->setChecked(select == 8);
    ui->PB_TrailerSet_Trailer9->setChecked(select == 9);
    ui->PB_TrailerSet_Trailer10->setChecked(select == 10);
}

int HNS_Trailer_Screen::fGetSelectedTrailer() const
{
    if(ui->PB_TrailerSet_Trailer1->isChecked())
    {
        return 1;
    }
    else if(ui->PB_TrailerSet_Trailer2->isChecked())
    {
        return 2;
    }
    else if(ui->PB_TrailerSet_Trailer3->isChecked())
    {
        return 3;
    }
    else if(ui->PB_TrailerSet_Trailer4->isChecked())
    {
        return 4;
    }
    else if(ui->PB_TrailerSet_Trailer5->isChecked())
    {
        return 5;
    }
    else if(ui->PB_TrailerSet_Trailer6->isChecked())
    {
        return 6;
    }
    else if(ui->PB_TrailerSet_Trailer7->isChecked())
    {
        return 7;
    }
    else if(ui->PB_TrailerSet_Trailer8->isChecked())
    {
        return 8;
    }
    else if(ui->PB_TrailerSet_Trailer9->isChecked())
    {
        return 9;
    }
    else if(ui->PB_TrailerSet_Trailer10->isChecked())
    {
        return 10;
    }
    else
    {
        return 0;
    }
}


void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer1_clicked()
{
    fSelectTrailer(1);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer2_clicked()
{
    fSelectTrailer(2);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer3_clicked()
{
    fSelectTrailer(3);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer4_clicked()
{
    fSelectTrailer(4);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer5_clicked()
{
    fSelectTrailer(5);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer6_clicked()
{
    fSelectTrailer(6);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer7_clicked()
{
    fSelectTrailer(7);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer8_clicked()
{
    fSelectTrailer(8);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer9_clicked()
{
    fSelectTrailer(9);
}

void HNS_Trailer_Screen::on_PB_TrailerSet_Trailer10_clicked()
{
    fSelectTrailer(10);
}

