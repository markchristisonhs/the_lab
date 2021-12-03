#ifndef HNS_CHARACTER_PANEL_H
#define HNS_CHARACTER_PANEL_H

#include <QWidget>
#include <fonts.h>

namespace Ui {
class HNS_Character_Panel;
}

class HNS_Character_Panel : public QWidget
{
    Q_OBJECT

public:
    explicit HNS_Character_Panel(QWidget *parent = nullptr);
    ~HNS_Character_Panel();

    void fSetCharacter(const HNS_Character &character);
    void fClearCharacter();

    void fSetGraphic(const QImage &graphic);

    void fSelect(const bool &selected);
    bool fIsSelected() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void fClicked();

private:
    Ui::HNS_Character_Panel *ui;

    bool f_selected;

    QImage f_character_img;
};

#endif // HNS_CHARACTER_PANEL_H
