#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include <vector>
#include <string>
#include "graphics.h"
#include "fonts.h"
#include "PixelOut.h"

typedef enum
{
    HNS_BRD_ORIENTATION_BYTES_HORIZONTAL=0,
    HNS_BRD_ORIENTATION_BYTES_VERTICAL
} type_hns_board_orientation;

typedef enum
{
    HNS_BRD_TRAILER_FULL_MATRIX = 0,
    HNS_BRD_TRAILER_CHARACTER_BOARD
} type_hns_trailer_type;

typedef enum
{
    HNS_BRD_ERROR_NONE = 0,
    HNS_BRD_ERROR_TOOWIDE = 0x1,
    HNS_BRD_ERROR_TOOTALL = 0x2
} type_hns_board_error;

typedef enum
{
    HNS_SGNBRD_ERROR_NONE = 0,
    HNS_SGNBRD_ERROR_TOOWIDE = 0x1,
    HNS_SGNBRD_ERROR_TOOTALL = 0x2,
    HNS_SGNBRD_ERROR_SGNBRD_FORMAT = 0x4,
    HNS_SGNBRD_CHAR_SIZE = 0x8
} type_hns_signboard_error;

typedef enum
{
    HNS_SGNBRD_SPECIAL_NONE = 0x0,
    HNS_SGNBRD_SPECIAL_DUAL_DISPLAY = 0x1,
    HNS_SGNBRD_SPECIAL_VSL = 0x2
} type_hns_signboard_specials;

typedef enum
{
    HNS_DISP_BROADCAST = 0,
    HNS_DISP_ONE_BOARD_AT_A_TIME
} type_hns_display_type;

typedef enum
{
    HNS_PIX_OUT_SEARCHING = 0,
    HNS_PIX_OUT_FOUND,
    HNS_PIX_OUT_NOT_FOUND
} type_hns_pix_out_state;

class HNS_Board;
class HNS_Message_Justified_Element;

enum
{
    HNS_BOARD_SPLIT_UPPER = 0,
    HNS_BOARD_SPLIT_LOWER = 1
};

class HNS_SignBoard_Info;

class HNS_Board
{
public:
    HNS_Board();
    HNS_Board(const std::string &name, const size_t &num_bytes, const size_t &leds_per_byte, const size_t &hpitch = 30, const size_t &vpitch = 30, const bool &split = false, const size_t &split_width = 0 );

    size_t fGetNumBytes() const;
    size_t fGetLedsPerByte() const;
    std::string fGetName() const;

    size_t fGetHPitch() const;
    size_t fGetVPitch() const;

    bool fGetSplit() const;
    size_t fGetSplitWidth() const;

    void fSetByte(const size_t &index, const unsigned char &byte);
    unsigned char fGetByte(const size_t &index) const;
    std::vector<unsigned char> fGetBytes() const;
    HNS_Board fGetSplitBoard(const int &upper_lower) const;

    void fClearBoard();

    //sets one LED on the Board.  position = 0 means LSB and position = f_leds_per_byte-1 means MSB
    void fSetLED(const size_t &byte, const size_t &position, const bool &onoff);

    type_hns_display_type fGetDisplayType() const;

    void fClearPixelsOut();
    void fAddPixelsOut(const std::vector<HNS_PixelOut> &pixels_out);
    void fAddPixelOut(const HNS_PixelOut &pixel_out);
    void fAddPixelOut(const size_t &byte,const size_t &led);
    void fSetPixelOut(const std::vector<HNS_PixelOut> &pixels_out);
    size_t fGetNumPixelsOut() const;
    std::vector<HNS_PixelOut> fGetPixelsOut() const;

    type_hns_pix_out_state fGetPixelOutState() const;
    void fSetPixelOutState(const type_hns_pix_out_state &state);

private:
    std::vector<unsigned char> f_bytes;
    size_t f_num_bytes;
    size_t f_leds_per_byte;
    std::string f_name;

    size_t f_hpitch,f_vpitch;

    type_hns_display_type f_display_type;

    //this demands some explanation.  Some boards are effectively two boards in one.  For exampl 264652.
    //So for example you might have an SMC-5000 with 6 boards across and two boards tall.  But each board is split into a top and bottom.
    //You would have the top left board as 1 and 7, the next is 2 and 8, the next 3 and 9, and so on
    //This gets complicated when you don't have the full width  of boards, like in the TTMS system.  So for that system you have
    //boards 1 2 3 7 8 9, with 4 5 6 skipped.
    bool f_split;
    size_t f_split_width;

    std::vector<HNS_PixelOut> f_pixels_out;
    type_hns_pix_out_state f_pix_out_state;
};

class HNS_SignBoard_Info
{
public:
    HNS_SignBoard_Info();
    HNS_SignBoard_Info(const HNS_Board &board, const size_t &boards_wide, const size_t &boards_tall, const type_hns_board_orientation &orientation,
                       const type_hns_trailer_type &type, const size_t &vsize = 1750, const size_t &hsize = 3000, const size_t &vborder = 100,
                       const size_t &hborder = 100, const std::string &name = "", const std::string &graphic_name = "", const type_hns_signboard_specials &specials = HNS_SGNBRD_SPECIAL_NONE);

    HNS_Board& fBoard() {return f_board;}
    HNS_Board fGetBoard() const {return f_board;}
    void fSetBoard(const HNS_Board &board) {f_board = board;}
    size_t fGetNumBoards() {return (f_boards_wide * f_boards_tall);}

    size_t& fBoardsWide() {return f_boards_wide;}
    size_t fGetBoardsWide() const {return f_boards_wide;}
    void fSetBoardsWide(const size_t &boards_wide) {f_boards_wide = boards_wide;}

    size_t& fBoardsTall() {return f_boards_tall;}
    size_t fGetBoardsTall() const {return f_boards_tall;}
    void fSetBoardsTall(const size_t &boards_tall) {f_boards_tall = boards_tall;}

    size_t fGetWidth() const;
    size_t fGetHeight() const;

    size_t fGetCharacterHeight() const;
    size_t fGetCharacterWidth() const;

    type_hns_board_orientation& fOrientation() {return f_orientation;}
    type_hns_board_orientation fGetOrientation() const {return f_orientation;}
    void fSetOrientation(const type_hns_board_orientation &orientation) {f_orientation = orientation;}

    type_hns_trailer_type& fType() {return f_type;}
    type_hns_trailer_type fGetType() const {return f_type;}
    void fSetType(const type_hns_trailer_type &type) {f_type = type;}

    size_t &fVSize() {return f_vsize;}
    size_t fGetVSize() const {return f_vsize;}
    void fSetVSize(const size_t &vsize) {f_vsize = vsize;}

    size_t &fHSize() {return f_hsize;}
    size_t fGetHSize() const {return f_hsize;}
    void fSetHSize(const size_t &hsize) {f_hsize = hsize;}

    size_t& fVBorder() {return f_vborder;}
    size_t fGetVBorder() const {return f_vborder;}
    void fSetVBorder(const size_t &vborder) {f_vborder = vborder;}

    size_t& fHBorder() {return f_hborder;}
    size_t fGetHBorder() const {return f_hborder;}
    void fSetHBorder(const size_t &hborder) {f_hborder = hborder;}

    std::string& fName() {return f_name;}
    std::string fGetName() const {return f_name;}
    void fSetName(const std::string &name) {f_name = name;}

    std::string fGetConfigName() const {return f_config_name;}
    void fSetConfigName(const std::string &config_name) {f_config_name = config_name;}

    std::string& fGraphicName() {return f_graphic_name;}
    std::string fGetGraphicName() const {return f_graphic_name;}
    void fSetGraphicName(const std::string &graphic_name) {f_graphic_name = graphic_name;}

    type_hns_signboard_specials& fSpecials() {return f_specials;}
    type_hns_signboard_specials fGetSpecials() const {return f_specials;}
    bool fGetSpecial(const type_hns_signboard_specials &special) const {return ((static_cast<int>(f_specials) & static_cast<int>(special)) != 0);}
    void fSetSpecial(const type_hns_signboard_specials &specials) {f_specials = static_cast<type_hns_signboard_specials> (static_cast<int>(f_specials) | static_cast<int>(specials));}
    void fClearSpecial(const type_hns_signboard_specials &specials) {f_specials = static_cast<type_hns_signboard_specials>(static_cast<int>(f_specials) & ~static_cast<int>(specials));}
    void fClearSpecials() {f_specials = HNS_SGNBRD_SPECIAL_NONE;}

    void fSetSpecialData(const type_hns_signboard_specials &specials, const std::vector<int> &special_data);
    std::vector<int> fGetSpecialData(const type_hns_signboard_specials &specials) const;

    HNS_SignBoard_Info fGetAdjustedBoardInfo() const;
private:
    size_t fGetBoardHeight() const;
    size_t fGetBoardWidth() const;

    HNS_Board f_board;
    size_t f_boards_wide;
    size_t f_boards_tall;

    type_hns_board_orientation f_orientation;

    type_hns_trailer_type f_type;

    size_t f_vsize,f_hsize;
    size_t f_vborder, f_hborder;
    std::string f_name;
    //Where to load graphics from when loading this trailer.
    std::string f_graphic_name;
    //Where to grab the NTCIP file describing this trailer.
    std::string f_config_name;

    type_hns_signboard_specials f_specials;
    std::vector< std::vector<int> > f_special_data;
};

#include "message.h"

class HNS_SignBoard
{
public:
    HNS_SignBoard();
    HNS_SignBoard(const size_t &boards_wide, const size_t &boards_tall, const type_hns_board_orientation &orientation, const type_hns_trailer_type &type,
                  const size_t &vsize = 1750, const size_t &hsize = 3000, const size_t &vborder = 100, const size_t &hborder = 100, const std::string &name = "",
                  const std::string &graphic_name = "", const type_hns_signboard_specials &specials = HNS_SGNBRD_SPECIAL_NONE);
    HNS_SignBoard(const HNS_SignBoard_Info &info);

    HNS_SignBoard_Info fGetSignBoardInfo() const;

    void fSetBoard(const HNS_Board &board);
    void fSetBoard(const HNS_Board &board, const size_t &index);
    void fSetBoard(const HNS_Board &board, const size_t &board_x, const size_t &board_y);
    void fSetBoards(const std::vector<HNS_Board> &boards, const size_t &index);
    bool fApplySubSignBoard(const HNS_SignBoard &signboard, const size_t &index);
    bool fApplySubSignBoard(const HNS_SignBoard &signboard, const size_t &board_x, const size_t &board_y);
    HNS_Board fGetBoard(const size_t &index) const;
    HNS_Board fGetBoard(const size_t &board_x, const size_t &board_y) const;
    std::vector<HNS_Board> fGetBoards() const;
    void fSetBoardsWide(const int &boards_wide);
    void fSetBoardsTall(const int &boards_tall);
    void fSetOrientation(const type_hns_board_orientation &orientation);
    void fSetType(const type_hns_trailer_type &type);
    void fSetName(const std::string &name);
    void fSetGraphicName(const std::string &graphics);

    void fSetVSize(const size_t &vsize);
    void fSetHSize(const size_t &hsize);

    void fSetVBorder(const size_t &vborder);
    void fSetHBorder(const size_t &hborder);

    size_t fGetWidth() const;
    size_t fGetHeight() const;
    type_hns_board_orientation fGetOrientation() const;
    type_hns_trailer_type fGetType() const;
    std::string fGetName() const;
    std::string fGetGraphicName() const;

    size_t fGetBoardsTall() const;
    size_t fGetBoardsWide() const;
    size_t fGetNumBoards() const;

    size_t fGetLedsPerByte() const;
    size_t fGetNumBytes() const;
    size_t fGetVPitch() const;
    size_t fGetHPitch() const;

    size_t fGetVSize() const;
    size_t fGetHSize() const;

    size_t fGetVBorder() const;
    size_t fGetHBorder() const;

    size_t fGetCharacterHeight() const;
    size_t fGetCharacterWidth() const;

    bool fGetSpecial(const type_hns_signboard_specials &special) {return f_signboard_info.fGetSpecial(special);}
    type_hns_signboard_specials fGetSpecials() const {return f_signboard_info.fGetSpecials();}
    void fSetSpecial(const type_hns_signboard_specials &specials) {f_signboard_info.fSetSpecial(specials);}
    void fClearSpecial(const type_hns_signboard_specials &specials) {f_signboard_info.fClearSpecial(specials);}
    void fClearSpecials() {f_signboard_info.fClearSpecials();}

    //Gets the bitmap of an individual character board, in a character matrix trailer
    HNS_Bitmap fGetCharBoardBitmap(const size_t &board_no);

    //Gets the bitmap of a full matrix signboard
    HNS_Bitmap fGetFMBitmap();

    static void fSetFonts(const std::vector<HNS_Font> &fonts);
    static void fSetGraphics(const std::vector<HNS_Graphic> &graphics);

    type_hns_signboard_error fAddElement(HNS_Message_Justified_Element &element, const int64_t &time = 0, const bool &preview_mode = false);

    void fClearBoard();

    type_hns_display_type fGetDisplayType() const;

    std::vector< std::vector<unsigned char> > fGetDataStream();

    void fAddPixelsOut(const std::vector< std::vector< HNS_PixelOut> > &pixels_out);
    void fAddPixelsOut(const size_t &board, const std::vector<HNS_PixelOut> &pixels_out);
    void fAddPixelOut(const size_t &board, const HNS_PixelOut &pixel_out);
    void fAddPixelOut(const size_t &board, const size_t &byte, const size_t &led);
    void fClearPixelOut(const bool &reset_state = true);
    std::vector<HNS_PixelOut> fGetPixelsOut(const size_t &board) const;

    type_hns_pix_out_state fGetPixelOutState(const size_t &board) const;

    std::vector< std::vector<int> > fGetPixelsOutXY() const;
    void fSetPixelOutState(const type_hns_pix_out_state &state, const size_t &board);
    void fSetPixelOutStateAll(const type_hns_pix_out_state &state);
    bool fPixOutStillSearching() const;
    void fGiveUpPixSearch();
private:
    void fResize();

    //For 3 line boards, applies a character to one board
    void fApplyCharacterToBoard(const HNS_Bitmap &character, const size_t &board_no);
    void fApplyLineToBoard(const std::vector<HNS_Bitmap> &characters, const size_t &row, const size_t &col);

    void fApplyBitmapToFMBoard(const HNS_Bitmap &bitmap, const size_t &dest_x, const size_t &dest_y);

    size_t fGetBoardHeight() const;
    size_t fGetBoardWidth() const;

    void fSetBoardLED(const size_t &board_no, const size_t &x, const size_t &y, const bool &onoff);

    //this returns the number of boards when accounting for split boards.  For most sign boards, this returns f_boards.size()
    //But for the TTMS sign this would return 9.  For SMC-5000 this would return 24.
    size_t fGetNumVirBoards() const;
    //Same for getting the number of bytes
    size_t fGetNumVirBytes() const;
    //Get actual bytes.  Board_no refers not to the specific board, but to the "virtual board" that is half of a board.  This is really confusing.
    //In the case of a sign board where the bytes are not split, board_no is just the number of the board.
    std::vector<unsigned char> fGetVirBytes(const size_t &board_no) const;
    std::vector<unsigned char> fGetSplitByteStream();

    std::vector<HNS_Board> f_boards;

    //Where to load graphics from when loading this trailer.
    static std::vector<HNS_Font> f_fonts;
    static std::vector<HNS_Graphic> f_graphics;

    HNS_SignBoard_Info f_signboard_info;
};

class HNS_DisplayBuffer
{
public:
    HNS_DisplayBuffer():f_crc(0) {}

    void fSetBuffer(const std::vector<unsigned char> &buffer);
    std::vector<unsigned char> fGetBuffer() const {return f_buffer;}
    size_t fGetSize() const {return f_buffer.size();}
    void fClear();
    void fPushBack(const unsigned char &item);

    uint16_t fGetCRC() const {return f_crc;}
private:
    std::vector<unsigned char> f_buffer;
    uint16_t f_crc;
};

#endif // MESSAGEBOARD_H
