#ifndef LOADNTCIP_WORKER_H
#define LOADNTCIP_WORKER_H

#include <QObject>
#include <NTCIP.h>

class LoadNTCIP_Worker : public QObject
{
    Q_OBJECT
public:
    LoadNTCIP_Worker();

    NTCIP_Node fGetResult();

public slots:
    void doWork(const QString &file_path, const QString &backup_resource);

signals:
    void resultReady();

private:
    NTCIP_Node f_ntcip;
};

#endif // LOADNTCIP_WORKER_H
