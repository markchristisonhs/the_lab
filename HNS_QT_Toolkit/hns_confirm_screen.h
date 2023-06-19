#ifndef HNS_CONFIRM_SCREEN_H
#define HNS_CONFIRM_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Confirm_Screen;
}

class HNS_Confirm_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Confirm_Screen(QWidget *parent, HNS_Machine_Interface *machine, const QString &title, const QString &confirm_label = "CONFIRM", const QString &cancel_label = "CANCEL");
    ~HNS_Confirm_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

private slots:
    void on_PB_Cancel_clicked();

    void on_PB_Confirm_clicked();

signals:
    void SIG_Confirm_Closed(const bool &confirmed);

private:
    Ui::HNS_Confirm_Screen *ui;
};

#endif // HNS_CONFIRM_SCREEN_H
