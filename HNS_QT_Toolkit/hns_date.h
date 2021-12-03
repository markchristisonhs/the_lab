#ifndef HNS_DATE_H
#define HNS_DATE_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class HNS_Date;
}

class HNS_Date : public QLineEdit
{
    Q_OBJECT

public:
    explicit HNS_Date(QWidget *parent = nullptr);
    explicit HNS_Date(const QString &contents, QWidget *parent = nullptr);
    ~HNS_Date();

    void fClear();

private slots:

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::HNS_Date *ui;

    void fSetDateString();

    QString fCreateDateString(const int &month, const int &day, const int &year);

    bool eventFilter(QObject *obj, QEvent *event);

    void fKeyPressFSM(const int &key);

    Q_INVOKABLE void resetCursor(QWidget *w) {static_cast<QLineEdit *>(w)->setCursorPosition(f_cursor_position);}

    int f_month,f_day,f_year; //year stored in 00-99 format, -1 for all fields means no date set and that field should be blank

    int f_cursor_position;
};

#endif // HNS_DATE_H
