#ifndef SNMP_VIEWER_H
#define SNMP_VIEWER_H

#include <QWidget>
#include <SNMP.h>
#include <QVector>

namespace Ui {
class SNMP_Viewer;
}

class SNMP_Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit SNMP_Viewer(QWidget *parent = nullptr);
    ~SNMP_Viewer();

    void fSetSNMP_Packet(const SNMP_Packet &packet);

private slots:
    void on_SPN_PDU_valueChanged(int arg1);

private:
    Ui::SNMP_Viewer *ui;

    QVector<SNMP_PDU_Data> f_pdus;

    void fUpdate();
};

#endif // SNMP_VIEWER_H
