#ifndef HNS_TIME_BUTTON_H
#define HNS_TIME_BUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QTime>

#include <hns_time_input_stream.h>

class HNS_Time_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit HNS_Time_Button(QWidget *parent = nullptr);

    void fClear();

    void fResetText();

    void fSetEditMode(const bool &edit_mode, const bool &accept = true);
    bool fGetEditMode() const {return f_editing_enabled;}

    void fToggleAMPM();

    void fSetTime(const QTime &time);

    QTime fGetTime();

    void fSetDefaultLabel(const QString &label);

public slots:
    void my_slot_key_input(int key);

    void my_slot_ampm_toggle();

private slots:
    void my_slot_TikTok();

private:
    bool eventFilter(QObject *obj, QEvent *event);

    QTime fGetTimeStream();

    HNS_Time_Input_Stream f_time_stream;
    QTime f_time;

    QTimer *f_timer;

    bool f_editing_enabled;

    bool f_blink_on;

    QString f_default_label;
};

#endif // HNS_TIME_BUTTON_H
