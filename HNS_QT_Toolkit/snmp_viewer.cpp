#include "snmp_viewer.h"
#include "ui_snmp_viewer.h"

SNMP_Viewer::SNMP_Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SNMP_Viewer)
{
    ui->setupUi(this);

    ui->SPN_PDU->setEnabled(false);
    ui->PGE_PDU->setEnabled(false);
}

SNMP_Viewer::~SNMP_Viewer()
{
    delete ui;
}

void SNMP_Viewer::fSetSNMP_Packet(const SNMP_Packet &packet)
{
    ui->LE_Version->setText(QString::number(packet.fGetVersion()));
    ui->LE_Community->setText(QString::fromStdString(packet.fGetCommunity()));
    ui->LE_NumPDU->setText(QString::number(packet.fGetNumPDUs()));

    f_pdus.clear();
    for(size_t ui=0;ui<packet.fGetNumPDUs();ui++)
    {
        f_pdus.push_back(packet.fGetPDU(ui));
    }

    if(f_pdus.size() == 0)
    {
        ui->SPN_PDU->setEnabled(false);
        ui->PGE_PDU->setEnabled(false);
    }
    else
    {
        ui->SPN_PDU->setEnabled(true);
        ui->PGE_PDU->setEnabled(true);
        ui->SPN_PDU->setValue(0);
        ui->SPN_PDU->setMinimum(0);
        ui->SPN_PDU->setMaximum(f_pdus.size() - 1);
        fUpdate();
    }
}

void SNMP_Viewer::fUpdate()
{
    int index = ui->SPN_PDU->value();

    if((index >= 0) && (index < f_pdus.size()))
    {
        ui->PGE_PDU->fSetPDU(f_pdus[index]);
    }
}

void SNMP_Viewer::on_SPN_PDU_valueChanged(int /*arg1*/)
{
    fUpdate();
}
