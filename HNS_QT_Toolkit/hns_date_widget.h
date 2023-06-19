#ifndef HNS_DATE_WIDGET_H
#define HNS_DATE_WIDGET_H

#include <QWidget>

namespace Ui {
class HNS_Date_Widget;
}

class HNS_Date_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Date_Widget(QWidget *parent = nullptr);
    ~HNS_Date_Widget();

private:
    Ui::HNS_Date_Widget *ui;

    QTimer *f_timer;
private slots:
    void fTickTock();
};

#endif // HNS_DATE_WIDGET_H
