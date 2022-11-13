#include "mythread.h"

MyThread::MyThread(QObject* parent)
    : QThread{parent}
{
    m_stop = false;
}

void MyThread::run()
{
    switch(threadType)
    {
        case ThreadType::BUILD:
            // ToDo
            break;
    }
}

void MyThread::setType(MyThread::ThreadType type)
{
    threadType = type;
}
