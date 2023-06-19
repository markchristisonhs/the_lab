#ifndef HNS_MESSAGE_EDIT_SCREEN_H
#define HNS_MESSAGE_EDIT_SCREEN_H

#include <QWidget>
#include <HNS_UI_Widget.h>
#include <vector>
#include <fonts.h>
#include <graphics.h>
#include "hns_vkb.h"
#include "hns_vkp.h"

typedef enum
{
    HNS_EDIT_NORMAL = 0,
    HNS_EDIT_PICKER,
    HNS_EDIT_EXISTING
} type_edit_mode;

namespace Ui {
class HNS_Message_Edit_Screen;
}

class HNS_Message_Edit_Screen : public HNS_UI_Widget
{
    Q_OBJECT

public:
    HNS_Message_Edit_Screen(QWidget *parent, HNS_Machine_Interface *machine, const type_edit_mode &edit_mode, const QString &multi = "");
    ~HNS_Message_Edit_Screen();

    type_widget fGetType() const;
    void fSetAccess(const type_user_level &access);

    void fUpdate(const int &update_flags = HNS_DATA_ALL);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void SIG_Edit_Result(const QString &multi);

private slots:
    void on_PB_Edit_Back_clicked();

    void on_PB_Edit_Cancel_clicked();

    void on_PB_Edit_Preview_clicked();

    void on_PB_Edit_FontOptions_clicked();

    void on_PB_Edit_AddMessage_clicked();

    void on_PB_Edit_AddStaticImage_clicked();

    void on_PB_Edit_AddAnimatedImage_clicked();

    void on_PB_Edit_Direct_MULTI_clicked();

    void on_PB_Edit_LeftJustify_clicked();

    void on_PB_Edit_CenterLineJustify_clicked();

    void on_PB_Edit_RightJustify_clicked();

    void on_PB_Edit_TopJustify_clicked();

    void on_PB_Edit_CenterPageJustify_clicked();

    void on_PB_Edit_BottomJustify_clicked();

    void on_PB_Edit_InsertRadarSpeed_clicked();

    void on_PB_Edit_FlashText_clicked();

    void my_slot_Direct_Multi_Finished(const QString &multi);

    void my_slot_Font_Selected(const int &font_select, const bool &apply_all);

    void my_slot_Preview_Accepted();

    void my_slot_Preview_Add_Library();

    void on_PB_Edit_AddPage_clicked();

    void on_PB_Edit_RemovePage_clicked();

    void on_PB_Edit_Prev_clicked();

    void on_PB_Edit_Next_clicked();

    void my_slot_Edit_Flash_Changed();

    void GV_Edit_LineJustificationChanged(const type_justification_line &line_justification);

    void GV_Edit_PageJustificationChanged(const type_justification_page &page_justification);

    void my_slot_EditPreview_NewPageCreated();

    void my_slot_Message(const QString &multi, const HNS_NTCIP_MessageIDCode &id_code, const type_ui_message_mode &mode);

private:
    Ui::HNS_Message_Edit_Screen *ui;

    type_edit_mode f_edit_mode;
    double f_current_pageoff_time, f_current_pageon_time;

    void on_LE_Edit_PageOffTime_clicked();
    void on_LE_Edit_PageOnTime_clicked();

    void LE_Edit_PageOnTime_editingFinished(const bool &enter_pressed);
    void LE_Edit_PageOffTime_editingFinished(const bool &enter_pressed);

    void fVKP_Finished(const bool &);
};

#endif // HNS_MESSAGE_EDIT_SCREEN_H
