#ifndef HNS_CLOCK_WIDGET_H
#define HNS_CLOCK_WIDGET_H

#include <QWidget>

namespace Ui {
class HNS_Clock_Widget;
}

class HNS_Clock_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Clock_Widget(QWidget *parent = nullptr, const QString &timestring = "h:mm AP");
    ~HNS_Clock_Widget();

private:
    Ui::HNS_Clock_Widget *ui;

    QTimer *f_timer;
    QString f_timestring;
private slots:
    void fTickTock();
};

#endif // HNS_CLOCK_WIDGET_H
