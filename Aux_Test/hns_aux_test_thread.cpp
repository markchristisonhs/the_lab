#include "hns_aux_test_thread.h"

#include <QDebug>

HNS_Aux_Test_Thread::HNS_Aux_Test_Thread(QObject *parent):
    QThread(parent)
  , f_quit(false)
{

}

void HNS_Aux_Test_Thread::run()
{
    bool done = false;
    while(!done)
    {
        sleep(1);
        qDebug() << "thread is running";

        if(isInterruptionRequested())
        {
            f_mutex.lock();
            qDebug() << "interruption was requested";
            if(f_quit)
            {
                qDebug() << "quit received";
                done = true;
            }
            f_mutex.unlock();
        }
    }
}

void HNS_Aux_Test_Thread::fQuit()
{
    f_quit = true;
    requestInterruption();
    wait();
    quit();
}
