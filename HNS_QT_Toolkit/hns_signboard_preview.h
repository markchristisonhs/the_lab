#ifndef HNS_SIGNBOARD_PREVIEW_H
#define HNS_SIGNBOARD_PREVIEW_H

#include <QWidget>
#include <MessageBoard.h>

namespace Ui {
class HNS_Signboard_Preview;
}

class HNS_Signboard_Preview : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Signboard_Preview(QWidget *parent = nullptr);
    ~HNS_Signboard_Preview();


    void fSetSignBoard(const HNS_SignBoard &signboard);
    QImage fGetContents();
private:
    Ui::HNS_Signboard_Preview *ui;

    HNS_SignBoard f_signboard;
    QImage f_image;
    QVector <QImage> f_images;
    //This is an image to be returned later, that shows this widget's contents
    QImage f_display;

    void fUpdate();

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // HNS_SIGNBOARD_PREVIEW_H
