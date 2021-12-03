#ifndef HNS_PIXELOUT_DISPLAY_H
#define HNS_PIXELOUT_DISPLAY_H

#include <QWidget>
#include <MessageBoard.h>

namespace Ui {
class HNS_PixelOut_Display;
}

class HNS_PixelOut_Display : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_PixelOut_Display(QWidget *parent = nullptr);
    ~HNS_PixelOut_Display();

    void fSetSignBoard(const HNS_SignBoard &board);

private:
    Ui::HNS_PixelOut_Display *ui;

    HNS_SignBoard f_signboard;

    void fDrawPixelOut(QPainter *painter, const QRect &pixel);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // HNS_PIXELOUT_DISPLAY_H
