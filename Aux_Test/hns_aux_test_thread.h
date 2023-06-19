#ifndef HNS_AUX_TEST_THREAD_H
#define HNS_AUX_TEST_THREAD_H

#include <QThread>
#include <QMutex>

class HNS_Aux_Test_Thread : public QThread
{
    Q_OBJECT
public:
    HNS_Aux_Test_Thread(QObject *parent = nullptr);
    void run() override;

    void fQuit();

private:
    bool f_quit;
    QMutex f_mutex;
};

#endif // HNS_AUX_TEST_THREAD_H
