#include "snmp_varbinding_viewer.h"
#include "ui_snmp_varbinding_viewer.h"

#include <QTextStream>
#include <vector>

using namespace std;

SNMP_VarBinding_Viewer::SNMP_VarBinding_Viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SNMP_VarBinding_Viewer)
{
    ui->setupUi(this);
}

SNMP_VarBinding_Viewer::~SNMP_VarBinding_Viewer()
{
    delete ui;
}

void SNMP_VarBinding_Viewer::fSetVarBinding(const SNMP_Variable_Bindings &varbinding)
{
    QString tempstring;
    QTextStream out_stream(&tempstring);
    SNMP_Data temp_data = varbinding.fGetData();
    vector<unsigned char> temp_raw_data = temp_data.fGetRawData();

    ui->LE_OID->setText(QString::fromStdString(varbinding.fGetOID()));
    ui->LE_Type->setText(QString::number(temp_data.fGetDataType()));
    ui->LE_Length->setText(QString::number(temp_data.fGetDataSize()));

    for(size_t ui=0;ui<temp_data.fGetDataSize();ui++)
    {
        out_stream.setIntegerBase(16);
        out_stream.setFieldWidth(2);
        out_stream.setPadChar(QLatin1Char('0'));
        out_stream << static_cast<signed>(temp_data.fGetData().at(ui));
    }

    ui->LE_Data->setText(tempstring);
}
