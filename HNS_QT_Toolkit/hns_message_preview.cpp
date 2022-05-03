#include "hns_message_preview.h"
#include "ui_hns_message_preview.h"
#include "hns_qt_toolkit.h"

#include <QPainter>
#include <QKeyEvent>

using namespace std;

HNS_Message_Preview::HNS_Message_Preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_Message_Preview)
  , f_page_strings(1,"")
  , f_current_page(0)
  , f_selected(false)
  , f_last_update(0)
  , f_preview_state(HNS_PREVIEW_ON)
{
    ui->setupUi(this);
}

HNS_Message_Preview::~HNS_Message_Preview()
{
    delete ui;
}

void HNS_Message_Preview::fSetMessage(const HNS_Message2 &message, const bool &do_update)
{
    QString tempstring;
    f_current_message = message;
    tempstring = QString::fromStdString(f_current_message.fGetMULTI());

    f_page_strings = fSplitPages(tempstring);

    f_current_page = 0;

    if(do_update)
    {
        fUpdate(false);
    }
}

void HNS_Message_Preview::fSetMessage(const QString &multi, const bool &do_update)
{
    int error = 0;
    type_multi_syntax_error multi_error = HNS_MULTI_SYNTAX_ERROR_NONE;
    string tempstring = multi.toStdString();
    HNS_Message2 temp_message;
    error = temp_message.fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr,&multi_error);
    f_current_message = temp_message;

    f_page_strings = fSplitPages(multi);

    f_current_page = 0;

    if(do_update)
    {
        fUpdate(false);
    }
}

void HNS_Message_Preview::fSetActivationCode(const HNS_NTCIP_MessageActivationCode &activation_code)
{
    f_activation_code = activation_code;
}

//MSC20210402 Prevent blank pages from being erroneously added.
void HNS_Message_Preview::fAddMessage(const QString &multi, const bool &do_update)
{
    QVector<QString> temp_pages = fSplitPages(multi);

    if(f_page_strings.size() == 1)
    {
        if(f_page_strings[0].isEmpty())
        {
            f_page_strings.clear();
        }
    }

    if(f_page_strings.size() == 0)
    {
        f_page_strings.append(temp_pages);
    }
    else
    {
        if(f_page_strings[f_page_strings.size()-1].isEmpty())
        {
            f_page_strings.erase(f_page_strings.end());
        }
        f_page_strings.append(temp_pages);
    }

    //MSC20211220 I THINK that page strings being size 0 at this point is impossible, but just in case...
    if(f_page_strings.size() <= 0)
    {
        f_page_strings = QVector<QString>(1,"");
    }

    if(do_update)
    {
        fUpdate(true);
    }
}

int HNS_Message_Preview::fInsertMessage(const QString &multi, const int &pos, const bool &do_update)
{
    QVector<QString> temp_pages = fSplitPages(multi);
    int index = pos;
    if(pos == -1)
    {
        index = f_current_page;
    }

    if(index >= 0 && index < f_page_strings.size())
    {
        //insert at end.
        if(index == f_page_strings.size()-1)
        {
            if(f_page_strings[index].isEmpty())
            {
                f_page_strings.remove(index);
            }
            else
            {
                f_current_page = index + 1;
            }
            f_page_strings.append(temp_pages);
        }
        //insert somewhere else
        else
        {
            for(int i=(temp_pages.size()-1);i>=0;i--)
            {
                f_page_strings.insert(index+1,temp_pages[i]);
            }
            if(f_page_strings[index].isEmpty())
            {
                f_page_strings.remove(index);
                f_current_page = index;
            }
            else
            {
                f_current_page = index+1;
            }
        }
        if(do_update)
        {
            fUpdate(true);
        }
    }

    return f_current_page;
}

void HNS_Message_Preview::fAddText(const QString &text, const int &page, const bool &do_update)
{
    if(page == -1)
    {
        f_page_strings[f_current_page] += text;

    }
    else
    {
        if(page >= 0 && page < f_page_strings.size())
        {
            f_page_strings[page] += text;
        }
    }
    if(do_update)
    {
        fUpdate(true);
    }
}

void HNS_Message_Preview::fSetFonts(const vector<HNS_Font> &fonts)
{
    f_fonts = fonts;
}

void HNS_Message_Preview::fSetGraphics(const vector<HNS_Graphic> &graphics)
{
    f_graphics = graphics;
}

void HNS_Message_Preview::fSetLineJustification(const type_justification_line &line_justification)
{
    QString tag,current_message = f_page_strings[f_current_page];

    switch(line_justification)
    {
    case HNS_JUSTIFICATION_LEFT:
        tag = "[jl2]";
        break;
    case HNS_JUSTIFICATION_LINE_CENTER:
        tag = "[jl3]";
        break;
    case HNS_JUSTIFICATION_RIGHT:
        tag = "[jl4]";
        break;
    case HNS_JUSTIFICATION_FULL:
        tag = "[jl5]";
        break;
    }

    if(IsTagAtEnd(current_message).contains("jl"))
    {
        current_message = RemoveTagAtEnd(current_message);
    }
    current_message += tag;

    f_page_strings[f_current_page] = current_message;
    std::string tempstring = fMergePages().toStdString();
    f_current_message.fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr);
}

void HNS_Message_Preview::fSetPageJustification(const type_justification_page &page_justification)
{
    QString tag,current_message = f_page_strings[f_current_page];

    switch(page_justification)
    {
    case HNS_JUSTIFICATION_BOTTOM:
        tag = "[jp4]";
        break;
    case HNS_JUSTIFICATION_PAGE_CENTER:
        tag = "[jp3]";
        break;
    case HNS_JUSTIFICATION_TOP:
        tag = "[jp2]";
        break;
    }

    if(IsTagAtEnd(current_message).contains("jp"))
    {
        current_message = RemoveTagAtEnd(current_message);
    }
    current_message += tag;

    f_page_strings[f_current_page] = current_message;
    fUpdate();
}

void HNS_Message_Preview::fAddPage(const int &page)
{
    if(page == -1)
    {
        f_page_strings.insert(f_current_page+1,"");
        f_current_page++;
    }
    else
    {
        if(page < f_page_strings.size())
        {
            f_page_strings.insert(page+1,"");
        }
    }

    fUpdate();
}

void HNS_Message_Preview::fRemovePage(const int &page)
{
    if(page == -1)
    {
        f_page_strings.remove(f_current_page);
    }
    else
    {
        f_page_strings.remove(page);
    }

    if(f_page_strings.size() == 0)
    {
        f_page_strings.resize(1);
    }

    if(f_current_page >= f_page_strings.size())
    {
        if(f_current_page > 0)
        {
            f_current_page--;
        }
        else
        {
            f_current_page = 0;
        }
    }

    fUpdate(true);
}

void HNS_Message_Preview::fNextPage()
{
    if(f_current_page < (f_page_strings.size() - 1))
    {
        f_current_page++;
    }

    fUpdate();
}

void HNS_Message_Preview::fPrevPage()
{
    if(f_current_page > 0)
    {
        f_current_page--;
    }

    fUpdate();
}

void HNS_Message_Preview::fSelectPage(const int &page)
{
    if((page >= 0) && page < (f_page_strings.size()))
    {
        f_current_page  = page;
    }

    fUpdate(false);
}

int HNS_Message_Preview::fGetCurrentPage() const
{
    return f_current_page;
}

void HNS_Message_Preview::fGenerateFontTest(const size_t &font_no)
{
    f_current_message.fSetFontTest(&f_fonts,font_no);

    fUpdate(false);
}

void HNS_Message_Preview::fSelect(const bool &selected)
{
    f_selected = selected;

    fUpdate(false);
}

bool HNS_Message_Preview::fIsSelected() const
{
    return f_selected;
}

void HNS_Message_Preview::fChangeFont(const int &font_no, const bool &all)
{
    QString tag = QString("[fo%1]").arg(font_no);
    if(all)
    {
        for(int i = 0;i<f_page_strings.size();i++)
        {
            while(IsTagInString(f_page_strings[i],"fo") >= 0)
            {
                f_page_strings[i] = RemoveTag(f_page_strings[i],"fo");
            }
        }
        if(f_page_strings.size() == 0)
        {
            f_page_strings.push_back(tag);
        }
        else
        {
            f_page_strings[0] = tag + f_page_strings[0];
        }
    }
    else
    {
        if(IsTagAtEnd(f_page_strings[f_current_page]).contains("fo",Qt::CaseInsensitive))
        {
            RemoveTagAtEnd(f_page_strings[f_current_page]);
        }
        f_page_strings[f_current_page] = f_page_strings[f_current_page] + tag;
    }
    fUpdate(true);
}

int HNS_Message_Preview::fTotalPage() const
{
    return f_current_message.fGetNumPages();
}

HNS_Message2 HNS_Message_Preview::fGetCurrentMessage() const
{
    return f_current_message;
}

HNS_NTCIP_MessageActivationCode HNS_Message_Preview::fGetActivationCode() const
{
    return f_activation_code;
}

void HNS_Message_Preview::fClear()
{
    f_page_strings.clear();
    f_page_strings.push_back("");
    f_current_page = 0;
    fUpdate(true);
}

void HNS_Message_Preview::fStartPreview(const qint64 &time, const int &flip_time)
{
    f_last_update = time;
    f_current_page = 0;
    f_preview_state = HNS_PREVIEW_ON;
    f_flip_time = flip_time;
    fUpdate(false);
}

void HNS_Message_Preview::fTickTock(const qint64 &time)
{
    if(f_current_message.fGetNumPages() > 1)
    {
        switch(f_preview_state)
        {
        case HNS_PREVIEW_ON:
            //if more than 3 seconds has passed update the display
            if((time-f_last_update) > f_flip_time)
            {
                f_last_update = time;
                if(f_current_message.fGetPageTimeOff(f_current_page) > 0.0)
                {
                    fUpdate(false,true);
                    f_preview_state = HNS_PREVIEW_OFF;
                }
                else
                {
                    f_current_page++;
                    if(static_cast<size_t>(f_current_page) == f_current_message.fGetNumPages())
                    {
                        f_current_page = 0;
                    }
                    fUpdate(false);
                }
            }
            break;
        case HNS_PREVIEW_OFF:
            if(time - f_last_update > 500)
            {
                f_last_update = time;

                f_current_page++;
                if(static_cast<size_t>(f_current_page) == f_current_message.fGetNumPages())
                {
                    f_current_page = 0;
                }
                f_preview_state = HNS_PREVIEW_ON;
                fUpdate(false);
            }
            break;
        }
    }
}

void HNS_Message_Preview::fSetPageTimes(const double &pagetime_on, const double &pagetime_off, const int &page)
{
    int selected_page = f_current_page;
    if(page >= 0)
    {
        if(page < f_page_strings.size())
        {
            selected_page = page;
        }
        else
        {
            return;
        }
    }
    QString tag,current_message = f_page_strings[selected_page];

    tag = QString("[pt%1o%2]").arg(static_cast<int>(pagetime_on*10.0)).arg(static_cast<int>(pagetime_off*10.0));

    if(IsTagInString(current_message,"pt") >= 0)
    {
        current_message = RemoveTag(current_message,"pt");
    }
    current_message = tag + current_message;

    f_page_strings[f_current_page] = current_message;
    fUpdate(false);
}

void HNS_Message_Preview::paintEvent(QPaintEvent */*event*/)
{
    static double width_ratio = 0.95;
    static double height_ratio = 0.95;
    size_t xpos,ypos,box_width,box_height;
    QPainter painter(this);
    painter.fillRect(0,0,size().width(),size().height(),QBrush(Qt::black,Qt::SolidPattern));
    HNS_SignBoard_Info signboard_info = HNS_Message2::fGetDefaultSignBoardInfo();

    if(signboard_info.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        if(!f_image.isNull())
        {
            QImage preview = f_image.scaled(size().width(),size().height());
            painter.drawImage(0,0,preview);
        }
    }
    else if(signboard_info.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        int left,right,top,bottom;
        size_t row,col;

        for(size_t i=0;i<signboard_info.fGetBoardsWide()-1;i++)
        {
            right = (((width() * width_ratio)/signboard_info.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(signboard_info.fGetBoardsWide() - 1))) * (i+1);
            left = right - ((width() * (1 - width_ratio))/(signboard_info.fGetBoardsWide() - 1));
            top = 0;
            bottom = height();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        for(size_t i=0;i<signboard_info.fGetBoardsTall()-1;i++)
        {
            bottom = (((height() * height_ratio)/signboard_info.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(signboard_info.fGetBoardsTall() - 1))) * (i+1);
            top = bottom - ((height() * (1 - height_ratio))/(signboard_info.fGetBoardsTall() - 1));
            left = 0;
            right = width();
            painter.fillRect(QRect(QPoint(left,top),QPoint(right,bottom)),QBrush(Qt::white,Qt::SolidPattern));
        }

        if(f_images.size() > 0)
        {

            for(size_t ui = 0; ui < signboard_info.fGetNumBoards(); ui++)
            {
                row = ui / signboard_info.fGetBoardsWide();
                col = ui % signboard_info.fGetBoardsWide();

                xpos = (((width() * width_ratio)/signboard_info.fGetBoardsWide()) + ((width() * (1 - width_ratio))/(signboard_info.fGetBoardsWide() - 1))) * col;
                ypos = (((height() * height_ratio)/signboard_info.fGetBoardsTall()) + ((height() * (1 - height_ratio))/(signboard_info.fGetBoardsTall() - 1))) * row;
                box_width = ((width() * width_ratio)/signboard_info.fGetBoardsWide());
                box_height = ((height() * height_ratio)/signboard_info.fGetBoardsTall());

                QImage preview = f_images[ui].scaled(box_width,box_height);
                painter.drawImage(xpos,ypos,preview);
            }
        }
    }

    if(f_selected)
    {
        QPen temp_pen(QColor(255,165,0));
        temp_pen.setWidth(15);
        painter.setPen(temp_pen);
        painter.drawRect(0,0,size().width(),size().height());
    }
}

void HNS_Message_Preview::mousePressEvent(QMouseEvent *event)
{
    emit fClicked(event);
}

void HNS_Message_Preview::keyPressEvent(QKeyEvent *event)
{
    QString temp_qstring;
    bool emit_line = false, emit_page = false;
    QChar last_char;
    int index;

    int temp_key = event->key();

    if(event->key() >= 0x01000000)
    {
        switch(event->key())
        {
        case Qt::Key_Backspace:
            if(!(temp_qstring = IsTagAtEnd(f_page_strings[f_current_page])).isEmpty())
            {
                if(temp_qstring.contains("[jp",Qt::CaseInsensitive) != 0)
                {
                    f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
                    emit_page = true;
                }
                else if(temp_qstring.contains("[jl",Qt::CaseInsensitive) != 0)
                {
                    f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
                    emit_line = true;
                }
                else if(temp_qstring.contains("[nl",Qt::CaseInsensitive) != 0)
                {
                    f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
                }
                else if(temp_qstring.contains("[fo",Qt::CaseInsensitive) != 0)
                {
                    f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
                    if(!f_page_strings[f_current_page].isEmpty() && IsTagAtEnd(f_page_strings[f_current_page]).isEmpty())
                    {
                        f_page_strings[f_current_page].chop(1);
                    }
                }
                else if(temp_qstring.contains("[f",Qt::CaseInsensitive) != 0)
                {
                    f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
                }
                else
                {
                    index = f_page_strings[f_current_page].size()-2;
                    last_char = f_page_strings[f_current_page][index];

                    if(last_char == "]")
                    {
                        f_page_strings[f_current_page].chop(2);
                    }
                }
            }
            else
            {
                index = f_page_strings[f_current_page].size()-1;
                if(index >= 0)
                {
                    last_char = f_page_strings[f_current_page][index];

                    if(last_char == '[' || last_char == ']')
                    {
                        f_page_strings[f_current_page].chop(2);
                    }
                    else
                    {
                        f_page_strings[f_current_page].chop(1);
                    }
                }
            }
            if(emit_line)
            {
                emit fLineJustificationChanged(f_current_message.fGetLastLineJustification(f_current_page));
            }
            if(emit_page)
            {
                emit fPageJustificationChanged(f_current_message.fGetLastPageJustification(f_current_page));
            }
            fUpdate(true);
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            f_page_strings[f_current_page] = f_page_strings[f_current_page] + QString("[nl]");
            fUpdate(true);
            break;
        }
    }
    else
    {
        if(event->key() == Qt::Key_Space)
        {
            f_page_strings[f_current_page] = f_page_strings[f_current_page] + QString(" ");
        }
        else if(event->key() == Qt::Key_BracketLeft || event->key() == Qt::Key_BracketRight)
        {
            if(event->key() == Qt::Key_BracketLeft)
            {
                f_page_strings[f_current_page] = f_page_strings[f_current_page] + "[[";
            }
            else
            {
                f_page_strings[f_current_page] = f_page_strings[f_current_page] + "]]";
            }
        }
        else
        {
            f_page_strings[f_current_page] = f_page_strings[f_current_page] + QString(event->key());
        }

        if(fIsNewPageNeeded(f_page_strings[f_current_page]))
        {
            QString tempstring = f_page_strings[f_current_page].right(1);
            QKeyEvent key_event(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
            keyPressEvent(&key_event);

            while(IsTagAtEnd(f_page_strings[f_current_page]).contains("NL",Qt::CaseInsensitive))
            {
                f_page_strings[f_current_page] = RemoveTagAtEnd(f_page_strings[f_current_page]);
            }

            fAddPage();

            int key = 0;
            if (tempstring.size() > 0)
            {
                key = tempstring[0].unicode();
            }
            if(key != 0)
            {
                key_event = QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier);
                keyPressEvent(&key_event);
            }
        }

        if(!fUpdate(true))
        {
            QKeyEvent key_event(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
            keyPressEvent(&key_event);
        }
    }
}

QString HNS_Message_Preview::fMergePages()
{
    QString result;

    int i;

    for(i=0;i<f_page_strings.size();i++)
    {
        result = result + f_page_strings[i];
        if(i<f_page_strings.size()-1)
        {
            result = result + "[np]";
        }
    }

    return result;
}

QVector<QString> HNS_Message_Preview::fSplitPages(const QString &multi_input)
{
    QVector<QString> result;
    QString tempstring = multi_input,tempstring2;

    do
    {
        if(tempstring.indexOf("[np",0,Qt::CaseInsensitive) >= 0)
        {
            tempstring2 = tempstring.left(tempstring.indexOf("[np",0,Qt::CaseInsensitive));
            result.push_back(tempstring2);
            tempstring = tempstring.remove(0,tempstring.indexOf("[np",0,Qt::CaseInsensitive));
            tempstring = tempstring.remove(0,tempstring.indexOf("]",0,Qt::CaseInsensitive)+1);
            if(tempstring.left(3) == "[np")
            {
                tempstring = "";
            }
        }
        else if(!tempstring.isEmpty())
        {
            result.push_back(tempstring);
            tempstring = "";
        }
    }while(!tempstring.isEmpty());

    if(result.size() <= 0)
    {
        result = QVector<QString>(1,"");
    }

    return result;
}

bool HNS_Message_Preview::fUpdate(const bool &refresh_pages, const bool &blank)
{
    string tempstring;
    HNS_Bitmap temp_bitmap;
    HNS_SignBoard_Info signboard_info = HNS_Message2::fGetDefaultSignBoardInfo();
    HNS_SignBoard signboard(signboard_info);
    bool success = true;
    type_multi_syntax_error multi_error = HNS_MULTI_SYNTAX_ERROR_NONE;

    if(refresh_pages)
    {
        tempstring = fMergePages().toStdString();
        f_current_message.fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr,&multi_error);
        if(multi_error != HNS_MULTI_SYNTAX_ERROR_NONE)
        {
            success = false;
        }
    }

    if(f_current_message.fGetNumPages() > 0)
    {
        signboard = f_current_message.fGetPage(f_current_page).fGetSignBoard(0,true);
        if(signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
        {
            if(!blank)
            {
                temp_bitmap = signboard.fGetFMBitmap();
                f_image = ConvertHNS_BitmapToQImage(temp_bitmap,HNS_Color(255,255,255),QColor(255,172,39));
            }
            else
            {
                f_image = QImage(f_current_message.fGetBoardWidth(),f_current_message.fGetBoardHeight(),QImage::Format_RGB32);
                f_image.fill(Qt::black);
            }
        }
        else if(signboard.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
        {
            f_images.clear();
            if(!blank)
            {
                f_images.resize(signboard.fGetNumBoards());
                for(size_t ui = 0; ui < signboard.fGetNumBoards(); ui++)
                {
                    temp_bitmap = signboard.fGetCharBoardBitmap(ui);
                }
            }
        }
    }
    else
    {
        if(signboard.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
        {
            f_image = QImage(f_current_message.fGetBoardWidth(),f_current_message.fGetBoardHeight(),QImage::Format_RGB32);
            f_image.fill(Qt::black);
        }
        else if(signboard.fGetType() == HNS_BRD_TRAILER_CHARACTER_BOARD)
        {
            f_images.clear();
            f_images.resize(signboard.fGetNumBoards());
            for(size_t ui = 0; ui < signboard.fGetNumBoards(); ui++)
            {
                f_images[ui] = QImage(signboard.fGetCharacterWidth(),signboard.fGetCharacterHeight(),QImage::Format_RGB32);
                f_images[ui].fill(Qt::black);
            }
        }
    }

    this->repaint();

    return success;
}

bool HNS_Message_Preview::fIsNewPageNeeded(const QString &)
{
    bool new_page_needed = false;
    HNS_Message2 temp_message = f_current_message;
    bool too_tall;

    std::string tempstring = fMergePages().toStdString();
    temp_message.fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr,nullptr,&too_tall);

    if(too_tall)
    {
        new_page_needed = true;
    }

    emit fNewPageCreated();

    return new_page_needed;
}
