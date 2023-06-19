#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QDir>
#include <hns_ntcip_xml.h>
#include <QStandardPaths>
#include <utilities.h>
#include <structures.h>
#include <vector>
#include <graphics.h>
#include <CRC.h>
#include <HNS_NTCIP_MIB_Strings.h>
#include <sstream>
#include <HNS_NTCIP_Utilities.h>
#include <HNS_NTCIP_Enums.h>
#include <QSettings>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace NTCIP_MESSAGE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    type_multi_syntax_error error;
    string tempstring;

    ui->setupUi(this);
    fLoadNTCIP();
    fInitFontsFromNTCIP();
    fInitGraphicsFromNTCIP();
    fLoadTrailers();

    f_trailer_select = 5;
    fSetTrailer(f_signboard_library[f_trailer_select]);

    HNS_SignBoard::fSetFonts(f_fonts);
    HNS_SignBoard::fSetGraphics(f_graphics);

    tempstring = "[fo8]55";
    f_current_message2[f_vsl_display].fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr,&error);
    f_current_message2[f_normal_display].fSetMULTI(tempstring,&f_fonts,&f_graphics,nullptr,&error);

    HNS_DisplayBuffer buffer = fGenerateBuffer(0);

    vector<unsigned char> debug_buffer = buffer.fGetBuffer();

    QString debug_string;
    for(size_t ui=0; ui<debug_buffer.size();ui++)
    {
        debug_string += QString("%1 ").arg(debug_buffer[ui],2,16,QLatin1Char('0'));
    }

    ui->PTE_Debug->appendPlainText(debug_string);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fKludge()
{
    vector<unsigned char> temp_vec(4,0);
    CalcCRC(temp_vec);

    HNS_intToByteArray(2);
    HNS_ByteArrayToint(temp_vec);

    HNS_IP_Settings::fIPStringtoByteArray("0.0.0.0");
    HNS_SignBoard dummy_brd;

    HNS_Color dummy_color;
}

void MainWindow::fLoadNTCIP()
{
    QString file_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "NTCIP" + QDir::separator() + "NTCIP.xml";
    QString load_path = file_path;

    HNS_NTCIP_XML xml_file(load_path);

    xml_file.fParseNTCIPFile(&f_ntcip);

    ui->PTE_Debug->appendPlainText(QString("Loading Done"));
}

void MainWindow::fInitFontsFromNTCIP()
{
    stringstream ss;
    string tempstring;
    HNS_Font temp_font;
    HNS_Character temp_character;
    int num_fonts, font_status, max_chars, itemp;
    vector<unsigned char> temp_data;

    f_fonts.clear();

    num_fonts = f_ntcip.fGetIntData(gk_numFonts);

    for(int i=0;i<num_fonts;i++)
    {
        ss.str("");
        ss << gk_fontStatus << "." << (i+1);

        font_status = f_ntcip.fGetIntData(ss.str());

        max_chars = f_ntcip.fGetIntData(gk_maxFontCharacters);

        //A used font.
        if(font_status == 6 || font_status == 4)
        {
            ss.str("");
            ss << gk_fontNumber << "." << (i+1);
            itemp = f_ntcip.fGetIntData(ss.str());
            temp_font.fSetFontNumber(itemp);

            ss.str("");
            ss << gk_fontName << "." << (i+1);
            tempstring = f_ntcip.fGetStringData(ss.str());
            temp_font.fSetFontName(tempstring);

            ss.str("");
            ss << gk_fontHeight << "." << (i+1);
            itemp = f_ntcip.fGetIntData(ss.str());
            temp_font.fSetFontHeight(itemp);

            ss.str("");
            ss << gk_fontCharSpacing << "." << (i+1);
            itemp = f_ntcip.fGetIntData(ss.str());
            temp_font.fSetCharSpacing(itemp);

            ss.str("");
            ss << gk_fontLineSpacing << "." << (i+1);
            itemp = f_ntcip.fGetIntData(ss.str());
            temp_font.fSetLineSpacing(itemp);

            for(int j=0;j<max_chars;j++)
            {
                ss.str("");
                ss << gk_characterNumber << "." << (i+1) << "." << (j+1);
                itemp = f_ntcip.fGetIntData(ss.str());

                //A used character
                if(itemp != 0)
                {
                    temp_character.fSetCharacterNumber(itemp);

                    ss.str("");
                    ss << gk_characterWidth << "." << (i+1) << "." << (j+1);
                    itemp = f_ntcip.fGetIntData(ss.str());

                    ss.str("");
                    ss << gk_characterBitmap << "." << (i+1) << "." << (j+1);
                    temp_data = f_ntcip.fGetData(ss.str());
                    temp_character.fSetData(temp_data,itemp,temp_font.fGetFontHeight());

                    temp_font.fAddCharacter(temp_character);
                }
            }

            f_fonts.push_back(temp_font);
        }
        //MSC20211221 Don't push non-used fonts to the font vector!!!!
    }
}

void MainWindow::fInitGraphicsFromNTCIP()
{
    stringstream ss;
    string tempstring;
    int num_graphics,max_graphics;
    HNS_Graphic temp_graphic;
    HNS_Bitmap temp_bitmap;
    vector<unsigned char> temp_data, temp_data2;
    int itemp, block_size, width, height, num_blocks;

    f_graphics.clear();

    num_graphics = f_ntcip.fGetIntData(gk_dmsGraphicNumEntries);

    max_graphics = f_ntcip.fGetIntData(gk_dmsGraphicMaxEntries);

    f_graphics.resize(max_graphics);

    block_size = f_ntcip.fGetIntData(gk_dmsGraphicBlockSize);

    for(int i=0;i<max_graphics;i++)
    {
        ss.str("");
        ss << gk_dmsGraphicStatus << "." << (i+1);
        itemp = f_ntcip.fGetIntData(ss.str(),GRAPHIC_STATUS_NOT_USED);

        if(itemp == GRAPHIC_STATUS_READY_FOR_USE || itemp == GRAPHIC_STATUS_PERMANENT)
        {
            ss.str("");
            ss << gk_dmsGraphicNumber << "." << (i+1);
            itemp = f_ntcip.fGetIntData(ss.str());
            temp_graphic.fSetGraphicNumber(itemp);

            ss.str("");
            ss << gk_dmsGraphicName << "." << (i+1);
            tempstring = f_ntcip.fGetStringData(ss.str());
            temp_graphic.fSetName(tempstring);

            ss.str("");
            ss << gk_dmsGraphicHeight << "." << (i+1);
            height = f_ntcip.fGetIntData(ss.str());

            ss.str("");
            ss << gk_dmsGraphicWidth << "." << (i+1);
            width = f_ntcip.fGetIntData(ss.str());

            num_blocks = ((((width * height)+7)/8) + (block_size -1))/block_size;
            temp_data.clear();
            for(int j = 0; j < num_blocks; j++)
            {
                ss.str("");
                ss << gk_dmsGraphicBlockBitmap << "." << (i+1) << "." << (j+1);
                temp_data2 = f_ntcip.fGetData(ss.str());
                temp_data.insert(temp_data.end(),temp_data2.begin(),temp_data2.end());
            }
            temp_bitmap = HNS_Bitmap(temp_data,height,width);
            temp_graphic.fSetBitmap(temp_bitmap);
            f_graphics[i] = temp_graphic;
        }
    }

    num_graphics = CountGraphics(&f_ntcip);
    f_ntcip.fSetData(gk_dmsGraphicNumEntries,num_graphics,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
}

void MainWindow::fLoadTrailers(const bool &reset)
{
    int num_trailers, num_boards;
    int num_bytes, leds_per_byte;
    int itemp;
    int vpitch,hpitch;
    bool split;
    QString special_string;
    int split_width;
    HNS_Board temp_board;
    HNS_SignBoard_Info temp_signboard;
    QString tempstring,tempstring2;
    QString name, board_name;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "Trailer",rc_path;
    QString full_path = path + QDir::separator() + "config.ini";
    QStringList q_special_data,q_special_strings;
    vector<int> special_data;

    if(reset)
    {
        QFile(full_path).remove();
    }
    if(!QFile(full_path).exists() || reset)
    {
        if(!QFileInfo::exists(path))
        {
            QDir dir = QDir(path);
            dir.mkpath(path);
        }
        rc_path = ":/config/Trailer/config.ini";
        QFile(rc_path).copy(full_path);
        QFile::setPermissions(full_path,QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    }
    QSettings settingsfile(full_path,QSettings::IniFormat);

    num_boards = settingsfile.value("Boards/NumBoards",0).toInt();

    for(int i=0;i<num_boards;i++)
    {
        tempstring = QString("Board%1").arg(i,2,10,QChar('0'));

        name = settingsfile.value(tempstring + "/name","default").toString();

        num_bytes = settingsfile.value(tempstring + "/num_bytes",0).toInt();
        leds_per_byte = settingsfile.value(tempstring + "/leds_per_byte",0).toInt();
        vpitch = settingsfile.value(tempstring + "/vpitch",30).toInt();
        hpitch = settingsfile.value(tempstring + "/hpitch",30).toInt();
        special_string = settingsfile.value(tempstring + "/special","no").toString();
        split = (special_string == "split");
        split_width = settingsfile.value(tempstring + "/split_width",0).toInt();

        f_board_library.push_back(HNS_Board(name.toStdString(),num_bytes,leds_per_byte,hpitch,vpitch,split,split_width));
    }

    num_trailers = settingsfile.value("Signboards/NumSignboards",0).toInt();

    for(int i=0;i<num_trailers;i++)
    {
        tempstring = QString("Signboard%1").arg(i,2,10,QChar('0'));

        name = settingsfile.value(tempstring + "/name","default").toString();
        temp_signboard.fSetName(name.toStdString());

        name = settingsfile.value(tempstring + "/graphics","none").toString();
        temp_signboard.fSetGraphicName(name.toStdString());

        tempstring2 = settingsfile.value(tempstring + "/byte_orientation","horizontal").toString();
        if(tempstring2 == "horizontal")
        {
            temp_signboard.fSetOrientation(HNS_BRD_ORIENTATION_BYTES_HORIZONTAL);
        }
        else
        {
            temp_signboard.fSetOrientation(HNS_BRD_ORIENTATION_BYTES_VERTICAL);
        }

        tempstring2 = settingsfile.value(tempstring + "/type","full_matrix").toString();
        if(tempstring2 == "full_matrix")
        {
            temp_signboard.fSetType(HNS_BRD_TRAILER_FULL_MATRIX);
        }
        else
        {
            temp_signboard.fSetType(HNS_BRD_TRAILER_CHARACTER_BOARD);
        }

        itemp = settingsfile.value(tempstring + "/boards_wide",0).toInt();
        temp_signboard.fSetBoardsWide(itemp);

        itemp = settingsfile.value(tempstring + "/boards_tall",0).toInt();
        temp_signboard.fSetBoardsTall(itemp);

        itemp = settingsfile.value(tempstring + "/vsize",1750).toInt();
        temp_signboard.fSetVSize(itemp);

        itemp = settingsfile.value(tempstring + "/hsize",3000).toInt();
        temp_signboard.fSetHSize(itemp);

        itemp = settingsfile.value(tempstring + "/vborder",100).toInt();
        temp_signboard.fSetVBorder(itemp);

        itemp = settingsfile.value(tempstring + "/hborder",100).toInt();
        temp_signboard.fSetHBorder(itemp);

        q_special_strings = settingsfile.value(tempstring + "/special").toStringList();

        for(int j=0;j<q_special_strings.size();j++)
        {
            special_string = q_special_strings[j];
            if(special_string == "dual_display")
            {
                temp_signboard.fSetSpecial(HNS_SGNBRD_SPECIAL_DUAL_DISPLAY);

                q_special_data = settingsfile.value(tempstring + "/special_data" + QString("/%1").arg(j+1)).toStringList();

                for(int k=0;k<q_special_data.size();k++)
                {
                    special_data.push_back(q_special_data[k].toInt());
                }
                temp_signboard.fSetSpecialData(HNS_SGNBRD_SPECIAL_DUAL_DISPLAY,special_data);
            }
            else if(special_string == "vsl")
            {
                temp_signboard.fSetSpecial(HNS_SGNBRD_SPECIAL_VSL);

                q_special_data = settingsfile.value(tempstring + "/special_data" + QString("/%1").arg(j+1)).toStringList();

                for(int k=0;k<q_special_data.size();k++)
                {
                    special_data.push_back(q_special_data[k].toInt());
                }
                temp_signboard.fSetSpecialData(HNS_SGNBRD_SPECIAL_VSL,special_data);
            }
        }

        board_name = settingsfile.value(tempstring + "/board","default").toString();
        int j = 0;
        for(j=0;j<f_board_library.size();j++)
        {
            if(board_name == QString::fromStdString(f_board_library[j].fGetName()))
            {
                temp_board = f_board_library[j];
                break;
            }
        }
        if(j < f_board_library.size())
        {
            temp_signboard.fSetBoard(temp_board);
        }
        else
        {
            continue;
        }

        f_signboard_library.push_back(temp_signboard);
    }
}

//MSC20210406 Refresh the message before pushing the updated message to the display.
void MainWindow::fSetTrailer(const HNS_SignBoard_Info &sign_board_info)
{
    milliseconds ms = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
    string tempstring;

    HNS_Message2::fSetDefaultSignBoardInfo(sign_board_info.fGetAdjustedBoardInfo());
    if(sign_board_info.fGetSpecial(HNS_SGNBRD_SPECIAL_DUAL_DISPLAY))
    {
        f_current_message2.resize(2);
    }
    else
    {
        f_current_message2.resize(1);
    }

    if(sign_board_info.fGetSpecial(HNS_SGNBRD_SPECIAL_VSL))
    {
        vector<int> special_data = sign_board_info.fGetSpecialData(HNS_SGNBRD_SPECIAL_VSL);
        if(special_data.size() > 0)
        {
            f_vsl_display = special_data[0]-1;
            if(f_vsl_display == 0)
            {
                f_normal_display = 1;
            }
            else
            {
                f_normal_display = 0;
            }
        }
    }
    else
    {
        f_vsl_display = -1;
        f_normal_display = 0;
    }

    f_sign_config = sign_board_info;

    f_ntcip.fSetData(gk_vmsCharacterHeightPixels,f_sign_config.fGetCharacterHeight(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_vmsCharacterWidthPixels,f_sign_config.fGetCharacterWidth(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    f_ntcip.fSetData(gk_vmsSignHeightPixels,f_sign_config.fGetHeight(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_vmsSignWidthPixels,f_sign_config.fGetWidth(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    f_ntcip.fSetData(gk_dmsSignType,f_sign_config.fGetType() == HNS_BRD_TRAILER_FULL_MATRIX ? 6 : 4, HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    f_ntcip.fSetData(gk_vmsHorizontalPitch,f_sign_config.fGetBoard().fGetHPitch(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_vmsVerticalPitch,f_sign_config.fGetBoard().fGetVPitch(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_dmsSignHeight,f_sign_config.fGetVSize(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_dmsSignWidth,f_sign_config.fGetHSize(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_dmsHorizontalBorder,f_sign_config.fGetHBorder(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    f_ntcip.fSetData(gk_dmsVerticalBorder,f_sign_config.fGetVBorder(),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
}

HNS_DisplayBuffer MainWindow::fGenerateBuffer(const int64_t &runtime)
{
    HNS_DisplayBuffer result;
    string multi = f_current_message2[f_normal_display].fGetMULTI();

    //do this to refresh field data.  Could do this better, but this works for now.  Hope it doesn't slow things down too much.
    f_current_message2[f_normal_display].fSetMULTI(multi,&f_fonts,&f_graphics,nullptr);

    //joint board
    HNS_SignBoard combined_board(f_sign_config);

    //recover individual signboards
    vector<HNS_SignBoard> sign_boards(f_current_message2.size());
    HNS_SignBoard_Info sign_board_info = HNS_Message2::fGetDefaultSignBoardInfo();
    vector<int> special_data = sign_board_info.fGetSpecialData(HNS_SGNBRD_SPECIAL_DUAL_DISPLAY);
    int offset = 0;
    for(size_t ui=0;ui<sign_boards.size();ui++)
    {
        offset = 0;
        if(ui < special_data.size())
        {
            offset = special_data[ui] - 1;
        }

        sign_boards[ui] = f_current_message2[ui].fGetSignBoard(runtime);

        combined_board.fApplySubSignBoard(sign_boards[ui],offset);
    }

    f_current_signboard = combined_board;
    ui->PGE_SignBoard->fSetSignBoard(f_current_signboard);

    if(combined_board.fGetDisplayType() == HNS_DISP_BROADCAST)
    {
        result.fSetBuffer(combined_board.fGetDataStream().at(0));
    }

    return result;
}
