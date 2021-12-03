#ifndef SNMP_PDU_VIEWER_H
#define SNMP_PDU_VIEWER_H

#include <QWidget>
#include <SNMP.h>
#include <QVector>

namespace Ui {
class SNMP_PDU_Viewer;
}

class SNMP_PDU_Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit SNMP_PDU_Viewer(QWidget *parent = nullptr);
    ~SNMP_PDU_Viewer();

    void fSetPDU(const SNMP_PDU_Data &pdu);

private slots:
    void on_SPN_VarBinding_valueChanged(int arg1);

private:
    Ui::SNMP_PDU_Viewer *ui;

    QVector <SNMP_Variable_Bindings> f_varbindings;

    void fUpdate();
};

#endif // SNMP_PDU_VIEWER_H
