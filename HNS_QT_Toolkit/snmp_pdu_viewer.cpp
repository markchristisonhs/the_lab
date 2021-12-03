#include "snmp_pdu_viewer.h"
#include "ui_snmp_pdu_viewer.h"

SNMP_PDU_Viewer::SNMP_PDU_Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SNMP_PDU_Viewer)
{
    ui->setupUi(this);

    ui->SPN_VarBinding->setEnabled(false);
    ui->PGE_VarBindings->setEnabled(false);
}

SNMP_PDU_Viewer::~SNMP_PDU_Viewer()
{
    delete ui;
}

void SNMP_PDU_Viewer::fSetPDU(const SNMP_PDU_Data &pdu)
{
    ui->LE_RequestID->setText(QString::number(pdu.fGetRequestID()));
    ui->LE_ErrorStatus->setText(QString::number(pdu.fGetErrorStatus()));
    ui->LE_ErrorIndex->setText(QString::number(pdu.fGetErrorIndex()));
    ui->LE_NumBindings->setText(QString::number(pdu.fGetNumVarBindings()));

    f_varbindings.clear();

    for(size_t ui=0;ui<pdu.fGetNumVarBindings();ui++)
    {
        f_varbindings.push_back(pdu.fGetVarBinding(ui));
    }

    if(f_varbindings.size() == 0)
    {
        ui->SPN_VarBinding->setEnabled(false);
        ui->PGE_VarBindings->setEnabled(false);
    }
    else
    {
        ui->SPN_VarBinding->setEnabled(true);
        ui->PGE_VarBindings->setEnabled(true);
        ui->SPN_VarBinding->setValue(0);
        ui->SPN_VarBinding->setMinimum(0);
        ui->SPN_VarBinding->setMaximum(f_varbindings.size());
        fUpdate();
    }
}

void SNMP_PDU_Viewer::fUpdate()
{
    int index = ui->SPN_VarBinding->value();

    if((index >= 0) && (index < f_varbindings.size()))
    {
        ui->PGE_VarBindings->fSetVarBinding(f_varbindings[index]);
    }
}

void SNMP_PDU_Viewer::on_SPN_VarBinding_valueChanged(int /*arg1*/)
{
    fUpdate();
}
