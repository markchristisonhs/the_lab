#ifndef HNS_TEST_PATTERN_SCREEN_H
#define HNS_TEST_PATTERN_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>

namespace Ui {
class HNS_Test_Pattern_Screen;
}

class HNS_Test_Pattern_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    explicit HNS_Test_Pattern_Screen(QWidget *parent, HNS_Machine_Interface *machine);
    ~HNS_Test_Pattern_Screen();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

private slots:
    void on_PB_TestPattern_Run_clicked();

    void on_PB_TestPattern_Cancel_clicked();

    void my_slot_TestPattern1_clicked(QMouseEvent *event);
    void my_slot_TestPattern2_clicked(QMouseEvent *event);

private:
    Ui::HNS_Test_Pattern_Screen *ui;

    void fSetTestPatternSelection(const int &select);
    int fGetTestPatternSelection();
};

#endif // HNS_TEST_PATTERN_SCREEN_H
