#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "message.h"
#include <string>
#include <dirent.h>
#include <sstream>
#include "hns_qt_toolkit.h"
#include <hns_message_preview.h>
#include <QDir>
#include <structures.h>
#include <QStandardPaths>
#include <QSettings>
#include <QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>
#include <hns_ntcip_xml.h>
#include <HNS_NTCIP_MIB_Strings.h>
#include <message2.h>

#include "multi.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_current_page(0)
    , f_port(nullptr)
    , f_commlink(nullptr)
    , f_link(nullptr)
    , f_rs485_bus(nullptr)
    , f_timer(new QTimer(this))
{
    HNS_IP_Settings dummy;
    HNS_Message_Preview dummy2;
    bool generate_file;

    fLoadNTCIP();
    //fLoadFonts();
    fUpdateFontFromNTCIP();
    fSignVoltTest();

    connect(f_timer,SIGNAL(timeout()),SLOT(fTickTock()));
    f_timer->setInterval(100);
    f_timer->start();

#ifdef HNS_ANDROID
    QString q_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "graphics";
    QString generate_path;
    if(!QFileInfo::exists(q_path))
    {
        generate_path = q_path;
        q_path = "assets:/graphics";
        generate_file = true;
    }
    string path = q_path.toStdString();
#else
    string path = getenv("HOME") + string("/HNS/graphics");
    QString generate_path = "";
#endif
    fLoadGraphics(path,"fmst",generate_file,generate_path.toStdString());
    fLoadTrailers();

    HNS_SignBoard::fSetFonts(f_fonts);
    HNS_SignBoard::fSetGraphics(f_factory_graphics);
    HNS_Message2::fSetDefaultSignBoardInfo(f_signboard_library[1].fGetSignBoardInfo());
    HNS_Message2::fSetDefaultFont(1);

    f_field_data.fSetSpeed(130,true);
    f_field_data.fSetTemperature(32,true);
    ui->setupUi(this);

    ui->GV_Preview->fClear();

    ui->GV_Preview->fSetFonts(f_fonts);
    ui->GV_Preview->fSetGraphics(f_factory_graphics);

    QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();

    for(int i=0;i<port_list.size();i++)
    {
        ui->CMB_Serial->addItem(port_list.at(i).portName());
    }
    ui->CMB_Serial->addItem("LocalHost");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PTE_Input_textChanged()
{
    type_multi_syntax_error multi_error = HNS_MULTI_SYNTAX_ERROR_NONE;
    int error = HNS_MULTI_PARSER_NO_ERROR;
    f_field_data.fSetTime(time(nullptr));
    string tempstring = ui->PTE_Input->toPlainText().toStdString();
    error = f_current_message.fSetMULTI(tempstring, &f_fonts, &f_factory_graphics,&f_field_data,&multi_error);

    if(multi_error != HNS_MULTI_SYNTAX_ERROR_NONE)
    {
        ui->PTE_Console->appendPlainText(QString("Error detected %1").arg(multi_error));
    }

//    HNS_MESSAGE_V2::HNS_Message message_v2(tempstring);

    fUpdate();
}

void MainWindow::fUpdate()
{
    ui->LE_PageNo->setText(QString::number(f_current_page+1));

    if(f_current_message.fGetNumPages() > 0)
    {
        ui->GV_Preview->fSetMessage(f_current_message);
        ui->GV_Preview->fSelectPage(f_current_page);

        ui->GV_Preview_2->fSetSignBoard(f_current_message.fGetSignBoard(ui->LE_Time->text().toInt()));
    }
    else
    {
        ui->GV_Preview->fClear();
    }
}

void MainWindow::fLoadFonts()
{
    QStringRef tempstring_ref;
    QXmlStreamReader xml;
#ifdef HNS_ANDROID
    bool generate_file = false;
    QString q_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "fonts" + QDir::separator() + "font.xml";
    if(!QFileInfo::exists(q_path))
    {
        q_path = "assets:/fonts/font.xml";
        generate_file = true;
    }
    string path = q_path.toStdString();
#else
    string path = getenv("HOME") + string("/HNS/fonts/font.xml");
#endif
    QFile file(path.c_str());
    QXmlStreamReader::TokenType token_type;
    bool at_end = false;

    if(file.open(QIODevice::ReadWrite))
    {
        xml.setDevice(&file);
        do
        {
            token_type = xml.readNext();
            if(token_type == QXmlStreamReader::Invalid || token_type == QXmlStreamReader::EndDocument)
            {
                at_end = true;
            }
            else if(token_type == QXmlStreamReader::StartDocument)
            {
                tempstring_ref = xml.documentVersion();
            }
            else if(token_type == QXmlStreamReader::DTD)
            {
                tempstring_ref = xml.dtdName();
            }
            else if(token_type == QXmlStreamReader::StartElement)
            {
                tempstring_ref = xml.name();
                if(xml.name() == "HNS_Font")
                {
                    fLoadFont(&xml);
                }
                else
                {
                    at_end = true;
                }
            }
            else if(token_type == QXmlStreamReader::EndElement)
            {

            }
        }while(!at_end);
    }

#ifdef HNS_ANDROID
    if(generate_file)
    {
        QString copy_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "fonts";
        if(!QFileInfo::exists(copy_path))
        {
            QDir dir = QDir(copy_path);
            dir.mkpath(copy_path);
        }
        copy_path = copy_path + QDir::separator() + "font.xml";
        QFile::copy(QString::fromStdString(path),
                    copy_path);
    }
#endif
}

bool MainWindow::fLoadFont(QXmlStreamReader *xml)
{
    bool success = true;
    bool found_end = false;
    QXmlStreamReader::TokenType token_type;
    QStringRef tempstring_ref;
    QString font_name, tempstring;
    int font_number;
    HNS_Font current_font;
    QImage font_image;

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(xml->name().contains(QString("font"),Qt::CaseInsensitive))
            {
                font_name = tempstring = xml->name().toString();
                font_number = tempstring.remove(QString("font"), Qt::CaseInsensitive).toInt();
                font_name = getenv("HOME") + QString("/HNS/fonts/") + font_name + QString(".bmp");

                current_font.fSetFontNumber(font_number);

                font_image = QImage(font_name,"bmp");

                f_characters.resize(f_characters.size()+1);
                f_character_numbers.resize(f_character_numbers.size()+1);
                if(fLoadCharacters(xml, font_image, current_font))
                {
                    //MainWindow::f_machine.fAddFont(current_font);
                    f_fonts.push_back(current_font);
                    current_font = HNS_Font();
                }
                else
                {
                    found_end = true;
                    success = false;
                }
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            found_end = true;
        }
        else if(token_type == QXmlStreamReader::Characters)
        {
            tempstring_ref = xml->text();
        }
        else
        {
            success = false;
            found_end = true;
        }
    }while(!found_end);

    return success;
}

bool MainWindow::fLoadCharacters(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font)
{
    bool success = true;
    bool found_end = false;
    QXmlStreamReader::TokenType token_type;

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(xml->name() == QString("character"))
            {
                fLoadCharacter(xml,font_image,font);
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            found_end = true;
        }
        else if(token_type == QXmlStreamReader::Characters)
        {

        }
        else
        {
            found_end = true;
            success = false;
        }
    }while(!found_end);

    if(success)
    {

    }

    return success;
}

bool MainWindow::fLoadCharacter(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font)
{
    bool success = true;
    bool found_end = false;
    int character_number;
    int x=0,y=0,height=0,width=0;
    QXmlStreamReader::TokenType token_type;
    int state = -1; // -1 is looking for a tag, 0 is character number, 1 is x, 2 is y, 3 is height, and 4 is width
    HNS_Character current_character;
    QImage temp_image;

    do
    {
        token_type = xml->readNext();
        if(token_type == QXmlStreamReader::StartElement)
        {
            if(state == -1)
            {
                if(xml->name() == QString("char_number"))
                {
                    state = 0;
                }
                else if(xml->name() == QString("x"))
                {
                    state = 1;
                }
                else if(xml->name() == QString("y"))
                {
                    state = 2;
                }
                else if(xml->name() == QString("height"))
                {
                    state = 3;
                }
                else if(xml->name() == QString("width"))
                {
                    state = 4;
                }
            }
            else
            {
                found_end = true;
                success = false;
            }
        }
        else if(token_type == QXmlStreamReader::EndElement)
        {
            if(state == -1)
            {
                found_end = true;
            }
            else
            {
                state = -1;
            }
        }
        else if(token_type == QXmlStreamReader::Characters)
        {
            if(!xml->isWhitespace())
            {
                switch(state)
                {
                case 0:
                    character_number = xml->text().toInt();
                    break;
                case 1:
                    x = xml->text().toInt();
                    break;
                case 2:
                    y = xml->text().toInt();
                    break;
                case 3:
                    height = xml->text().toInt();
                    break;
                case 4:
                    width = xml->text().toInt();
                    break;
                }
            }
        }
        else
        {
            found_end = true;
            success = false;
        }
    }while(!found_end);

    if(success)
    {
        bool *data = nullptr;
        temp_image = font_image.copy(x,y,width,height);
        data = new bool[temp_image.width() * temp_image.height()];
        f_characters[f_characters.size()-1].push_back(temp_image);
        f_character_numbers[f_character_numbers.size()-1].push_back(character_number);
        ConvertQImageToBoolArray(temp_image,Qt::white,data,temp_image.width() * temp_image.height());
        current_character.fSetData(data,temp_image.width() * temp_image.height(),width,height);
        current_character.fSetCharacterNumber(character_number);
        font.fAddCharacter(current_character);
        delete [] data;
    }

    return success;
}

bool MainWindow::fLoadGraphics(const string &file_path, const string &trailer_name, const bool &generate_file, const string &generate_path)
{
    bool success = true;
    QString tempstring2;
    vector<string> tempstrings;
    size_t itemp;
    stringstream *parse = nullptr;
    string tempstring3;
    string tempstring;
    string factory_path = file_path + "/factory/";
    QString path = QString::fromStdString(file_path) + QDir::separator() + "factory" + QDir::separator() + QString::fromStdString(trailer_name) + QDir::separator();
    QString full_path;
    HNS_Graphic temp_graphic;
    QString copy_path = QString::fromStdString(generate_path) + QDir::separator() + "factory" + QDir::separator() + QString::fromStdString(trailer_name);
    QString full_copy_path;

    if(!QFileInfo::exists(copy_path))
    {
        QDir dir = QDir(copy_path);
        dir.mkpath(copy_path);
    }

    struct dirent *entry;
    DIR *dir = opendir(factory_path.c_str());

    if(dir != nullptr)
    {
        while((entry = readdir(dir)) != nullptr)
        {
            tempstring3 = entry->d_name;
            if(tempstring3.find(trailer_name) != string::npos)
            {
                tempstrings.push_back(factory_path + tempstring3);
            }
        }
    }

    closedir(dir);

    QDir dir2(QString::fromStdString(factory_path) + QDir::separator() + QString::fromStdString(trailer_name));
    QStringList list = dir2.entryList(QDir::Files,QDir::Name);

    sort(tempstrings.begin(), tempstrings.end());

    //for(auto tempstring : tempstrings)
    for(int i=0;i<list.size();i++)
    {
        tempstring2 = list[i];
        //full_path = path + tempstring2;
        full_path = path + list[i];
        temp_graphic.fSetBitmap(ConvertQImagetoHNS_Bitmap(QImage(full_path,"bmp")));
#ifdef HNS_ANDROID
        if(generate_file)
        {
            full_copy_path = copy_path + list[i];
            QFile::copy(full_path,
                        full_copy_path);
        }
#endif
        tempstring = tempstring2.toStdString();
        itemp = tempstring.find_last_of('/') + 1;
        if(itemp != string::npos)
        {
            tempstring = tempstring.substr(itemp);
        }
        itemp = tempstring.find_first_of('.');
        if(itemp != string::npos)
        {
            tempstring = tempstring.substr(0,itemp);
        }
        itemp = tempstring.length();
        for (string::reverse_iterator it = tempstring.rbegin();it != tempstring.rend();++it)
        {
            if(isdigit(*it))
            {
                itemp--;
            }
            else
            {
                break;
            }
        }
        tempstring = tempstring.substr(itemp);
        parse = new stringstream(tempstring);

        (*parse) >> itemp;
        temp_graphic.fSetGraphicNumber(static_cast<int>(itemp));
        tempstring2 = QString("g%1").arg(itemp);
        temp_graphic.fSetName(tempstring2.toStdString());
        f_factory_graphics.push_back(temp_graphic);
        delete parse;
        parse = nullptr;
    }

    return success;
}

void MainWindow::fLoadTrailers()
{
    int num_trailers, num_boards;
    int num_bytes, leds_per_byte;
    int itemp;
    int vpitch,hpitch;
    bool split;
    QString special_string;
    int split_width;
    HNS_Board temp_board;
    HNS_SignBoard temp_signboard;
    QString tempstring,tempstring2;
    QString name, board_name;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "Trailer",rc_path;
    QString full_path = path + QDir::separator() + "config.ini";
    if(!QFile(full_path).exists())
    {
        if(!QFileInfo::exists(path))
        {
            QDir dir = QDir(path);
            dir.mkpath(path);
        }
        rc_path = "assets:/Trailer/config.ini";
        QFile(rc_path).copy(full_path);
    }
    QSettings settingsfile(full_path,QSettings::NativeFormat);

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
//    if(f_signboard_library.size() > 0)
//    {
//        f_trailer_select = 0;
//    }
}

void MainWindow::fLoadNTCIP()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "NTCIP.xml";

    if(QFileInfo::exists(path))
    {
        HNS_NTCIP_XML xml_file(path);

        xml_file.fParseNTCIPFile(&f_ntcip);
    }
}

void MainWindow::fSignVoltTest()
{
    unsigned char data = 132;
    double batt_voltage = static_cast<double>(data)/10.0;

    f_ntcip.fSetData(gk_signVolts,static_cast<int>(batt_voltage * 100),HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
}

void MainWindow::fUpdateFontFromNTCIP()
{
    int data,width,char_number;
    QString tempstring, data_string;

    HNS_Character *temp_char = nullptr;

    vector<uint16_t> versionID;
    vector<size_t> notUsed;
    vector<unsigned char> data_vec;
    type_ntcip_error error;

    f_fonts.clear();

    data = f_ntcip.fGetIntData(gk_numFonts,0,&error);
    if(error == HNS_NTCIP_NOERROR)
    {
        f_fonts.resize(data);
    }

    for(size_t i=0;i<f_fonts.size();i++)
    {
        tempstring = QString::fromStdString(gk_fontNumber) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),i+1);
        f_fonts[i].fSetFontNumber(data);

        tempstring = QString::fromStdString(gk_fontName) + QString(".%1").arg(i+1);
        data_string = QString::fromStdString(f_ntcip.fGetStringData(tempstring.toStdString(),QString("Font %1").arg(i+1).toStdString()));
        f_fonts[i].fSetFontName(data_string.toStdString());

        tempstring = QString::fromStdString(gk_fontHeight) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),0);
        f_fonts[i].fSetFontHeight(data);

        tempstring = QString::fromStdString(gk_fontCharSpacing) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),0);
        f_fonts[i].fSetCharSpacing(data);

        tempstring = QString::fromStdString(gk_fontLineSpacing) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),0);
        f_fonts[i].fSetLineSpacing(data);

        tempstring = QString::fromStdString(gk_fontVersionID) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),0);
        versionID.push_back(static_cast<uint16_t>(data));

        tempstring = QString::fromStdString(gk_fontStatus) + QString(".%1").arg(i+1);
        data = f_ntcip.fGetIntData(tempstring.toStdString(),1);
        if(data == 1)
        {
            notUsed.push_back(i);
        }

        for(size_t j=0;j<MAX_NUM_CHARACTERS;j++)
        {
            tempstring = QString::fromStdString(gk_characterNumber) + QString(".%1.%2").arg(i+1).arg(j+1);
            char_number = f_ntcip.fGetIntData(tempstring.toStdString());

            if(char_number == 0)
            {
                //no such character, usually used for characters that aren't used.
                continue;
            }
            else
            {
                tempstring = QString::fromStdString(gk_characterWidth) + QString(".%1.%2").arg(i+1).arg(j+1);
                width = f_ntcip.fGetIntData(tempstring.toStdString());

                if(width > 0)
                {
                    temp_char = new HNS_Character();

                    tempstring = QString::fromStdString(gk_characterBitmap) + QString(".%1.%2").arg(i+1).arg(j+1);
                    data_vec = f_ntcip.fGetData(tempstring.toStdString());

                    temp_char->fSetData(data_vec,width,f_fonts[i].fGetFontHeight());
                    temp_char->fSetCharacterNumber(char_number);
                    f_fonts[i].fAddCharacter(*temp_char);

                    delete temp_char;
                    temp_char = nullptr;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    for(size_t i = 0;i<notUsed.size();i++)
    {
        f_fonts.erase(f_fonts.begin()+notUsed[i]);

        for(size_t j=i+1;j<notUsed.size();j++)
        {
            notUsed[j]--;
        }
    }
}

void MainWindow::on_PB_Update_clicked()
{
    on_PTE_Input_textChanged();
}

void MainWindow::on_PB_NextPage_clicked()
{
    if(static_cast<size_t>(f_current_page) < (f_current_message.fGetNumPages() -1 ))
    {
        f_current_page++;
    }
    fUpdate();
}

void MainWindow::on_PB_PrevPage_clicked()
{
    if(f_current_page >= 1)
    {
        f_current_page--;
    }
    fUpdate();
}

void MainWindow::on_PB_Send_clicked()
{
    vector<unsigned char> current_buffer = f_current_message.fGetPage(f_current_page).fGetSignBoard().fGetDataStream().at(0);
    if(f_rs485_bus != nullptr)
    {
        f_rs485_bus->fDisplayData(0xFE,current_buffer);
    }
}

void MainWindow::on_PB_Connect_clicked()
{
    QString port_location;
    if(ui->CMB_Serial->currentText() == "LocalHost")
    {
        port_location = "LocalHost";
    }
    else
    {
        QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();
        //find desired port
        for(int i=0;i<port_list.size();i++)
        {
            if(port_list.at(i).portName() == ui->CMB_Serial->currentText())
            {
                port_location = port_list.at(i).systemLocation();
                break;
            }
        }
    }

    if(!port_location.isEmpty() && (f_port == nullptr))
    {
        if(port_location == "LocalHost")
        {
            f_commlink = new LocalHostLink(this);
            f_link = f_commlink;
        }
        else
        {
            f_port = new HNS_SerialPort(port_location.toStdString(),HNS_SER_BAUD115200);
            f_link = f_port;
            if (f_port->fIsOpened()) {
                ui->PTE_Console->appendPlainText("Port opened");
            } else {
                QMessageBox::critical(this, tr("Error"), "");
            }
        }
        f_rs485_bus = new HNS_SMCBus2();

        ui->PB_Disconnect->setEnabled(true);
        ui->PB_Connect->setEnabled(false);
        ui->PB_Send->setEnabled(true);
    }
}

void MainWindow::on_PB_Disconnect_clicked()
{
    if(f_port != nullptr)
    {
        delete f_port;
        f_port = nullptr;
        f_link = nullptr;
        delete f_rs485_bus;
        f_rs485_bus = nullptr;

        ui->PB_Disconnect->setEnabled(false);
        ui->PB_Connect->setEnabled(true);
        ui->PB_Send->setEnabled(false);
    }
}

void MainWindow::on_SLD_Time_valueChanged(int value)
{
    ui->LE_Time->setText(QString("%1").arg(value));

    fUpdate();
}


void MainWindow::on_LE_Time_editingFinished()
{
    int time = ui->LE_Time->text().toInt();
    ui->SLD_Time->setSliderPosition(time);

    fUpdate();
}


void MainWindow::on_PB_Create_Sign_clicked()
{
    QString tempstring;
    size_t test;
    HNS_DisplayBuffer current_buffer;
    current_buffer.fSetBuffer(f_current_message.fGetSignBoard(0).fGetDataStream().at(0));
    vector<unsigned char> data = current_buffer.fGetBuffer();

    test = current_buffer.fGetSize();

    for(size_t ui=0;ui < data.size();ui++)
    {
        tempstring += QString("0x%1 ").arg(data[ui],2,16,QLatin1Char('0'));
    }
    ui->PTE_Console->appendPlainText(tempstring);
}


void MainWindow::on_PB_FontTest_clicked()
{
    vector<int> fonts_used = f_current_message.fGetFontsUsed();

    QString tempstring = "Fonts used are: ";

    for(size_t i=0;i<fonts_used.size();i++)
    {
        tempstring = tempstring + QString("%1 ").arg(fonts_used[i]);
    }
    ui->PTE_Console->appendPlainText(tempstring);
    qDebug() << tempstring;
}

void MainWindow::fTickTock()
{
    if(f_port != nullptr)
    {
        HNS_SMCBus_Command command_to_send;

        f_rs485_bus->fRunCycle();
        type_hns_smc_bus_error error = f_rs485_bus->fGetPendingCommand(command_to_send);
        if(error == HNS_SMC_BUS_NOERR && !command_to_send.fWasDataSent())
        {
            f_port->fWrite(command_to_send.fGetSendData().data(),command_to_send.fGetSendData().size());
            f_rs485_bus->fPendingCommandSent();
        }
    }
}
