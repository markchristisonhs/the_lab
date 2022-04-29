#ifndef HNS_MESSAGE_PREVIEW_H
#define HNS_MESSAGE_PREVIEW_H

#include <QWidget>
#include <message.h>
#include <fonts.h>
#include <vector>
#include <QImage>
#include <HNS_NTCIP_Datatypes.h>
#include <MessageBoard.h>

//MSC20210402 Included ability to insert messages, in addition to adding them to the end.

typedef enum
{
    HNS_PREVIEW_ON = 0,
    HNS_PREVIEW_OFF
} type_preview_state;

namespace Ui {
class HNS_Message_Preview;
}

class HNS_Message_Preview : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Message_Preview(QWidget *parent = nullptr);
    ~HNS_Message_Preview();

    void fSetMessage(const HNS_Message2 &message, const bool &do_update = true);
    void fSetMessage(const QString &multi, const bool &do_update = true);
    bool fUpdate(const bool &refresh_pages = false, const bool &blank = false);

    void fSetActivationCode(const HNS_NTCIP_MessageActivationCode &activation_code);

    //SetMessage overrides the existing message.  AddMessage appends the incoming message to the end.
    void fAddMessage(const QString &multi, const bool &do_update = true);

    //Inserts a message after pos.  If pos is -1, then it inserts at current position.  If the page at the current position is empty, the first page of multi replaces it.
    //returns the new current page, which in fact might not be changed
    int fInsertMessage(const QString &multi, const int &pos = -1, const bool &do_update = true);

    //Where Add message assumes the string represents new pages, this adds text to the indicated page, or if that is -1, the currently selected page
    void fAddText(const QString &text, const int &page = -1, const bool &do_update = true);

    void fSetFonts(const std::vector<HNS_Font> &fonts);
    void fSetGraphics(const std::vector<HNS_Graphic> &graphics);

    void fSetLineJustification(const type_justification_line &line_justification);
    void fSetPageJustification(const type_justification_page &page_justification);

    //page == -1 indicates to insert a page after currently selected page
    //otherwise insert a new page after indicated page.
    void fAddPage(const int &page = -1);

    //same as above, -1 indicates to remove currently selected page
    //otherwise remove the page specified
    void fRemovePage(const int &page = -1);

    //selects next page for display if page is available
    void fNextPage();
    //selects prior page for display if page is available
    void fPrevPage();

    void fSelectPage(const int &page);

    int fGetCurrentPage() const;

    //page == -1 means to set the page time for the currently selected page.
    void fSetPageTimes(const double &pagetime_on, const double &pagetime_off, const int &page = -1);

    void fGenerateFontTest(const size_t &font_no);

    void fSelect(const bool &selected);
    bool fIsSelected() const;

    void fChangeFont(const int &font_no, const bool &all = false);
    int fTotalPage() const;

    HNS_Message2 fGetCurrentMessage() const;
    HNS_NTCIP_MessageActivationCode fGetActivationCode() const;

    void fClear();

    //flip time is the time that it flips from one message to the next
    void fStartPreview(const qint64 &time, const int &flip_time = 1000);
    void fTickTock(const qint64 &time);


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void keyPressEvent(QKeyEvent *event);

signals:
    void fLineJustificationChanged(const type_justification_line &line_justification);
    void fPageJustificationChanged(const type_justification_page &page_justification);
    void fClicked(QMouseEvent *event);

    void fNewPageCreated();

private:
    bool fIsNewPageNeeded(const QString &input);

    Ui::HNS_Message_Preview *ui;

    HNS_Message2 f_current_message;
    HNS_NTCIP_MessageActivationCode f_activation_code;
    QVector<QString> f_page_strings;

    int f_current_page;

    std::vector<HNS_Font> f_fonts;
    std::vector<HNS_Graphic> f_graphics;
    HNS_SignBoard f_signboard;

    QString fMergePages();
    QVector<QString> fSplitPages(const QString &multi_input);

    QImage f_image;
    QVector <QImage> f_images;

    bool f_selected;

    qint64 f_last_update;
    type_preview_state f_preview_state;
    int f_flip_time;
};

#endif // HNS_MESSAGE_PREVIEW_H
