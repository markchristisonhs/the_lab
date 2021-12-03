#include "MessageBoard.h"

using namespace std;

vector<HNS_Font> HNS_SignBoard::f_fonts;
vector<HNS_Graphic> HNS_SignBoard::f_graphics;

HNS_SignBoard::HNS_SignBoard():
    f_boards_wide(0)
  , f_boards_tall(0)
  , f_orientation(HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
  , f_type(HNS_BRD_TRAILER_FULL_MATRIX)
  , f_vsize(1750)
  , f_hsize(3000)
  , f_vborder(100)
  , f_hborder(100)
{

}

HNS_SignBoard::HNS_SignBoard(const size_t &boards_wide, const size_t &boards_tall, const type_hns_board_orientation &orientation, const type_hns_trailer_type &type,
                             const size_t &vsize, const size_t &hsize, const size_t &vborder, const size_t &hborder, const string &name):
    f_boards(boards_wide*boards_tall)
  , f_boards_wide(boards_wide)
  , f_boards_tall(boards_tall)
  , f_orientation(orientation)
  , f_type(type)
  , f_vsize(vsize)
  , f_hsize(hsize)
  , f_vborder(vborder)
  , f_hborder(hborder)
  , f_name(name)
{

}

void HNS_SignBoard::fSetBoardsWide(const int &boards_wide)
{
    f_boards_wide = boards_wide;
    fResize();
}

void HNS_SignBoard::fSetBoardsTall(const int &boards_tall)
{
    f_boards_tall = boards_tall;
    fResize();
}

void HNS_SignBoard::fSetOrientation(const type_hns_board_orientation &orientation)
{
    f_orientation = orientation;
}

void HNS_SignBoard::fSetType(const type_hns_trailer_type &type)
{
    f_type = type;
}

void HNS_SignBoard::fSetName(const string &name)
{
    f_name = name;
}

void HNS_SignBoard::fSetGraphicName(const string &graphic_name)
{
    f_graphic_name = graphic_name;
}

void HNS_SignBoard::fSetVSize(const size_t &vsize)
{
    f_vsize = vsize;
}

void HNS_SignBoard::fSetHSize(const size_t &hsize)
{
    f_hsize = hsize;
}

void HNS_SignBoard::fSetVBorder(const size_t &vborder)
{
    f_vborder = vborder;
}

void HNS_SignBoard::fSetHBorder(const size_t &hborder)
{
    f_hborder = hborder;
}

void HNS_SignBoard::fSetBoard(const HNS_Board &board)
{
    for(size_t i=0;i<f_boards.size();i++)
    {
        f_boards[i] = board;
    }
}

size_t HNS_SignBoard::fGetWidth() const
{
    if(f_boards.size() == 0)
    {
        return 0;
    }

    if(f_orientation == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
    {
        return f_boards_wide * f_boards[0].fGetLedsPerByte();
    }
    else
    {
        return f_boards_wide * f_boards[0].fGetNumBytes();
    }
}

size_t HNS_SignBoard::fGetHeight() const
{
    if(f_boards.size() == 0)
    {
        return 0;
    }

    if(f_orientation == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
    {
        return f_boards_tall * f_boards[0].fGetNumBytes();
    }
    else
    {
        return f_boards_tall & f_boards[0].fGetLedsPerByte();
    }
}

type_hns_board_orientation HNS_SignBoard::fGetOrientation() const
{
    return f_orientation;
}

type_hns_trailer_type HNS_SignBoard::fGetType() const
{
    return f_type;
}

string HNS_SignBoard::fGetName() const
{
    return f_name;
}

string HNS_SignBoard::fGetGraphicName() const
{
    return f_graphic_name;
}

size_t HNS_SignBoard::fGetBoardsTall() const
{
    return f_boards_tall;
}

size_t HNS_SignBoard::fGetBoardsWide() const
{
    return f_boards_wide;
}

size_t HNS_SignBoard::fGetNumBoards() const
{
    return f_boards.size();
}

size_t HNS_SignBoard::fGetLedsPerByte(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetLedsPerByte();
    }
    return 0;
}

size_t HNS_SignBoard::fGetNumBytes(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetNumBytes();
    }
    return 0;
}

size_t HNS_SignBoard::fGetVPitch(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetVPitch();
    }
    return 30;
}

size_t HNS_SignBoard::fGetHPitch(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetHPitch();
    }
    return 30;
}

size_t HNS_SignBoard::fGetVSize() const
{
    return f_vsize;
}

size_t HNS_SignBoard::fGetHSize() const
{
    return f_hsize;
}

size_t HNS_SignBoard::fGetVBorder() const
{
    return f_vborder;
}

size_t HNS_SignBoard::fGetHBorder() const
{
    return f_hborder;
}

size_t HNS_SignBoard::fGetCharacterHeight() const
{
    if(f_boards.size() == 0)
    {
        return 0;
    }

    if(f_type == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        return fGetBoardHeight();
    }
    else
    {
        return 0;
    }
}

size_t HNS_SignBoard::fGetCharacterWidth() const
{
    if(f_boards.size() == 0)
    {
        return 0;
    }

    if(f_type == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        return fGetBoardWidth();
    }
    else
    {
        return 0;
    }
}

HNS_Bitmap HNS_SignBoard::fGetCharBoardBitmap(const size_t &board_no)
{
    HNS_Bitmap result;
    unsigned char byte;
    bool led_on;

    if(board_no < f_boards.size())
    {
        if(fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_VERTICAL)
        {
            result = HNS_Bitmap(f_boards[board_no].fGetLedsPerByte(),f_boards[board_no].fGetNumBytes());

            for(size_t ui = 0; ui < f_boards[board_no].fGetNumBytes(); ui++)
            {
                byte = f_boards[board_no].fGetByte(ui);
                for(size_t uj = 0; uj < f_boards[board_no].fGetLedsPerByte(); uj++)
                {
                    led_on = ((byte & (0x1 << uj)) != 0);
                    result.fSetPixel(ui,result.fGetHeight()-uj-1,led_on);
                }
            }
        }
        else if(fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
        {
            result = HNS_Bitmap(f_boards[board_no].fGetNumBytes(),f_boards[board_no].fGetLedsPerByte());

            for(size_t ui = 0; ui < f_boards[board_no].fGetNumBytes(); ui++)
            {
                byte = f_boards[board_no].fGetByte(ui);
                for(size_t uj = 0; uj < f_boards[board_no].fGetLedsPerByte(); uj++)
                {
                    led_on = ((byte & (0x1 << uj)) != 0);
                    result.fSetPixel(uj,ui,led_on);
                }
            }
        }
    }

    return result;
}

HNS_Bitmap HNS_SignBoard::fGetFMBitmap()
{
    HNS_Bitmap result,temp_bitmap;
    size_t row,col, xpos, ypos;

    if(fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        result = HNS_Bitmap(fGetHeight(),fGetWidth());
        for(size_t ui = 0; ui < f_boards.size(); ui++)
        {
            row = ui / fGetBoardsWide();
            col = ui % fGetBoardsWide();

            xpos = col * fGetBoardWidth();
            ypos = row * fGetBoardHeight();

            temp_bitmap = fGetCharBoardBitmap(ui);

            result.fCopy(temp_bitmap,xpos,ypos);
        }
    }

    return result;
}

void HNS_SignBoard::fSetFonts(const vector<HNS_Font> &fonts)
{
    f_fonts = fonts;
}

void HNS_SignBoard::fSetGraphics(const vector<HNS_Graphic> &graphics)
{
    f_graphics = graphics;
}

type_hns_signboard_error HNS_SignBoard::fAddElement(HNS_Message_Justified_Element &element)
{
    HNS_Message_Element2 temp_element;
    size_t current_line = 0;
    string tempstring;
    if(f_type == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        size_t x,y;
        HNS_Bitmap temp_bitmap;
        if(f_boards.size() == (fGetBoardsWide() * fGetBoardsTall()) )
        {
            //first step, create a temporary bitmap with the element.
            //first task in the first step is to find the height of each line.  Borrow heavily from existing work done with my first generation of message handling
            if(element.fGetLineJustification() == HNS_JUSTIFICATION_FULL)
            {
                temp_bitmap = element.fGetBitmapFullJustified(&f_fonts);
            }
            else
            {
                temp_bitmap = element.fGetBitmap(&f_fonts, &f_graphics);
            }

            if(element.fIsGraphic())
            {
                x = element.fGetGraphicPos().fGetX();
                y = element.fGetGraphicPos().fGetY();
            }
            else
            {
                switch(element.fGetLineJustification())
                {
                case HNS_JUSTIFICATION_LEFT:
                    x = 0;
                    break;
                case HNS_JUSTIFICATION_FULL:
                case HNS_JUSTIFICATION_LINE_CENTER:
                    x = (fGetWidth() - temp_bitmap.fGetWidth())/2;
                    break;
                case HNS_JUSTIFICATION_RIGHT:
                    x = fGetWidth() - temp_bitmap.fGetWidth();
                    break;
                }

                switch(element.fGetPageJustification())
                {
                case HNS_JUSTIFICATION_TOP:
                    y = 0;
                    break;
                case HNS_JUSTIFICATION_PAGE_CENTER:
                    y = (fGetHeight() - temp_bitmap.fGetHeight())/2;
                    break;
                case HNS_JUSTIFICATION_BOTTOM:
                    y = fGetHeight() - temp_bitmap.fGetHeight();
                    break;
                }
            }
            fApplyBitmapToFMBoard(temp_bitmap,x,y);
        }
    }
    else if(f_type == HNS_BRD_TRAILER_CHARACTER_BOARD)
    {
        if( f_boards.size() == (fGetBoardsWide() * fGetBoardsTall()) )
        {
            int row,column;
            int num_characters = 0;
            HNS_Character temp_char;
            vector<HNS_Bitmap> characters;
            int current_font = 0;
            current_line = 0;
            if(element.fGetNumLines() > fGetBoardsTall())
            {
                return HNS_SGNBRD_ERROR_TOOTALL;
            }
            else
            {
                for(size_t ui = 0; ui < element.fGetNumElements(); ui++)
                {
                    temp_element = element.fGetElement(ui);
                    if(temp_element.fGetLineNo() != current_line)
                    {
                        //Add line to board
                        if(characters.size() > fGetBoardsWide())
                        {
                            return HNS_SGNBRD_ERROR_TOOWIDE;
                        }
                        else
                        {
                            switch(element.fGetLineJustification())
                            {
                            case HNS_JUSTIFICATION_LEFT:
                                column = 0;
                                break;
                            case HNS_JUSTIFICATION_LINE_CENTER:
                                column = (fGetBoardsWide() - characters.size())/2;
                                break;
                            case HNS_JUSTIFICATION_FULL:
                                column = 0;
                                break;
                            case HNS_JUSTIFICATION_RIGHT:
                                column = fGetBoardsWide() - characters.size();
                                break;
                            }

                            switch(element.fGetPageJustification())
                            {
                            case HNS_JUSTIFICATION_TOP:
                                row = current_line;
                                break;
                            case HNS_JUSTIFICATION_PAGE_CENTER:
                                row = ((fGetBoardsTall() - element.fGetNumLines())/2) + current_line;
                                break;
                            case HNS_JUSTIFICATION_BOTTOM:
                                row = (fGetBoardsTall() - element.fGetNumLines()) + current_line;
                                break;
                            }
                        }
                        fApplyLineToBoard(characters,row,column);
                        characters.clear();
                        current_line = temp_element.fGetLineNo();
                    }

                    if(!temp_element.fIsGraphic())
                    {
                        current_font = temp_element.fGetFontNo();
                        tempstring = temp_element.fGetText();
                        num_characters += tempstring.size();

                        for(size_t uj = 0; uj < tempstring.size(); uj++)
                        {
                            temp_char = f_fonts.at(current_font-1).fGetCharacter(tempstring[uj]);
                            if((static_cast<size_t>(temp_char.fGetHeight()) > fGetCharacterHeight()) || (static_cast<size_t>(temp_char.fGetWidth()) > fGetCharacterWidth()))
                            {
                                return HNS_SGNBRD_CHAR_SIZE;
                            }
                            else
                            {
                                characters.push_back(temp_char.fGetBitmap());
                            }
                        }
                    }
                }
                if(characters.size() > 0)
                {
                    if(characters.size() > fGetBoardsWide())
                    {
                        return HNS_SGNBRD_ERROR_TOOWIDE;
                    }
                    else
                    {
                        switch(element.fGetLineJustification())
                        {
                        case HNS_JUSTIFICATION_LEFT:
                            column = 0;
                            break;
                        case HNS_JUSTIFICATION_LINE_CENTER:
                            column = (fGetBoardsWide() - characters.size())/2;
                            break;
                        case HNS_JUSTIFICATION_FULL:
                            column = 0;
                            break;
                        case HNS_JUSTIFICATION_RIGHT:
                            column = fGetBoardsWide() - characters.size();
                            break;
                        }

                        switch(element.fGetPageJustification())
                        {
                        case HNS_JUSTIFICATION_TOP:
                            row = current_line;
                            break;
                        case HNS_JUSTIFICATION_PAGE_CENTER:
                            row = ((fGetBoardsTall() - element.fGetNumLines())/2) + current_line;
                            break;
                        case HNS_JUSTIFICATION_BOTTOM:
                            row = (fGetBoardsTall() - element.fGetNumLines()) + current_line;
                            break;
                        }

                        fApplyLineToBoard(characters,row,column);
                    }
                }
            }
        }
        else
        {
            return HNS_SGNBRD_ERROR_SGNBRD_FORMAT;
        }
    }

    return HNS_SGNBRD_ERROR_NONE;
}

void HNS_SignBoard::fClearBoard()
{
    for(size_t ui = 0; ui < f_boards.size(); ui++)
    {
        f_boards[ui].fClearBoard();
    }
}

type_hns_display_type HNS_SignBoard::fGetDisplayType() const
{
    if(f_boards.size() > 0)
    {
        return f_boards[0].fGetDisplayType();
    }
    else
    {
        return HNS_DISP_BROADCAST;
    }
}

vector< vector<unsigned char> > HNS_SignBoard::fGetDataStream()
{
    vector < vector <unsigned char> > result;
    vector <unsigned char> temp_vec;

    if((f_boards.size() == (fGetBoardsTall() * fGetBoardsWide())) && (f_boards.size() > 0))
    {
        switch(f_boards[0].fGetDisplayType())
        {
        case HNS_DISP_BROADCAST:
            result.resize(1);
            //Not needed; the insert function below handles this just fine
            //result[0].resize(f_boards[0].fGetNumBytes() * fGetNumVirBoards());
            break;
        case HNS_DISP_ONE_BOARD_AT_A_TIME:
            result.resize(fGetNumVirBoards());
            for(size_t ui = 0; ui < fGetNumVirBoards(); ui++)
            {
                result[ui].resize(f_boards[ui].fGetNumBytes());
            }
            break;
        }

        for(size_t ui = 0; ui < f_boards.size(); ui++)
        {
            temp_vec = fGetVirBytes(ui);
            switch(f_boards[0].fGetDisplayType())
            {
            case HNS_DISP_BROADCAST:
                result[0].insert(result[0].end(),temp_vec.begin(),temp_vec.end());
                break;
            case HNS_DISP_ONE_BOARD_AT_A_TIME:
                result[ui] = temp_vec;
                break;
            }
        }
    }

    return result;
}

void HNS_SignBoard::fAddPixelsOut(const vector< vector< HNS_PixelOut> > &pixels_out)
{
    if(pixels_out.size() <= (fGetBoardsTall() * fGetBoardsWide()))
    {
        for(size_t ui=0; ui < pixels_out.size(); ui++)
        {
            f_boards[ui].fAddPixelsOut(pixels_out[ui]);
        }
    }
}

void HNS_SignBoard::fAddPixelsOut(const size_t &board, const std::vector<HNS_PixelOut> &pixels_out)
{
    if(board < f_boards.size())
    {
        f_boards[board].fAddPixelsOut(pixels_out);
    }
}

void HNS_SignBoard::fAddPixelOut(const size_t &board, const HNS_PixelOut &pixel_out)
{
    if(board < f_boards.size())
    {
        f_boards[board].fAddPixelOut(pixel_out);
    }
}

void HNS_SignBoard::fAddPixelOut(const size_t &board, const size_t &byte, const size_t &led)
{
    if(board < f_boards.size())
    {
        f_boards[board].fAddPixelOut(byte,led);
    }
}

void HNS_SignBoard::fClearPixelOut()
{
    for(size_t ui=0;ui<f_boards.size();ui++)
    {
        f_boards[ui].fSetPixelOutState(HNS_PIX_OUT_FOUND);
        f_boards[ui].fClearPixelsOut();
    }
}

std::vector<HNS_PixelOut> HNS_SignBoard::fGetPixelsOut(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetPixelsOut();
    }
    return std::vector<HNS_PixelOut>();
}

type_hns_pix_out_state HNS_SignBoard::fGetPixelOutState(const size_t &board) const
{
    if(board < f_boards.size())
    {
        return f_boards[board].fGetPixelOutState();
    }
    return HNS_PIX_OUT_FOUND;
}

void HNS_SignBoard::fSetPixelOutState(const type_hns_pix_out_state &state, const size_t &board)
{
    if(board < f_boards.size())
    {
        f_boards[board].fSetPixelOutState(state);
    }
}

void HNS_SignBoard::fSetPixelOutStateAll(const type_hns_pix_out_state &state)
{
    for(size_t ui=0;ui<f_boards.size();ui++)
    {
        f_boards[ui].fSetPixelOutState(state);
    }
}

bool HNS_SignBoard::fPixOutStillSearching() const
{
    for(size_t ui=0;ui<f_boards.size();ui++)
    {
        if(f_boards[ui].fGetPixelOutState() == HNS_PIX_OUT_SEARCHING)
        {
            return true;
        }
    }

    return false;
}

void HNS_SignBoard::fGiveUpPixSearch()
{
    for(size_t ui=0;ui<f_boards.size();ui++)
    {
        if(f_boards[ui].fGetPixelOutState() == HNS_PIX_OUT_SEARCHING)
        {
            f_boards[ui].fSetPixelOutState(HNS_PIX_OUT_NOT_FOUND);
        }
    }
}

void HNS_SignBoard::fResize()
{
    f_boards.resize(f_boards_wide * f_boards_tall);
}

void HNS_SignBoard::fApplyCharacterToBoard(const HNS_Bitmap &character, const size_t &board_no)
{
    unsigned char byte = 0;
    if(board_no < f_boards.size())
    {
        if(f_orientation == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
        {
            if((character.fGetWidth() <= f_boards[board_no].fGetLedsPerByte())
            && (character.fGetHeight() <= f_boards[board_no].fGetNumBytes()))
            {
                for(size_t y = 0; y < character.fGetHeight();y++)
                {
                    byte = 0;
                    for(size_t x = 0; x < character.fGetWidth(); x++)
                    {
                        if(character.fGetPixel(x,y) != HNS_Color(0,0,0))
                        {
                            byte = byte | (0x1 << (character.fGetWidth()-y));
                        }
                    }
                    f_boards[board_no].fSetByte(y,byte);
                }
            }
        }
        else if(f_orientation == HNS_BRD_ORIENTATION_BYTES_VERTICAL)
        {
            if((character.fGetWidth() <= f_boards[board_no].fGetNumBytes())
            && (character.fGetHeight() <= f_boards[board_no].fGetLedsPerByte()))
            {
                for(size_t x = 0; x < character.fGetWidth();x++)
                {
                    byte = 0;
                    for(size_t y = 0; y < character.fGetHeight(); y++)
                    {
                        if(character.fGetPixel(x,y) != HNS_Color(0,0,0))
                        {
                            byte = byte | (0x1 << (character.fGetHeight()-y-1));
                        }
                    }
                    f_boards[board_no].fSetByte(x,byte);
                }
            }
        }
    }
}

void HNS_SignBoard::fApplyLineToBoard(const std::vector<HNS_Bitmap> &characters, const size_t &row, const size_t &col)
{
    size_t board_no;
    for(size_t uj = 0; uj < characters.size(); uj++)
    {
        board_no = (row * fGetBoardsWide()) + col + uj;
        fApplyCharacterToBoard(characters[uj],board_no);
    }
}

void HNS_SignBoard::fApplyBitmapToFMBoard(const HNS_Bitmap &bitmap, const size_t &dest_x, const size_t &dest_y)
{
    size_t board_x, board_y, board_no;
    size_t row,col;
    bool onoff;

    if((fGetBoardHeight() > 0) && (fGetBoardWidth() > 0))
    {
        for(size_t y = 0; y < bitmap.fGetHeight(); y++)
        {
            for(size_t x = 0; x < bitmap.fGetWidth(); x++)
            {
                col = (x + dest_x) / fGetBoardWidth();
                row = (y + dest_y) / fGetBoardHeight();
                board_no = (row * fGetBoardsWide()) + col;

                board_x = (x + dest_x) % fGetBoardWidth();
                board_y = (y + dest_y) % fGetBoardHeight();

                onoff = (bitmap.fGetPixel(x,y) != HNS_Color(0,0,0));

                fSetBoardLED(board_no,board_x,board_y,onoff);
            }
        }
    }
}

size_t HNS_SignBoard::fGetBoardHeight() const
{
    if(f_boards.size() > 0)
    {
        if(f_orientation == HNS_BRD_ORIENTATION_BYTES_VERTICAL)
        {
            return f_boards[0].fGetLedsPerByte();
        }
        else
        {
            return f_boards[0].fGetNumBytes();
        }
    }
    else
    {
        return 0;
    }
}

size_t HNS_SignBoard::fGetBoardWidth() const
{
    if(f_boards.size() > 0)
    {
        if(f_orientation == HNS_BRD_ORIENTATION_BYTES_VERTICAL)
        {
            return f_boards[0].fGetNumBytes();
        }
        else
        {
            return f_boards[0].fGetLedsPerByte();
        }
    }
    else
    {
        return 0;
    }
}

void HNS_SignBoard::fSetBoardLED(const size_t &board_no, const size_t &x, const size_t &y, const bool &onoff)
{
    if(board_no < f_boards.size())
    {
        size_t byte,position;
        if(fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_VERTICAL)
        {
            byte = x;
            position = fGetBoardHeight() - y - 1;
        }
        else if(fGetOrientation() == HNS_BRD_ORIENTATION_BYTES_HORIZONTAL)
        {
            position = x;
            byte = y;
        }
        f_boards[board_no].fSetLED(byte,position,onoff);
    }
}

size_t HNS_SignBoard::fGetNumVirBoards() const
{
    size_t result = 0;

    if(f_boards.size() > 0)
    {
        if(f_boards[0].fGetSplit())
        {
            result = (f_boards[0].fGetSplitWidth() * ((2 * fGetBoardsTall()) - 1)) + fGetBoardsWide();
        }
        else
        {
            result = f_boards.size();
        }
    }

    return result;
}

size_t HNS_SignBoard::fGetNumVirBytes() const
{
    size_t result = 0;

    if(f_boards.size() > 0)
    {
        if(f_boards[0].fGetSplit())
        {
            //so far all split boards are evenly divisible by 2. May need to revisit this if that doesn't hold in the future.
            result = f_boards[0].fGetNumBytes()/2;
        }
        else
        {
            result = f_boards[0].fGetNumBytes();
        }
    }

    return result;
}

vector<unsigned char> HNS_SignBoard::fGetVirBytes(const size_t &board_no) const
{
    size_t vir_row,actual_col;
    size_t actual_row;
    vector <unsigned char> result;
    //true if the board requested is the top of the board, false if its the bottom
    bool high_low;
    size_t actual_board;

    if(f_boards.size() > 0)
    {
        if(f_boards[0].fGetSplit())
        {
            if(board_no < fGetNumVirBoards())
            {
                result.resize(fGetNumVirBytes());
                vir_row = board_no / f_boards[0].fGetSplitWidth();
                actual_col = board_no % f_boards[0].fGetSplitWidth();
                actual_row = vir_row / 2;
                high_low = ((vir_row % 2) == 0);

                actual_board = (actual_row * f_boards[0].fGetSplitWidth()) + actual_col;

                for(size_t ui = 0; ui < fGetNumVirBytes(); ui++)
                {
                    result[ui] = f_boards[actual_board].fGetByte(ui + (!high_low ? 0 : fGetNumVirBytes()));
                }
            }
        }
        else
        {
            result = f_boards[board_no].fGetBytes();
        }
    }

    return result;
}

HNS_Board::HNS_Board():
    f_num_bytes(0)
  , f_leds_per_byte(0)
  , f_hpitch(30)
  , f_vpitch(30)
  , f_display_type(HNS_DISP_BROADCAST)
  , f_split(false)
  , f_split_width(0)
  , f_pix_out_state(HNS_PIX_OUT_FOUND)
{

}

HNS_Board::HNS_Board(const std::string &name, const size_t &num_bytes, const size_t &leds_per_byte, const size_t &hpitch, const size_t &vpitch, const bool &split, const size_t &split_width):
    f_bytes(num_bytes)
  , f_num_bytes(num_bytes)
  , f_leds_per_byte(leds_per_byte)
  , f_name(name)
  , f_hpitch(hpitch)
  , f_vpitch(vpitch)
  , f_display_type(HNS_DISP_BROADCAST)
  , f_split(split)
  , f_split_width(split_width)
{

}

size_t HNS_Board::fGetNumBytes() const
{
    return f_num_bytes;
}

size_t HNS_Board::fGetLedsPerByte() const
{
    return f_leds_per_byte;
}

string HNS_Board::fGetName() const
{
    return f_name;
}

size_t HNS_Board::fGetHPitch() const
{
    return f_hpitch;
}

size_t HNS_Board::fGetVPitch() const
{
    return f_vpitch;
}

bool HNS_Board::fGetSplit() const
{
    return f_split;
}

size_t HNS_Board::fGetSplitWidth() const
{
    return f_split_width;
}

void HNS_Board::fSetByte(const size_t &index, const unsigned char &byte)
{
    if(index < f_bytes.size())
    {
        f_bytes[index] = byte;
    }
}

unsigned char HNS_Board::fGetByte(const size_t &index) const
{
    if(index < f_bytes.size())
    {
        return f_bytes[index];
    }
    return 0;
}

vector <unsigned char> HNS_Board::fGetBytes() const
{
    return f_bytes;
}

void HNS_Board::fClearBoard()
{
    for(size_t ui = 0; ui < f_bytes.size(); ui++)
    {
        f_bytes[ui] = 0;
    }
}

void HNS_Board::fSetLED(const size_t &byte, const size_t &position, const bool &onoff)
{
    if((position < f_leds_per_byte)
    && (byte < f_bytes.size()))
    {
        unsigned char mask;
        if(onoff)
        {
            mask = 0x1 << position;
            f_bytes[byte] = f_bytes[byte] | mask;
        }
        else
        {
            mask = 0xFF ^ (0x1 << position);
            f_bytes[byte] = f_bytes[byte] & mask;
        }
    }
}

type_hns_display_type HNS_Board::fGetDisplayType() const
{
    return f_display_type;
}

void HNS_Board::fClearPixelsOut()
{
    f_pixels_out.clear();
}

void HNS_Board::fAddPixelsOut(const vector<HNS_PixelOut> &pixels_out)
{
    f_pixels_out.insert(f_pixels_out.end(),pixels_out.begin(),pixels_out.end());
}

void HNS_Board::fAddPixelOut(const HNS_PixelOut &pixel_out)
{
    f_pixels_out.push_back(pixel_out);
}

void HNS_Board::fAddPixelOut(const size_t &byte,const size_t &led)
{
    fAddPixelOut(HNS_PixelOut(byte,led));
}

void HNS_Board::fSetPixelOut(const std::vector<HNS_PixelOut> &pixels_out)
{
    f_pixels_out = pixels_out;
}

size_t HNS_Board::fGetNumPixelsOut() const
{
    return f_pixels_out.size();
}

std::vector<HNS_PixelOut> HNS_Board::fGetPixelsOut() const
{
    return f_pixels_out;
}

type_hns_pix_out_state HNS_Board::fGetPixelOutState() const
{
    return f_pix_out_state;
}

void HNS_Board::fSetPixelOutState(const type_hns_pix_out_state &state)
{
    f_pix_out_state = state;
}
