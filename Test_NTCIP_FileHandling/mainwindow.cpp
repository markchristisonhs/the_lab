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

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PB_Save_clicked()
{
    if(f_ntcip.fIsDirty())
    {
        ui->PTE_Debug->appendPlainText("Save is needed");

        QString dir_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "NTCIP";
        if(!QFileInfo::exists(dir_path))
        {
            QDir dir = QDir(dir_path);
            dir.mkpath(dir_path);
        }
        QString new_path = dir_path + QDir::separator() + "NTCIP.xml.new";
        QString existing_path = dir_path + QDir::separator() + "NTCIP.xml";
        QString old_path = dir_path + QDir::separator() + "NTCIP.xml.old";

        HNS_NTCIP_XML xml_file(new_path);
        xml_file = HNS_NTCIP_XML(new_path);
        xml_file.fWriteNTCIPFile(&f_ntcip);

        if(QFileInfo::exists(old_path))
        {
            QFile::remove(old_path);
        }
        QFile::rename(existing_path,old_path);
        QFile::rename(new_path,existing_path);

        f_ntcip.fRememberSave();
    }
    else
    {
        ui->PTE_Debug->appendPlainText("Save not needed");
    }
}


void MainWindow::on_PB_Load_clicked()
{
    QString file_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + "NTCIP" + QDir::separator() + "NTCIP.xml";
    QString load_path = file_path;

    HNS_NTCIP_XML xml_file(load_path);

    bool result = xml_file.fParseNTCIPFile(&f_ntcip);
    f_ntcip.fRememberSave();

    ui->PTE_Debug->appendPlainText(QString("Loading Done: %1").arg(result ? "success" : "failure"));
}

void MainWindow::fKludge()
{
    vector<unsigned char> temp_vec(4,0);
    CalcCRC(temp_vec);

    HNS_intToByteArray(2);
    HNS_ByteArrayToint(temp_vec);

    HNS_IP_Settings::fIPStringtoByteArray("0.0.0.0");

    HNS_Color dummy_color;
}


void MainWindow::on_PB_Dirty_clicked()
{
    if(f_ntcip.fDoesChildExist(gk_dmsControlMode))
    {
        int test = f_ntcip.fGetIntData(gk_dmsControlMode);
        if(test == 4)
        {
            test = 3;
        }
        else
        {
            test = 4;
        }
        f_ntcip.fSetData(gk_dmsControlMode,test,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
    }
}

