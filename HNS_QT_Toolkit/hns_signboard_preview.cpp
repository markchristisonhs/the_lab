#include "hns_signboard_preview.h"
#include "ui_hns_signboard_preview.h"
#include "hns_qt_toolkit.h"
#include <QPainter>

HNS_Signboard_Preview::HNS_Signboard_Preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_Signboard_Preview)
{
    ui->setupUi(this);
}

HNS_Signboard_Preview::~HNS_Signboard_Preview()
{
    delete ui;
}

void HNS_Signboard_Preview::fSetSignBoard(const HNS_SignBoard &signboard)
{
    f_signboard = signboard;
    fUpdate();
}

QImage HNS_Signboard_Preview::fGetContents()
{
    QImage result(size(),QImage::Format_ARGB32);

    static double width_ratio = 0.95;
    static double height_ratio = 0.95;
    size_t xpos,ypos,box_width,box_height;
    QPainter painter(this);
    painter.begin(&result);
    painter.fillRect(0,0,size().width(),size().height(),QBrush(Qt::black,Qt::SolidPattern));

    if(f_signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        if(!f_image.isNull())
        {
            QImage preview = f_image.scaled(size().width(),size().height());
            painter.drawImage(0,0,preview);
        }
    }
    else if(f_signboard.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        int left,right,top,bottom;
        size_t row,col;

        for(size_t i=0;i<f_signboard.fGetBoardsWide()-1;i++)
        {
            right = (((width() * width_ratio)/f_signboard.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1))) * (i+1);
            left = right - ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1));
            top = 0;
            bottom = height();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        for(size_t i=0;i<f_signboard.fGetBoardsTall()-1;i++)
        {
            bottom = (((height() * height_ratio)/f_signboard.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1))) * (i+1);
            top = bottom - ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1));
            left = 0;
            right = width();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        if(f_images.size() > 0)
        {

            for(size_t ui = 0; ui < f_signboard.fGetNumBoards(); ui++)
            {
                row = ui / f_signboard.fGetBoardsWide();
                col = ui % f_signboard.fGetBoardsWide();

                xpos = (((width() * width_ratio)/f_signboard.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1))) * col;
                ypos = (((height() * height_ratio)/f_signboard.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1))) * row;
                box_width = ((width() * width_ratio)/f_signboard.fGetBoardsWide());
                box_height = ((height() * height_ratio)/f_signboard.fGetBoardsTall());

                QImage preview = f_images[ui].scaled(box_width,box_height);
                painter.drawImage(xpos,ypos,preview);
            }
        }
    }
    painter.end();

    return result;
}

void HNS_Signboard_Preview::fUpdate()
{
    HNS_Bitmap temp_bitmap;
    if(f_signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        temp_bitmap = f_signboard.fGetFMBitmap();
        f_image = ConvertHNS_BitmapToQImage(temp_bitmap,HNS_Color(255,255,255),QColor(255,172,39));
    }
    else if(f_signboard.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        f_images.clear();
        f_images.resize(f_signboard.fGetNumBoards());
        for(size_t ui = 0; ui < f_signboard.fGetNumBoards(); ui++)
        {
            temp_bitmap = f_signboard.fGetCharBoardBitmap(ui);
            f_images[ui] = ConvertHNS_BitmapToQImage(f_signboard.fGetCharBoardBitmap(ui),HNS_Color(255,255,255),QColor(255,172,39));
        }
    }

    this->repaint();
}

void HNS_Signboard_Preview::paintEvent(QPaintEvent *)
{
    static double width_ratio = 0.95;
    static double height_ratio = 0.95;
    size_t xpos,ypos,box_width,box_height;
    QPainter painter(this);
    painter.fillRect(0,0,size().width(),size().height(),QBrush(Qt::black,Qt::SolidPattern));

    if(f_signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        if(!f_image.isNull())
        {
            QImage preview = f_image.scaled(size().width(),size().height());
            painter.drawImage(0,0,preview);
        }
    }
    else if(f_signboard.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        int left,right,top,bottom;
        size_t row,col;

        for(size_t i=0;i<f_signboard.fGetBoardsWide()-1;i++)
        {
            right = (((width() * width_ratio)/f_signboard.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1))) * (i+1);
            left = right - ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1));
            top = 0;
            bottom = height();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        for(size_t i=0;i<f_signboard.fGetBoardsTall()-1;i++)
        {
            bottom = (((height() * height_ratio)/f_signboard.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1))) * (i+1);
            top = bottom - ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1));
            left = 0;
            right = width();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        if(f_images.size() > 0)
        {

            for(size_t ui = 0; ui < f_signboard.fGetNumBoards(); ui++)
            {
                row = ui / f_signboard.fGetBoardsWide();
                col = ui % f_signboard.fGetBoardsWide();

                xpos = (((width() * width_ratio)/f_signboard.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(f_signboard.fGetBoardsWide() - 1))) * col;
                ypos = (((height() * height_ratio)/f_signboard.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(f_signboard.fGetBoardsTall() - 1))) * row;
                box_width = ((width() * width_ratio)/f_signboard.fGetBoardsWide());
                box_height = ((height() * height_ratio)/f_signboard.fGetBoardsTall());

                QImage preview = f_images[ui].scaled(box_width,box_height);
                painter.drawImage(xpos,ypos,preview);
            }
        }
    }
    painter.end();
}
