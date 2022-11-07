#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <mainwindow.h>

#include <lib/qtelnet.h>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject* parent = nullptr);
    void setMainW(MainWindow* mainW);
    void setTelnet(QTelnet* telnet);
protected:
    void run();
signals:
    void done();

private:
    QTelnet* telnet;
    MainWindow* mainW;
};

#endif // MYTHREAD_H
