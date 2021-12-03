#include "hns_character_panel.h"
#include "ui_hns_character_panel.h"
#include "hns_qt_toolkit.h"

#include <QPainter>

HNS_Character_Panel::HNS_Character_Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_Character_Panel)
  , f_selected(false)
{
    ui->setupUi(this);
}

HNS_Character_Panel::~HNS_Character_Panel()
{
    delete ui;
}

void HNS_Character_Panel::fSetCharacter(const HNS_Character &character)
{
    ConvertBoolArrayToQImage(character.fGetData(),character.fGetHeight()*character.fGetWidth(),character.fGetWidth(),character.fGetHeight(),f_character_img,Qt::white);

    this->repaint();
}

void HNS_Character_Panel::fClearCharacter()
{
    f_character_img = QImage();

    this->repaint();
}

void HNS_Character_Panel::fSetGraphic(const QImage &graphic)
{
    f_character_img = graphic;
    this->repaint();
}

void HNS_Character_Panel::fSelect(const bool &selected)
{
    f_selected = selected;

    this->repaint();
}

bool HNS_Character_Panel::fIsSelected() const
{
    return f_selected;
}

void HNS_Character_Panel::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QImage preview;
    if(f_character_img != QImage())
    {
        preview= f_character_img.scaled(size().width()-10,size().height()-10,Qt::KeepAspectRatio);
    }

    painter.fillRect(0,0,size().width(),size().height(),QBrush(Qt::black,Qt::SolidPattern));
    if(f_character_img != QImage())
    {
        painter.drawImage(5,5,preview);
    }

    if(f_selected)
    {
        QPen temp_pen(QColor(255,165,0));
        temp_pen.setWidth(15);
        painter.setPen(temp_pen);
        painter.drawRect(0,0,size().width(),size().height());
    }
}

void HNS_Character_Panel::mousePressEvent(QMouseEvent */*event*/)
{
    emit fClicked();
}
