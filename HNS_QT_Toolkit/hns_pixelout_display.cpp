#include "hns_pixelout_display.h"
#include "ui_hns_pixelout_display.h"

#include <QPainter>
#include <vector>

using namespace std;

HNS_PixelOut_Display::HNS_PixelOut_Display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_PixelOut_Display)
{
    ui->setupUi(this);
}

HNS_PixelOut_Display::~HNS_PixelOut_Display()
{
    delete ui;
}

void HNS_PixelOut_Display::fSetSignBoard(const HNS_SignBoard &board)
{
    f_signboard = board;
    repaint();
}

void HNS_PixelOut_Display::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(0,0,size().width(),size().height(),QBrush(Qt::black,Qt::SolidPattern));
    int x,y;
    vector <HNS_PixelOut> pixels_out;
    int pen_width = 3;

    if((f_signboard.fGetBoardsTall() > 0)
    && (f_signboard.fGetBoardsWide() > 0))
    {
        if(f_signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
        {
            pen_width = 3;
        }
        else
        {
            pen_width = 9;
        }
//        if(f_signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
//        {
            QVector<QRect> boards(f_signboard.fGetBoardsTall() * f_signboard.fGetBoardsWide());
            QVector<QVector <QRect> > pixels;
            pixels.resize(f_signboard.fGetBoardsTall() * f_signboard.fGetBoardsWide());
            for(int i = 0;i<pixels.size();i++)
            {
                pixels[i].resize(f_signboard.fGetLedsPerByte() * f_signboard.fGetNumBytes());
            }
            int index1,index2,pix_index1,pix_index2;
            painter.setPen(QPen(Qt::white,pen_width));

            for(size_t ui=1;ui<f_signboard.fGetBoardsWide();ui++)
            {
                x = (ui * size().width()) / f_signboard.fGetBoardsWide();
                painter.drawLine(x,0,x,size().height());

                for(size_t uj=0;uj<f_signboard.fGetBoardsTall();uj++)
                {
                    index1 = uj * f_signboard.fGetBoardsWide() + (ui-1);
                    index2 = index1 + 1;

                    if(ui == 1) //left edge
                    {
                        boards[index1].setLeft(0);
                    }
                    //bottom edge
                    if(ui == (f_signboard.fGetBoardsWide() - 1))
                    {
                        boards[index2].setRight(size().width());
                    }

                    boards[index1].setRight(x-(pen_width/2));
                    boards[index2].setLeft(x+(pen_width/2));
                }
            }

            for(size_t ui=1;ui<f_signboard.fGetBoardsTall();ui++)
            {
                y = (ui * size().height()) / f_signboard.fGetBoardsTall();
                painter.drawLine(0,y,size().width(),y);

                for(size_t uj = 0; uj < f_signboard.fGetBoardsWide(); uj++)
                {
                    index1 = uj + ((ui-1)*f_signboard.fGetBoardsWide());
                    index2 = index1 + f_signboard.fGetBoardsWide();

                    //top edge
                    if(ui == 1)
                    {
                        boards[index1].setTop(0);
                    }
                    if(ui == (f_signboard.fGetBoardsTall() - 1))
                    {
                        boards[index2].setBottom(size().height());
                    }
                    boards[index1].setBottom(y-(pen_width/2));
                    boards[index2].setTop(y+(pen_width/2));
                }
            }

            painter.setPen(QPen(Qt::white,1));

            //draw horizontal lines for individual pixels
            for(size_t ui=0;ui<f_signboard.fGetBoardsTall();ui++)
            {
                index1 = ui * f_signboard.fGetBoardsWide();
                if(f_signboard.fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
                {
                    for(size_t uj=1;uj<f_signboard.fGetNumBytes();uj++)
                    {
                        y = ((boards[index1].height() * uj) / f_signboard.fGetNumBytes()) + boards[index1].top();
                        painter.drawLine(0,y,size().width(),y);
                        for(size_t uk = 0;uk<f_signboard.fGetBoardsWide();uk++)
                        {
                            index2 = index1 + uk;
                            for(size_t ul=0;ul<f_signboard.fGetLedsPerByte();ul++)
                            {
                                pix_index1 = ((uj-1) * f_signboard.fGetLedsPerByte()) + ul;
                                pix_index2 = pix_index1 + f_signboard.fGetLedsPerByte();
                                //top edge
                                if(uj == 1)
                                {
                                    pixels[index2][pix_index1].setTop(boards[index2].top());
                                }
                                //bottom edge
                                if(uj == (f_signboard.fGetNumBytes() - 1))
                                {
                                    pixels[index2][pix_index2].setBottom(boards[index2].bottom());
                                }
                                pixels[index2][pix_index1].setBottom(y);
                                pixels[index2][pix_index2].setTop(y);
                            }
                        }
                    }
                }
                else
                {
                    for(size_t uj=1;uj<f_signboard.fGetLedsPerByte();uj++)
                    {
                        y = ((boards[index1].height() * uj) / f_signboard.fGetLedsPerByte()) + boards[index1].top();
                        painter.drawLine(0,y,size().width(),y);
                        for(size_t uk = 0;uk<f_signboard.fGetBoardsWide();uk++)
                        {
                            index2 = index1 + uk;
                            for(size_t ul=0;ul<f_signboard.fGetNumBytes();ul++)
                            {
                                pix_index1 = ((uj-1) * f_signboard.fGetNumBytes()) + ul;
                                pix_index2 = pix_index1 + f_signboard.fGetNumBytes();
                                //top edge
                                if(uj == 1)
                                {
                                    pixels[index2][pix_index1].setTop(boards[index2].top());
                                }
                                //bottom edge
                                if(uj == (f_signboard.fGetLedsPerByte() - 1))
                                {
                                    pixels[index2][pix_index2].setBottom(boards[index2].bottom());
                                }
                                pixels[index2][pix_index1].setBottom(y);
                                pixels[index2][pix_index2].setTop(y);
                            }
                        }
                    }
                }
            }

            //Draw vertical lines for individual pixels
            for(size_t ui = 0; ui<f_signboard.fGetBoardsWide();ui++)
            {
                index1 = ui;
                if(f_signboard.fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
                {
                    for(size_t uj = 1;uj<f_signboard.fGetLedsPerByte();uj++)
                    {
                        x = ((boards[index1].width() * uj) / f_signboard.fGetLedsPerByte()) + boards[index1].left();
                        painter.drawLine(x,0,x,size().height());

                        for(size_t uk = 0;uk<f_signboard.fGetBoardsTall();uk++)
                        {
                            index2 = index1 + (uk * f_signboard.fGetBoardsWide());
                            for(size_t ul = 0;ul < f_signboard.fGetNumBytes();ul++)
                            {
                                pix_index1 = (ul * f_signboard.fGetLedsPerByte()) + (uj-1);
                                pix_index2 = pix_index1 + 1;

                                //left edge
                                if(uj == 1)
                                {
                                    pixels[index2][pix_index1].setLeft(boards[index2].left());
                                }
                                //right edge
                                if(uj == (f_signboard.fGetLedsPerByte() -1))
                                {
                                    pixels[index2][pix_index2].setRight(boards[index2].right());
                                }
                                pixels[index2][pix_index1].setRight(x);
                                pixels[index2][pix_index2].setLeft(x);
                            }
                        }
                    }
                }
                else
                {
                    for(size_t uj = 1; uj<f_signboard.fGetNumBytes();uj++)
                    {
                        x = ((boards[index1].width() * uj) / f_signboard.fGetNumBytes()) + boards[index1].left();
                        painter.drawLine(x,0,x,size().height());

                        for(size_t uk = 0;uk<f_signboard.fGetBoardsTall();uk++)
                        {
                            index2 = index1 + (uk * f_signboard.fGetBoardsWide());
                            for(size_t ul = 0;ul < f_signboard.fGetLedsPerByte();ul++)
                            {
                                pix_index1 = (ul * f_signboard.fGetNumBytes()) + (uj-1);
                                pix_index2 = pix_index1 + 1;

                                //left edge
                                if(uj == 1)
                                {
                                    pixels[index2][pix_index1].setLeft(boards[index2].left());
                                }
                                //right edge
                                if(uj == (f_signboard.fGetNumBytes() -1))
                                {
                                    pixels[index2][pix_index2].setRight(boards[index2].right());
                                }
                                pixels[index2][pix_index1].setRight(x);
                                pixels[index2][pix_index2].setLeft(x);
                            }
                        }
                    }
                }
            }

            size_t pixel_index;
            for(size_t ui=0;ui<f_signboard.fGetNumBoards();ui++)
            {
                pixels_out = f_signboard.fGetPixelsOut(ui);
                for(size_t uj=0;uj<pixels_out.size();uj++)
                {
                    if(f_signboard.fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
                    {
                        pixel_index = (pixels_out[uj].fByte() * f_signboard.fGetLedsPerByte()) + pixels_out[uj].fLED();
                    }
                    else
                    {
                        pixel_index = (pixels_out[uj].fLED() * f_signboard.fGetNumBytes()) + pixels_out[uj].fByte();
                    }

                    fDrawPixelOut(&painter,pixels[ui][pixel_index]);
                }
            }
//        }
//        else
//        {
//            painter.setPen(QPen(Qt::white,9));

//            for(size_t ui=1;ui<f_signboard.fGetBoardsWide();ui++)
//            {
//                x = (ui * size().width()) / f_signboard.fGetBoardsWide();
//                painter.drawLine(x,0,x,size().height());
//            }

//            for(size_t ui=1;ui<f_signboard.fGetBoardsTall();ui++)
//            {
//                y = (ui * size().height()) / f_signboard.fGetBoardsTall();
//                painter.drawLine(0,y,size().width(),y);
//            }
//        }
    }
}

void HNS_PixelOut_Display::fDrawPixelOut(QPainter *painter, const QRect &pixel)
{
    QPen remember_pen = painter->pen();

    painter->setPen(QPen(Qt::white,2));
    painter->drawLine(pixel.topLeft(),pixel.bottomRight());
    painter->drawLine(pixel.topRight(),pixel.bottomLeft());

    painter->setPen(remember_pen);
}
