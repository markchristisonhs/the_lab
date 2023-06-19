#ifndef HNS_TRAILER_SCREEN_H
#define HNS_TRAILER_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Trailer_Screen;
}

class HNS_Trailer_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Trailer_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Trailer_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_TrailerSet_Save_clicked();

    void on_PB_TrailerSet_Cancel_clicked();

    void on_PB_TrailerSet_Back_clicked();

    void on_PB_TrailerSet_Trailer1_clicked();

    void on_PB_TrailerSet_Trailer2_clicked();

    void on_PB_TrailerSet_Trailer3_clicked();

    void on_PB_TrailerSet_Trailer4_clicked();

    void on_PB_TrailerSet_Trailer5_clicked();

    void on_PB_TrailerSet_Trailer6_clicked();

    void on_PB_TrailerSet_Trailer7_clicked();

    void on_PB_TrailerSet_Trailer8_clicked();

    void on_PB_TrailerSet_Trailer9_clicked();

    void on_PB_TrailerSet_Trailer10_clicked();

private:
    void fSelectTrailer(const int &select);
    int fGetSelectedTrailer() const;

    Ui::HNS_Trailer_Screen *ui;
};

#endif // HNS_TRAILER_SCREEN_H
