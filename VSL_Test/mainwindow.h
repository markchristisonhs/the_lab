#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NTCIP.h>
#include <fonts.h>
#include <graphics.h>
#include <message.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void fKludge();
    void fLoadNTCIP();
    void fInitFontsFromNTCIP();
    void fInitGraphicsFromNTCIP();

    void fLoadTrailers(const bool &reset = false);

    void fSetTrailer(const HNS_SignBoard_Info &sign_board_info);

    HNS_DisplayBuffer fGenerateBuffer(const int64_t &runtime);

    NTCIP_Node f_ntcip;

    std::vector <HNS_Font> f_fonts;
    std::vector <HNS_Graphic> f_graphics;
    std::vector <HNS_Message2> f_current_message2;

    QVector<HNS_Board> f_board_library;
    QVector<HNS_SignBoard_Info> f_signboard_library;
    int f_trailer_select;

    int f_vsl_display,f_normal_display;
    HNS_SignBoard_Info f_sign_config;
    HNS_SignBoard f_current_signboard;
};
#endif // MAINWINDOW_H
