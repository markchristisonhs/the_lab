#include "hns_vkb.h"
#include "ui_hns_vkb.h"
#include <QKeyEvent>

HNS_VKB::HNS_VKB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HNS_VKB),
    f_focus_object(nullptr),
    f_mode(HNS_KB_CAPS)
{
    ui->setupUi(this);

    setObjectName("main_VKB");
    ui->PB_A->setObjectName("VKB_Button");
    ui->PB_B->setObjectName("VKB_Button");
    ui->PB_C->setObjectName("VKB_Button");
    ui->PB_D->setObjectName("VKB_Button");
    ui->PB_E->setObjectName("VKB_Button");
    ui->PB_F->setObjectName("VKB_Button");
    ui->PB_G->setObjectName("VKB_Button");
    ui->PB_H->setObjectName("VKB_Button");
    ui->PB_I->setObjectName("VKB_Button");
    ui->PB_J->setObjectName("VKB_Button");
    ui->PB_K->setObjectName("VKB_Button");
    ui->PB_L->setObjectName("VKB_Button");
    ui->PB_M->setObjectName("VKB_Button");
    ui->PB_N->setObjectName("VKB_Button");
    ui->PB_O->setObjectName("VKB_Button");
    ui->PB_P->setObjectName("VKB_Button");
    ui->PB_Q->setObjectName("VKB_Button");
    ui->PB_R->setObjectName("VKB_Button");
    ui->PB_S->setObjectName("VKB_Button");
    ui->PB_T->setObjectName("VKB_Button");
    ui->PB_U->setObjectName("VKB_Button");
    ui->PB_V->setObjectName("VKB_Button");
    ui->PB_W->setObjectName("VKB_Button");
    ui->PB_X->setObjectName("VKB_Button");
    ui->PB_Y->setObjectName("VKB_Button");
    ui->PB_Z->setObjectName("VKB_Button");
    ui->PB_Apostrophe->setObjectName("VKB_Button");
    ui->PB_BANG->setObjectName("VKB_Button");
    ui->PB_Bkspc->setObjectName("VKB_Button");
    ui->PB_Comma->setObjectName("VKB_Button");
    ui->PB_Dash->setObjectName("VKB_Button");
    ui->PB_Period->setObjectName("VKB_Button");
    ui->PB_Ampersand->setObjectName("VKB_Button");
    ui->PB_Return->setObjectName("VKB_Button");
    ui->PB_Semicolon->setObjectName("VKB_Button");
    ui->PB_Shift->setObjectName("VKB_Button");
    ui->PB_Space->setObjectName("VKB_Button");
    ui->PB_Symbols->setObjectName("VKB_Button");

    setAutoFillBackground(true);
    setAttribute(Qt::WA_StyledBackground);
    ui->PB_Shift->setChecked(true);
    ui->PB_Symbols->setChecked(true);
    f_mode = HNS_KB_CAPS;
    fUpdateKB();
}

HNS_VKB::~HNS_VKB()
{
    delete ui;
}

void HNS_VKB::setFocusObject(QObject *object)
{
    f_focus_object = object;
}

QObject *HNS_VKB::getFocusObject()
{
    return f_focus_object;
}

void HNS_VKB::fSetNarrowMode(const int &x, const int &y, const bool &mode)
{
    if(mode)
    {
        setGeometry(x,y,1140,311);

        ui->PB_Q->setGeometry(42,11,83,66);
        ui->PB_W->setGeometry(139,11,83,66);
        ui->PB_E->setGeometry(236,11,83,66);
        ui->PB_R->setGeometry(333,11,83,66);
        ui->PB_T->setGeometry(430,11,83,66);
        ui->PB_Y->setGeometry(527,11,83,66);
        ui->PB_U->setGeometry(624,11,83,66);
        ui->PB_I->setGeometry(721,11,83,66);
        ui->PB_O->setGeometry(818,11,83,66);
        ui->PB_P->setGeometry(915,11,83,66);
        ui->PB_Bkspc->setGeometry(1012,11,83,66);

        ui->PB_A->setGeometry(42,87,83,66);
        ui->PB_S->setGeometry(139,87,83,66);
        ui->PB_D->setGeometry(236,87,83,66);
        ui->PB_F->setGeometry(333,87,83,66);
        ui->PB_G->setGeometry(430,87,83,66);
        ui->PB_H->setGeometry(527,87,83,66);
        ui->PB_J->setGeometry(624,87,83,66);
        ui->PB_K->setGeometry(721,87,83,66);
        ui->PB_L->setGeometry(818,87,83,66);
        ui->PB_Return->setGeometry(915,87,180,66);

        ui->PB_Shift->setGeometry(42,163,83,66);
        ui->PB_Z->setGeometry(139,163,83,66);
        ui->PB_X->setGeometry(236,163,83,66);
        ui->PB_C->setGeometry(333,163,83,66);
        ui->PB_V->setGeometry(430,163,83,66);
        ui->PB_B->setGeometry(527,163,83,66);
        ui->PB_N->setGeometry(624,163,83,66);
        ui->PB_M->setGeometry(721,163,83,66);
        ui->PB_Semicolon->setGeometry(818,163,83,66);
        ui->PB_Apostrophe->setGeometry(915,163,83,66);
        ui->PB_Dash->setGeometry(1012,163,83,66);

        ui->PB_Symbols->setGeometry(42,239,83,66);
        ui->PB_BANG->setGeometry(139,239,83,66);
        ui->PB_Space->setGeometry(236,239,568,66);
        ui->PB_Comma->setGeometry(818,239,83,66);
        ui->PB_Period->setGeometry(915,239,83,66);
        ui->PB_Ampersand->setGeometry(1012,239,83,66);
    }
    else
    {
        move(0,360);
    }
}

void HNS_VKB::fSetKeyMode(const type_keyboard_mode &mode)
{
    if(mode >= HNS_KB_NORMAL && mode <= HNS_KB_SYM)
    {
        f_mode = mode;

        fUpdateKB();
    }
}

void HNS_VKB::on_PB_Q_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_1);
    }
    else
    {
        keyClicked(Qt::Key_Q);
    }
}

void HNS_VKB::on_PB_W_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_2);
    }
    else
    {
        keyClicked(Qt::Key_W);
    }
}

void HNS_VKB::on_PB_E_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_3);
    }
    else
    {
        keyClicked(Qt::Key_E);
    }
}

void HNS_VKB::on_PB_R_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_4);
    }
    else
    {
        keyClicked(Qt::Key_R);
    }
}

void HNS_VKB::on_PB_T_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_5);
    }
    else
    {
        keyClicked(Qt::Key_T);
    }
}

void HNS_VKB::on_PB_Y_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_6);
    }
    else
    {
        keyClicked(Qt::Key_Y);
    }
}

void HNS_VKB::on_PB_U_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_7);
    }
    else
    {
        keyClicked(Qt::Key_U);
    }
}

void HNS_VKB::on_PB_I_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_8);
    }
    else
    {
        keyClicked(Qt::Key_I);
    }
}

void HNS_VKB::on_PB_O_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_9);
    }
    else
    {
        keyClicked(Qt::Key_O);
    }
}

void HNS_VKB::on_PB_P_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_0);
    }
    else
    {
        keyClicked(Qt::Key_P);
    }
}

void HNS_VKB::on_PB_Bkspc_clicked()
{
    keyClicked(Qt::Key_Backspace);
}

void HNS_VKB::on_PB_A_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_BraceLeft);
    }
    else
    {
        keyClicked(Qt::Key_A);
    }
}

void HNS_VKB::on_PB_S_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_BraceRight);
    }
    else
    {
        keyClicked(Qt::Key_S);
    }
}

void HNS_VKB::on_PB_D_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Question);
    }
    else
    {
        keyClicked(Qt::Key_D);
    }
}

void HNS_VKB::on_PB_F_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Bar);
    }
    else
    {
        keyClicked(Qt::Key_F);
    }
}

void HNS_VKB::on_PB_G_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_ParenLeft);
    }
    else
    {
        keyClicked(Qt::Key_G);
    }
}

void HNS_VKB::on_PB_H_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_ParenRight);
    }
    else
    {
        keyClicked(Qt::Key_H);
    }
}

void HNS_VKB::on_PB_J_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_NumberSign);
    }
    else
    {
        keyClicked(Qt::Key_J);
    }
}

void HNS_VKB::on_PB_K_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_At);
    }
    else
    {
        keyClicked(Qt::Key_K);
    }
}

void HNS_VKB::on_PB_L_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Slash);
    }
    else
    {
        keyClicked(Qt::Key_L);
    }
}

void HNS_VKB::on_PB_Return_clicked()
{
    keyClicked(Qt::Key_Enter);
}

void HNS_VKB::on_PB_Shift_clicked()
{
    switch(f_mode)
    {
    case HNS_KB_NORMAL:
        f_mode = HNS_KB_CAPS;
        ui->PB_Shift->setChecked(true);
        ui->PB_Symbols->setChecked(false);
        break;
    case HNS_KB_CAPS:
        ui->PB_Shift->setChecked(false);
        ui->PB_Symbols->setChecked(false);
        f_mode = HNS_KB_NORMAL;
        break;
    case HNS_KB_SYM:
        ui->PB_Shift->setChecked(true);
        ui->PB_Symbols->setChecked(false);
        f_mode = HNS_KB_CAPS;
        break;
    }

    fUpdateKB();
}

void HNS_VKB::on_PB_Z_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_BracketLeft);
    }
    else
    {
        keyClicked(Qt::Key_Z);
    }
}

void HNS_VKB::on_PB_X_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_BracketRight);
    }
    else
    {
        keyClicked(Qt::Key_X);
    }
}

void HNS_VKB::on_PB_C_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Less);
    }
    else
    {
        keyClicked(Qt::Key_C);
    }
}

void HNS_VKB::on_PB_V_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Greater);
    }
    else
    {
        keyClicked(Qt::Key_V);
    }
}

void HNS_VKB::on_PB_B_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Equal);
    }
    else
    {
        keyClicked(Qt::Key_B);
    }
}

void HNS_VKB::on_PB_N_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_QuoteDbl);
    }
    else
    {
        keyClicked(Qt::Key_N);
    }
}

void HNS_VKB::on_PB_M_clicked()
{
    if(f_mode == HNS_KB_SYM)
    {
        keyClicked(Qt::Key_Colon);
    }
    else
    {
        keyClicked(Qt::Key_M);
    }
}

void HNS_VKB::on_PB_Semicolon_clicked()
{
    keyClicked(Qt::Key_Semicolon);
}

void HNS_VKB::on_PB_Apostrophe_clicked()
{
    keyClicked(Qt::Key_Apostrophe);
}

void HNS_VKB::on_PB_Dash_clicked()
{
    keyClicked(Qt::Key_Minus);
}

void HNS_VKB::on_PB_Symbols_clicked()
{

    switch(f_mode)
    {
    case HNS_KB_NORMAL:
        ui->PB_Shift->setChecked(false);
        ui->PB_Symbols->setChecked(true);
        f_mode = HNS_KB_SYM;
        break;
    case HNS_KB_CAPS:
        ui->PB_Shift->setChecked(false);
        ui->PB_Symbols->setChecked(true);
        f_mode = HNS_KB_SYM;
        break;
    case HNS_KB_SYM:
        ui->PB_Shift->setChecked(false);
        ui->PB_Symbols->setChecked(false);
        f_mode = HNS_KB_CAPS;
        break;
    }

    fUpdateKB();
}

void HNS_VKB::on_PB_BANG_clicked()
{
    keyClicked(Qt::Key_Exclam);
}

void HNS_VKB::on_PB_Space_clicked()
{
    keyClicked(Qt::Key_Space);
}

void HNS_VKB::on_PB_Comma_clicked()
{
    keyClicked(Qt::Key_Comma);
}

void HNS_VKB::on_PB_Period_clicked()
{
    keyClicked(Qt::Key_Period);
}

void HNS_VKB::on_PB_Ampersand_clicked()
{
    keyClicked(Qt::Key_Ampersand);
}

void HNS_VKB::keyClicked(const int &key)
{
    int shifted_key = key;
    if(key >= Qt::Key_A && key <= Qt::Key_Z)
    {
        if(f_mode == HNS_KB_NORMAL)
        {
            shifted_key += 0x20;
        }
    }   

    QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress,shifted_key,Qt::NoModifier, QString(shifted_key));
    if(f_focus_object != nullptr)
    {
        QGuiApplication::postEvent(f_focus_object,event);

        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, shifted_key, Qt::NoModifier, QString(shifted_key));
        QGuiApplication::postEvent(f_focus_object, releaseEvent);
    }
}

void HNS_VKB::fUpdateKB()
{
    switch(f_mode)
    {
    case HNS_KB_NORMAL:
        ui->PB_A->setText("a");
        ui->PB_B->setText("b");
        ui->PB_C->setText("c");
        ui->PB_D->setText("d");
        ui->PB_E->setText("e");
        ui->PB_F->setText("f");
        ui->PB_G->setText("g");
        ui->PB_H->setText("h");
        ui->PB_I->setText("i");
        ui->PB_J->setText("j");
        ui->PB_K->setText("k");
        ui->PB_L->setText("l");
        ui->PB_M->setText("m");
        ui->PB_N->setText("n");
        ui->PB_O->setText("o");
        ui->PB_P->setText("p");
        ui->PB_Q->setText("q");
        ui->PB_R->setText("r");
        ui->PB_S->setText("s");
        ui->PB_T->setText("t");
        ui->PB_U->setText("u");
        ui->PB_V->setText("v");
        ui->PB_W->setText("w");
        ui->PB_X->setText("x");
        ui->PB_Y->setText("y");
        ui->PB_Z->setText("z");
        ui->PB_Symbols->setText("123");
        break;
    case HNS_KB_CAPS:
        ui->PB_A->setText("A");
        ui->PB_B->setText("B");
        ui->PB_C->setText("C");
        ui->PB_D->setText("D");
        ui->PB_E->setText("E");
        ui->PB_F->setText("F");
        ui->PB_G->setText("G");
        ui->PB_H->setText("H");
        ui->PB_I->setText("I");
        ui->PB_J->setText("J");
        ui->PB_K->setText("K");
        ui->PB_L->setText("L");
        ui->PB_M->setText("M");
        ui->PB_N->setText("N");
        ui->PB_O->setText("O");
        ui->PB_P->setText("P");
        ui->PB_Q->setText("Q");
        ui->PB_R->setText("R");
        ui->PB_S->setText("S");
        ui->PB_T->setText("T");
        ui->PB_U->setText("U");
        ui->PB_V->setText("V");
        ui->PB_W->setText("W");
        ui->PB_X->setText("X");
        ui->PB_Y->setText("Y");
        ui->PB_Z->setText("Z");
        ui->PB_Symbols->setText("123");
        break;
    case HNS_KB_SYM:
        ui->PB_A->setText("{");
        ui->PB_B->setText("=");
        ui->PB_C->setText("<");
        ui->PB_D->setText("?");
        ui->PB_E->setText("3");
        ui->PB_F->setText("|");
        ui->PB_G->setText("(");
        ui->PB_H->setText(")");
        ui->PB_I->setText("8");
        ui->PB_J->setText("#");
        ui->PB_K->setText("@");
        ui->PB_L->setText("/");
        ui->PB_M->setText(":");
        ui->PB_N->setText("\"");
        ui->PB_O->setText("9");
        ui->PB_P->setText("0");
        ui->PB_Q->setText("1");
        ui->PB_R->setText("4");
        ui->PB_S->setText("}");
        ui->PB_T->setText("5");
        ui->PB_U->setText("7");
        ui->PB_V->setText(">");
        ui->PB_W->setText("2");
        ui->PB_X->setText("]");
        ui->PB_Y->setText("6");
        ui->PB_Z->setText("[");
        ui->PB_Symbols->setText("ABC");
        break;
    }
}
