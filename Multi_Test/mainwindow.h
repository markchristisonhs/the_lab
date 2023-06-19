#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "message.h"
#include "fonts.h"
#include <QXmlStreamReader>
#include <QSerialPort>
#include <serialport.h>
#include <HNS_CommLink.h>
#include <MessageBoard.h>
#include <SMC_Bus.h>
#include <LocalHostLink.h>
#include <NTCIP.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PTE_Input_textChanged();

    void on_PB_Update_clicked();

    void on_PB_NextPage_clicked();

    void on_PB_PrevPage_clicked();

    void on_PB_Send_clicked();

    void on_PB_Connect_clicked();

    void on_PB_Disconnect_clicked();

    void on_SLD_Time_valueChanged(int value);

    void on_LE_Time_editingFinished();

    void on_PB_Create_Sign_clicked();

    void on_PB_FontTest_clicked();

    void fTickTock();

private:
    Ui::MainWindow *ui;
    HNS_Message2 f_current_message;

    int f_current_page;
    std::vector <HNS_Font> f_fonts;

    std::vector<HNS_Graphic> f_factory_graphics;

    QVector <QVector<QImage> > f_characters;
    QVector <QVector<int> > f_character_numbers;

    QVector<HNS_Board> f_board_library;
    QVector<HNS_SignBoard> f_signboard_library;

    HNS_Field_Data f_field_data;

    void fUpdate();

    void fLoadFonts();
    bool fLoadFont(QXmlStreamReader *xml);
    bool fLoadCharacters(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font);
    bool fLoadCharacter(QXmlStreamReader *xml, QImage &font_image, HNS_Font &font);

    bool fLoadGraphics(const std::string &file_path, const std::string &trailer_name, const bool &generate_file, const std::string &generate_path);

    void fLoadTrailers();

    void fLoadNTCIP();
    void fSignVoltTest();
    void fUpdateFontFromNTCIP();

    HNS_SerialPort *f_port;
    LocalHostLink *f_commlink;
    HNS_CommLink *f_link;
    HNS_SMCBus2 *f_rs485_bus;
    NTCIP_Node f_ntcip;

    QTimer *f_timer;
};
#endif // MAINWINDOW_H
