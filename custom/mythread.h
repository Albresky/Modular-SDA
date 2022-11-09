#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject* parent = nullptr);
    enum ThreadType {BUILD};

    void setType(MyThread::ThreadType);

protected:
    void run() override;

private:
    ThreadType threadType;
    volatile bool m_stop;
};

#endif // MYTHREAD_H
