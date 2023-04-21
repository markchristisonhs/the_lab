#ifndef HNS_DATE_BUTTON_H
#define HNS_DATE_BUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QDate>

#include <hns_date_input_stream.h>


class HNS_Date_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit HNS_Date_Button(QWidget *parent = nullptr);

    void fClear();

    void fResetText();

    void fSetEditMode(const bool &edit_mode);

    bool fGetEditMode() const {return f_editing_enabled;}

    void fSetDate(const QDate &date);

    QDate fGetDate();

    void fSetDefaultLabel(const QString &label);

public slots:
    void my_slot_key_input(int key);

    void my_slot_TikTok();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    QDate fGetDateStream();

    HNS_Date_Input_Stream f_date_stream;
    QDate f_date;

    QTimer *f_timer;

    bool f_editing_enabled;

    bool f_blink_on;

    QString f_default_label;
};

#endif // HNS_DATE_BUTTON_H
