#include "qtelnet.h"

#include <QtDebug>

QTelnet::QTelnet(QObject* parent) : QObject(parent), socket(0), notifier(0), connected(false)
{
    setSocket(new QTcpSocket(parent));
}

QTelnet::~QTelnet()
{
    delete socket;
    delete notifier;
}

void QTelnet::setSocket(QTcpSocket* s)
{
    delete socket;

    connected = false;
    socket = s;
    if (socket)
    {
        QObject::connect(socket, &QTcpSocket::connected, this, &QTelnet::socketConnected);
        QObject::connect(socket, &QTcpSocket::disconnected, this, &QTelnet::socketConnectionClosed);
        QObject::connect(socket, &QTcpSocket::readyRead, this, &QTelnet::socketReadyRead);
        QObject::connect(socket, &QTcpSocket::errorOccurred, this, &QTelnet::socketError);
    }
}

void QTelnet::connectToHost(const QString& host, quint16 port)
{
    if (connected)
    {
        return;
    }
    socket->connectToHost(host, port);
}

void QTelnet::disconnectTelnet()
{
    socket->disconnectFromHost();
}

//bool QTelnet::isConnected() const
//{
//    return connected;
//}

void QTelnet::consume()
{
    const QByteArray data = buffer;
    buffer.clear();
    int currpos = 0;
    int prevpos = -1;;
    while (prevpos < currpos && currpos < data.size())
    {
        prevpos = currpos;
        const uchar c = uchar(data[currpos]);
        if (c == Common::DM)
        {
            ++currpos;
        }
        else if (c == Common::IAC)
        {
            currpos += 3;    //parseIAC(data.mid(currpos));
        }
        else // Assume plain text
        {
            currpos += parsePlaintext(data.mid(currpos));
        }
    }
    if (currpos < data.size())
    {
        buffer.push_back(data.mid(currpos));
    }
}

void QTelnet::socketConnected()
{
    delete notifier;
    qDebug() << "socket connected";
    connected = true;
    notifier = new QSocketNotifier(socket->socketDescriptor(), QSocketNotifier::Exception, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(socketException(int)));
    emit sockConnected();
}

void QTelnet::socketConnectionClosed()
{
    delete notifier;
    notifier = 0;
    connected = false;
    emit sockDisconnected();
}

void QTelnet::socketReadyRead()
{
    buffer.append(socket->readAll());
    consume();
}

void QTelnet::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit connectionError(error);
}

void QTelnet::socketException(int)
{
    qDebug("socketException :: out-of-band data received, should handle that here!");
}

int QTelnet::parsePlaintext(const QByteArray& data)
{
    int consumed = 0;
    int length = data.indexOf('\0');
    if (length == -1)
    {
        length = data.size();
        consumed = length;
    }
    else
    {
        consumed = length + 1; // + 1 for removing '\0'
    }

    const QString text = QString::fromLocal8Bit(data.constData(), length);

    if (!text.isEmpty())
    {
        emit message(text);
    }

    return consumed;
}

void QTelnet::sendData(const QString& data)
{
    if (!connected)
    {
        return;
    }

    QByteArray str = data.toLocal8Bit();

    if (str.size() > 0)
    {
        socket->write(str);
        socket->waitForBytesWritten();
        qDebug() << QString(socket->readAll());
    }
}

void QTelnet::sendData2(const QByteArray& data)
{
    if (!connected)
    {
        return;
    }

    if (data.size() > 0)
    {
        socket->write(data);
    }
}
