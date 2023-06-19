#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <vector>
#include <QPainter>

#define FONT_MAX_CHARACTER_SIZE 25

typedef struct fcharEntry {
        uint16_t characterNumber;
        uint8_t characterWidth;
        uint8_t  characterBitMap[FONT_MAX_CHARACTER_SIZE];
} CHARACTER_ENTRY;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_PB_ConvertAll_clicked();

private:
    Ui::MainWindow *ui;

    QImage f_image;
    std::vector<QImage> f_font1_images, f_font2_images, f_font3_images, f_font4_images, f_font5_images, f_font6_images, f_font7_images;

    QGraphicsScene *f_scene;
    QGraphicsPixmapItem *f_item;
    std::vector<QGraphicsPixmapItem *> f_items;

    bool fConvertImage(const int &font, const int &char_no, const QString &filename, QImage &image);
};

#endif // MAINWINDOW_H
