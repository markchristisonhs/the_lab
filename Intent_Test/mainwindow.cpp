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


//void MainWindow::on_PB_Send_clicked()
//{
//    ui->PTE_Debug->appendPlainText("Sending intent");
//    ui->PTE_Debug->appendPlainText("just checking");

//    QString tempstring = QString("TEST");

//    QAndroidJniObject action_string = QAndroidJniObject::fromString("com.hillandsmith.example.TEST_INTENT");
//    QAndroidJniObject ticks_string = QAndroidJniObject::fromString(tempstring);
//    QAndroidJniObject type_string = QAndroidJniObject::fromString("text/plain");

//    QAndroidJniObject intent("android/content/Intent",
//                             "(Ljava/lang/String;)V",
//                             action_string.object());

//    QAndroidJniObject EXTRA_TEXT = QAndroidJniObject::getStaticObjectField("android/content/Intent","EXTRA_TEXT","Ljava/lang/String;");

//    intent.callObjectMethod("putExtra",
//                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
//                            EXTRA_TEXT.object(),
//                            ticks_string.object());
//    intent.callObjectMethod("setType",
//                            "(Ljava/lang/String;)Landroid/content/Intent;",
//                            type_string.object());

//    QAndroidJniObject activity = QtAndroid::androidActivity();
//    if(!activity.isValid())
//    {
//        ui->PTE_Debug->appendPlainText("Activity is not valid");
//    }
//    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
//                                                                 "()Landroid/content/pm/PackageManager;");
//    if(!packageManager.isValid())
//    {
//        ui->PTE_Debug->appendPlainText("Package Manager is not valid");
//    }

//    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
//                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
//                                                              packageManager.object());
//    if(!componentName.isValid())
//    {
//        ui->PTE_Debug->appendPlainText("Component Name is not valid");
//    }

//    if(componentName.isValid())
//    {
//        QtAndroid::startActivity(intent,REQUEST_CODE,[this](int requestCode, int resultCode, const QAndroidJniObject &data) {
//                                                    activityReceiver(requestCode, resultCode, data);
//                                                 });
//    }
//}


void MainWindow::on_PB_Send_clicked()
{
    ui->PTE_Debug->appendPlainText("Sending intent");
    ui->PTE_Debug->appendPlainText("just checking");

    QString tempstring = QString("HNS_GPIO SET 43 ON");

    QAndroidJniObject action_string = QAndroidJniObject::fromString("com.hillandsmith.GPIO_ACTION");
    QAndroidJniObject ticks_string = QAndroidJniObject::fromString(tempstring);
    QAndroidJniObject type_string = QAndroidJniObject::fromString("text/plain");

    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action_string.object());

    QAndroidJniObject EXTRA_TEXT = QAndroidJniObject::getStaticObjectField("android/content/Intent","EXTRA_TEXT","Ljava/lang/String;");

    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            EXTRA_TEXT.object(),
                            ticks_string.object());
    intent.callObjectMethod("setType",
                            "(Ljava/lang/String;)Landroid/content/Intent;",
                            type_string.object());

    QAndroidJniObject activity = QtAndroid::androidActivity();
    if(!activity.isValid())
    {
        ui->PTE_Debug->appendPlainText("Activity is not valid");
    }
    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
                                                                 "()Landroid/content/pm/PackageManager;");
    if(!packageManager.isValid())
    {
        ui->PTE_Debug->appendPlainText("Package Manager is not valid");
    }

    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
                                                              packageManager.object());
    if(!componentName.isValid())
    {
        ui->PTE_Debug->appendPlainText("Component Name is not valid");
    }

    if(componentName.isValid())
    {
        QtAndroid::startActivity(intent,REQUEST_CODE,[this](int requestCode, int resultCode, const QAndroidJniObject &data) {
                                                    activityReceiver(requestCode, resultCode, data);
                                                 });
    }
}


void MainWindow::on_PB_Send_2_clicked()
{
    ui->PTE_Debug->appendPlainText("Sending intent");
    ui->PTE_Debug->appendPlainText("just checking");

    QString tempstring = QString("HNS_GPIO SET 43 OFF");

    QAndroidJniObject action_string = QAndroidJniObject::fromString("com.hillandsmith.GPIO_ACTION");
    QAndroidJniObject ticks_string = QAndroidJniObject::fromString(tempstring);
    QAndroidJniObject type_string = QAndroidJniObject::fromString("text/plain");

    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action_string.object());

    QAndroidJniObject EXTRA_TEXT = QAndroidJniObject::getStaticObjectField("android/content/Intent","EXTRA_TEXT","Ljava/lang/String;");

    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            EXTRA_TEXT.object(),
                            ticks_string.object());
    intent.callObjectMethod("setType",
                            "(Ljava/lang/String;)Landroid/content/Intent;",
                            type_string.object());

    QAndroidJniObject activity = QtAndroid::androidActivity();
    if(!activity.isValid())
    {
        ui->PTE_Debug->appendPlainText("Activity is not valid");
    }
    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
                                                                 "()Landroid/content/pm/PackageManager;");
    if(!packageManager.isValid())
    {
        ui->PTE_Debug->appendPlainText("Package Manager is not valid");
    }

    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
                                                              packageManager.object());
    if(!componentName.isValid())
    {
        ui->PTE_Debug->appendPlainText("Component Name is not valid");
    }

    if(componentName.isValid())
    {
        QtAndroid::startActivity(intent,REQUEST_CODE,[this](int requestCode, int resultCode, const QAndroidJniObject &data) {
                                                    activityReceiver(requestCode, resultCode, data);
                                                 });
    }
}


void MainWindow::on_PB_Send_3_clicked()
{
    ui->PTE_Debug->appendPlainText("Sending intent");
    ui->PTE_Debug->appendPlainText("just checking");

    QString tempstring = QString("TEST");

    QAndroidJniObject action_string = QAndroidJniObject::fromString("com.hillandsmith.example.TEST_INTENT");
    QAndroidJniObject ticks_string = QAndroidJniObject::fromString(tempstring);
    QAndroidJniObject type_string = QAndroidJniObject::fromString("text/plain");

    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action_string.object());

    QAndroidJniObject EXTRA_TEXT = QAndroidJniObject::getStaticObjectField("android/content/Intent","EXTRA_TEXT","Ljava/lang/String;");

    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            EXTRA_TEXT.object(),
                            ticks_string.object());
    intent.callObjectMethod("setType",
                            "(Ljava/lang/String;)Landroid/content/Intent;",
                            type_string.object());

    QAndroidJniObject activity = QtAndroid::androidActivity();
    if(!activity.isValid())
    {
        ui->PTE_Debug->appendPlainText("Activity is not valid");
    }
    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
                                                                 "()Landroid/content/pm/PackageManager;");
    if(!packageManager.isValid())
    {
        ui->PTE_Debug->appendPlainText("Package Manager is not valid");
    }

    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
                                                              packageManager.object());
    if(!componentName.isValid())
    {
        ui->PTE_Debug->appendPlainText("Component Name is not valid");
    }

    if(componentName.isValid())
    {
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
                qDebug() << "Received Paylod";
                qDebug() << tempstring;
            }
        } else {
            qDebug() << "rejected!";
            ui->PTE_Debug->appendPlainText("rejected!");
        }
    }
}

