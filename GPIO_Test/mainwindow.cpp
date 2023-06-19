#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAndroidIntent>
#include <QDebug>

const int REQUEST_CODE = 123;
const jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");

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


void MainWindow::on_CBX_GPIO_43_stateChanged(int arg1)
{
    fSendCommand(43,arg1 == Qt::Checked);
}


void MainWindow::on_CBX_GPIO_44_stateChanged(int arg1)
{
    fSendCommand(44,arg1 == Qt::Checked);
}


void MainWindow::on_CBX_GPIO_45_stateChanged(int arg1)
{
    fSendCommand(45,arg1 == Qt::Checked);
}


void MainWindow::on_CBX_GPIO_46_stateChanged(int arg1)
{
    fSendCommand(46,arg1 == Qt::Checked);
}

void MainWindow::fSendCommand(const int &port, const bool &value)
{
    QString command = QString("HNS_GPIO SET %1").arg(port);
    if(value)
    {
        command += " ON";
    }
    else
    {
        command += " OFF";
    }

    QAndroidJniObject action_string = QAndroidJniObject::fromString("com.hillandsmith.GPIO_ACTION");
    QAndroidJniObject command_string = QAndroidJniObject::fromString(command);
    QAndroidJniObject type_string = QAndroidJniObject::fromString("text/plain");

    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action_string.object());

    QAndroidJniObject EXTRA_TEXT = QAndroidJniObject::getStaticObjectField("android/content/Intent","EXTRA_TEXT","Ljava/lang/String;");

    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            EXTRA_TEXT.object(),
                            command_string.object());
    intent.callObjectMethod("setType",
                            "(Ljava/lang/String;)Landroid/content/Intent;",
                            type_string.object());

    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
                                                                 "()Landroid/content/pm/PackageManager;");

    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
                                                              packageManager.object());

    if(componentName.isValid())
    {
        qDebug() << "Sending command";
        QtAndroid::startActivity(intent,REQUEST_CODE,[this](int requestCode, int resultCode, const QAndroidJniObject &data) {
                                                    activityReceiver(requestCode, resultCode, data);
                                                 });
    }
}

void MainWindow::activityReceiver(int requestCode, int resultCode, const QAndroidJniObject &data)
{
    QString tempstring;
    qDebug() << "activity result received";
    if (requestCode == REQUEST_CODE) {
        if (resultCode == RESULT_OK) {
            const QAndroidJniObject key = QAndroidJniObject::fromString("message");
            const QAndroidJniObject message = data.callObjectMethod(
                    "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;", key.object());
            if (message.isValid())
            {
                tempstring = message.toString();
                qDebug() << tempstring;
            }
        } else {
            qDebug() << "rejected!";
        }
    }
}
