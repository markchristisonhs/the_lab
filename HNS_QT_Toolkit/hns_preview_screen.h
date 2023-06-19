#ifndef HNS_PREVIEW_SCREEN_H
#define HNS_PREVIEW_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

typedef enum
{
    HNS_PREVIEW_NORMAL = 0,
    HNS_PREVIEW_PICKER
} type_preview_mode;

namespace Ui {
class HNS_Preview_Screen;
}

class HNS_Preview_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Preview_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_preview_mode &preview_mode, const QString &multi);
    ~HNS_Preview_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

signals:
    void SIG_Preview_Accepted();

    void SIG_Preview_Add_Library();

private slots:
    void on_PB_Preview_SaveToLibrary_clicked();

    void on_PB_Preview_Run_clicked();

    void on_PB_Preview_Schedule_clicked();

    void on_PB_Preview_Edit_clicked();

    void on_PB_Preview_Back_clicked();

//    void fTickTock();

private:
    Ui::HNS_Preview_Screen *ui;

//    QTimer *f_timer;
};

#endif // HNS_PREVIEW_SCREEN_H
