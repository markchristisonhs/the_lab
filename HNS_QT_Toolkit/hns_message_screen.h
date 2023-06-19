#ifndef HNS_MESSAGE_SCREEN_H
#define HNS_MESSAGE_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Message_Screen;
}

class HNS_Message_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Message_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Message_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

public slots:
    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

signals:
    void SIG_Run_Message(const QString &multi);

private slots:
    void on_PB_Message_Create_clicked();

    void on_PB_Message_OpenLibrary_clicked();

    void on_PB_Message_Back_clicked();

private:
    Ui::HNS_Message_Screen *ui;
};

#endif // HNS_MESSAGE_SCREEN_H
