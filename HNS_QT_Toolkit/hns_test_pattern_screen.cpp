#include "hns_test_pattern_screen.h"
#include "ui_hns_test_pattern_screen.h"

#include <hns_qt_toolkit.h>

using namespace std;

HNS_Test_Pattern_Screen::HNS_Test_Pattern_Screen(QWidget *parent, HNS_Machine_Interface *machine) :
    HNS_UI_Widget(parent, machine),
    ui(new Ui::HNS_Test_Pattern_Screen)
{
    ui->setupUi(this);

    ui->GV_TestPattern1->fSetFonts(*f_machine->fGetFonts());
    ui->GV_TestPattern1->fSetGraphics(*f_machine->fGetGraphics());

    ui->GV_TestPattern2->fSetFonts(*f_machine->fGetFonts());
    ui->GV_TestPattern2->fSetGraphics(*f_machine->fGetGraphics());

    QString multi;
    HNS_NTCIP_MessageActivationCode temp_act_code;
    vector <int> animation;

    if(HNS_Message2::fGetDefaultSignBoardInfo().fGetType() == HNS_BRD_TRAILER_FULL_MATRIX)
    {
        if(f_machine->fGetAnimations()->size() >= 8)
        {
            animation = f_machine->fGetAnimations()->at(7);
            multi = AnimationToMulti(animation,5000);
            temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,multi.toStdString(),"127.0.0.1");

            ui->GV_TestPattern1->fSetMessage(multi);
            ui->GV_TestPattern1->fSetActivationCode(temp_act_code);
        }
    }
    else
    {
        multi = "[JP2][FO4]^^^^^^^^[NP][JP3][FO4]^^^^^^^^[NP][JP4][FO4]^^^^^^^^";
        temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,multi.toStdString(),"127.0.0.1");

        ui->GV_TestPattern1->fSetMessage(multi);
        ui->GV_TestPattern1->fSetActivationCode(temp_act_code);
    }

    ui->GV_TestPattern2->fGenerateFontTest(1);
    temp_act_code = HNS_NTCIP_MessageActivationCode(0xFFFF,127,NTCIP_MESSAGE::MEM_TYPE_VOLATILE,1,ui->GV_TestPattern2->fGetCurrentMessage().fGetMULTI(),"127.0.0.1");
    ui->GV_TestPattern2->fSetActivationCode(temp_act_code);

    connect(ui->GV_TestPattern1,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_TestPattern1_clicked(QMouseEvent*)));
    connect(ui->GV_TestPattern2,SIGNAL(fClicked(QMouseEvent *)),this,SLOT(my_slot_TestPattern2_clicked(QMouseEvent *)));
}

HNS_Test_Pattern_Screen::~HNS_Test_Pattern_Screen()
{
    delete ui;
}

void HNS_Test_Pattern_Screen::fSetAccess(const type_user_level &/*access*/)
{

}

type_widget HNS_Test_Pattern_Screen::fGetType() const
{
    return HNS_WIDGET_TEST_PATTERN;
}

void HNS_Test_Pattern_Screen::on_PB_TestPattern_Run_clicked()
{
    HNS_Message2 temp_message(HNS_Message2::fGetDefaultSignBoardInfo());
    HNS_NTCIP_MessageActivationCode temp_act_code;
    QString tempstring;

    int selection = fGetTestPatternSelection();

    if(selection == 1)
    {
        tempstring = QString::fromStdString(ui->GV_TestPattern1->fGetCurrentMessage().fGetMULTI());
        temp_act_code = ui->GV_TestPattern1->fGetActivationCode();
    }
    else if(selection == 2)
    {
        tempstring = QString::fromStdString(ui->GV_TestPattern2->fGetCurrentMessage().fGetMULTI());
        temp_act_code = ui->GV_TestPattern2->fGetActivationCode();
    }
    else
    {
        return;
    }

    HNS_NTCIP_MessageIDCode id_code;

    id_code.fSetCRC(temp_act_code.fGetCRCAsVector());
    id_code.fSetMemoryType(temp_act_code.fGetMemoryType());
    id_code.fSetMessageNumber(temp_act_code.fGetMessageNo());

    emit SIG_Message(tempstring,id_code,HNS_PLAY_MESSAGE);

    emit SIG_GO_HOME();
}


void HNS_Test_Pattern_Screen::on_PB_TestPattern_Cancel_clicked()
{
    deleteLater();
}

void HNS_Test_Pattern_Screen::my_slot_TestPattern1_clicked(QMouseEvent *)
{
    fSetTestPatternSelection(1);
}

void HNS_Test_Pattern_Screen::my_slot_TestPattern2_clicked(QMouseEvent *)
{
    fSetTestPatternSelection(2);
}

void HNS_Test_Pattern_Screen::fSetTestPatternSelection(const int &select)
{
    ui->GV_TestPattern1->fSelect(select == 1);
    ui->GV_TestPattern2->fSelect(select == 2);
}

int HNS_Test_Pattern_Screen::fGetTestPatternSelection()
{
    if(ui->GV_TestPattern1->fIsSelected())
    {
        return 1;
    }
    else if(ui->GV_TestPattern2->fIsSelected())
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

