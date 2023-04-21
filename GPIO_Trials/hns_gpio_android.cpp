#include "hns_gpio_android.h"

#include <QDebug>
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniEnvironment>

HNS_GPIO_Service_Interface *g_gpio_service_interface2 = nullptr;

using namespace std;

static void inputChanged2(JNIEnv *, jobject, jint port, jint val)
{
    qDebug() << "New input received at port" << (int)port << "and value" << (int)val;
    if(g_gpio_service_interface2 != nullptr)
    {
        g_gpio_service_interface2->fRetrieveNewInput((int)port,(int)val);
    }
}

HNS_GPIO_Android::HNS_GPIO_Android(QObject *parent) :
    QObject(parent)
{
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_INPUT,11,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_INPUT,12,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_INPUT,13,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_INPUT,14,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_OUTPUT,43,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_OUTPUT,44,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_OUTPUT,45,0));
    f_ports.push_back(HNS_GPIO_Port(HNS_GPIO_OUTPUT,46,0));
    g_gpio_service_interface2 = (HNS_GPIO_Service_Interface *)this;

    JNINativeMethod methods[] {{"inputChanged", "(II)V", reinterpret_cast<void *>(inputChanged2)}};
    QAndroidJniObject javaClass("com/hillandsmith/GPIO_Trials/MyTest");

    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
    env->RegisterNatives(objectClass,
                         methods,
                         sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

    fStartService();
}

void HNS_GPIO_Android::fRetrieveNewInput(const int &port, const int &val)
{
    int index = -1;
    for(int i=0;i<f_ports.size();i++)
    {
        if(port == f_ports[i].fGetPortNum())
        {
            index = i;
            break;
        }
    }
    if(index >= 0)
    {
        f_ports[index].fSetVal(val);
        qDebug() << "Emitting signal for a change of signal at port" << port << "and val" << val;
        emit SIG_Input_Changed();
    }
}

HNS_GPIO_Port HNS_GPIO_Android::fGetGPIO_Port(const int &port)
{
    HNS_GPIO_Port result;
    int index = -1;
    for(int i=0;i<f_ports.size();i++)
    {
        if(port == f_ports[i].fGetPortNum())
        {
            index = i;
            break;
        }
    }

    if(index >=0)
    {
        if(f_ports[index].fGetDirection() == HNS_GPIO_INPUT)
        {
            fGetGPIO_Input(port);
        }
        result = f_ports[index];
    }
    return result;
}

vector<HNS_GPIO_Port> HNS_GPIO_Android::fGetGPIO_Ports()
{
    vector<HNS_GPIO_Port> result;

    for(int i=0;i<f_ports.size();i++)
    {
        fGetGPIO_Port(f_ports[i].fGetPortNum());
    }
    result = f_ports.toStdVector();

    return result;
}

int HNS_GPIO_Android::fGetGPIO_Input(const int &port)
{
    int val = -1;
    int index = -1;

    for(int i=0;i<f_ports.size();i++)
    {
        if(port == f_ports[i].fGetPortNum())
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        if(f_ports[index].fGetDirection() == HNS_GPIO_INPUT)
        {
            QAndroidJniEnvironment env;
            jclass cls2 = env->FindClass("com/hillandsmith/GPIO_Trials/MyTest");

            if(cls2 == nullptr)
            {
                qDebug() << "MyTest class not found";
            }
            else
            {
                qDebug() << "MyTest class found";
                jmethodID mid = env->GetStaticMethodID(cls2,"getInput", "(I)I");
                if(mid == nullptr)
                {
                    qDebug() << "method getIput not found";
                }
                else
                {
                    qDebug() << "Running getIput";
                    val = (int)env->CallStaticIntMethod(cls2,mid,(jint)port);
                    f_ports[index].fSetVal(val);
                }
            }

            env->DeleteLocalRef(cls2);
        }
    }

    return val;
}

void HNS_GPIO_Android::fSetGPIO_Output(const int &port, const int &val)
{
    int index = -1;

    for(int i=0;i<f_ports.size();i++)
    {
        if(port == f_ports[i].fGetPortNum())
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        qDebug() << "Port" << port << "is valid";
        if(f_ports[index].fGetDirection() == HNS_GPIO_OUTPUT)
        {
            QAndroidJniEnvironment env;
            jclass cls2 = env->FindClass("com/hillandsmith/GPIO_Trials/MyTest");

            if(cls2 == nullptr)
            {
                qDebug() << "MyTest class not found";
            }
            else
            {
                qDebug() << "MyTest class found";
                jmethodID mid = env->GetStaticMethodID(cls2,"setOutput", "(II)V");
                if(mid == nullptr)
                {
                    qDebug() << "method setOutput not found";
                }
                else
                {
                    qDebug() << "Running setOutput";
                    env->CallStaticVoidMethod(cls2,mid,(jint)port,(jint)val);
                    f_ports[index].fSetVal(val);
                }
            }
            env->DeleteLocalRef(cls2);
        }
    }
}

void HNS_GPIO_Android::fStartService()
{
    qDebug() << "Trying to start service";

    QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),
                                        "com/hillandsmith/GPIO_Trials/MyTest");

    QAndroidJniObject result = QtAndroid::androidActivity().callObjectMethod(
                "startService",
                "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                serviceIntent.handle().object());
}
