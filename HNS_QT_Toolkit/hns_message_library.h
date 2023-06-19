#ifndef HNS_MESSAGE_LIBRARY_H
#define HNS_MESSAGE_LIBRARY_H

#include <QWidget>
#include <HNS_UI_Widget.h>

typedef enum
{
    HNS_MSGLIB_MESSAGES = 0,
    HNS_MSGLIB_STATIC,
    HNS_MSGLIB_ANIMATED,
    HNS_MSGLIB_COMBINED
} type_MsgLib_Mode;

namespace Ui {
class HNS_Message_Library;
}

class HNS_Message_Library : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Message_Library(QWidget *parent, HNS_Machine_Interface *machine, const type_MsgLib_Mode &lib_mode);
    ~HNS_Message_Library();

    void fSetAccess(const type_user_level &access);

    type_widget fGetType() const;

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

private slots:
    void on_PB_MsgLib_Back_clicked();

    void my_slot_GV_MessageLibrary_Preview1_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview2_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview3_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview4_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview5_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview6_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview7_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview8_clicked(QMouseEvent *event);
    void my_slot_GV_MessageLibrary_Preview9_clicked(QMouseEvent *event);

    void on_PB_MsgLib_Prev_clicked();

    void on_PB_MsgLib_Next_clicked();

    void on_PB_MsgLib_InsSelected_clicked();

    void on_PB_MsgLib_Pick2_clicked();

    void on_PB_MsgLib_Pick1_clicked();

    void on_PB_MsgLib_Pick3_clicked();

    void on_PB_MsgLib_Custom_clicked();

    void on_PB_MsgLib_Fact_clicked();

    void on_PB_MsgLib_Del_All_clicked();

    void on_PB_MsgLib_Del_clicked();

    void my_slot_Del_All_confirmation(const bool &confirmed);

    void on_LE_Search_textEdited(const QString &arg1);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void swipeTriggred(const QPoint &startPos, const QPoint &endPos);

    void fSelectPreview(const int &index);

    Ui::HNS_Message_Library *ui;

    type_MsgLib_Mode f_msglib_mode;

    int f_messagelibrary_page;
    std::string f_searchString;
    bool f_factoryChecked;
    bool f_customChecked;
    int f_selectedCustMsgIndex;
    QPoint f_startPos;
    QPoint f_endPos;

    void fSetLibrarySelection(const int &select);
    int fGetLibrarySelection(HNS_Message2 *message = nullptr, HNS_NTCIP_MessageActivationCode *act_code = nullptr);
    void fSetMsgLibMode(const type_MsgLib_Mode &mode);
};

#endif // HNS_MESSAGE_LIBRARY_H
