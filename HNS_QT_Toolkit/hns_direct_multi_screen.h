#ifndef HNS_DIRECT_MULTI_SCREEN_H
#define HNS_DIRECT_MULTI_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Direct_Multi_Screen;
}

class HNS_Direct_Multi_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Direct_Multi_Screen(QWidget *parent, HNS_Machine_Interface *machine, const QString &input);
    ~HNS_Direct_Multi_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

    QString fGetText() const;

signals:
    void SIG_Direct_MULTI_Result(const QString &multi);

private slots:
    void on_PB_OK_clicked();

    void on_PB_Cancel_clicked();

    void on_PTE_Input_textChanged();

private:
    Ui::HNS_Direct_Multi_Screen *ui;

    type_multi_syntax_error f_current_multi_error;

    bool f_confirm;

    int f_max_num_pages;
};

#endif // HNS_DIRECT_MULTI_SCREEN_H
