#include "mythread.h"

MyThread::MyThread( QObject* parent)
    : QThread{parent}
{
}


void MyThread::setMainW(MainWindow* mainW)
{
    this->mainW = mainW;
    QObject::connect(this, &MyThread::done, mainW, &MainWindow::connectEstablished);
}

void MyThread::setTelnet(QTelnet* telnet)
{
    this->telnet = telnet;
}

void MyThread::run()
{
    if(telnet != nullptr)
    {
        int cnt = 10;
        telnet->connectToHost("127.0.0.1", 4444);
        while(true)
        {
            cnt--;
            sleep(1);
            if(cnt > 0 && telnet->isConnected())
            {
                qDebug() << "thread::telnet connected @127.0.0.1:4444";
                emit done();
                break;
            }
        }
    }
    quit();
}
