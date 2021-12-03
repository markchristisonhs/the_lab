#ifndef SNMP_VARBINDING_VIEWER_H
#define SNMP_VARBINDING_VIEWER_H

#include <QWidget>
#include <SNMP.h>

namespace Ui {
class SNMP_VarBinding_Viewer;
}

class SNMP_VarBinding_Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit SNMP_VarBinding_Viewer(QWidget *parent = nullptr);
    ~SNMP_VarBinding_Viewer();

    void fSetVarBinding(const SNMP_Variable_Bindings &varbinding);

private:
    Ui::SNMP_VarBinding_Viewer *ui;
};

#endif // SNMP_VARBINDING_VIEWER_H
