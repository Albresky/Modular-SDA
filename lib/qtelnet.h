#ifndef QTELNET_H
#define QTELNET_H

#include <QObject>
#include <QTcpSocket>
#include <QSocketNotifier>
#include <QByteArray>
#include <QString>

class QTelnet : public QObject
{
    Q_OBJECT

public:
    QTelnet(QObject* parent = 0);
    ~QTelnet();

    void setSocket(QTcpSocket* socket);
    void connectToHost(const QString& host, quint16 port);
//    bool isConnected() const;
    void disconnectTelnet();

public slots:
    void socketConnected();
    void socketConnectionClosed();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);
    void socketException(int);
    void sendData(const QString& data);
    void sendData2(const QByteArray& data);


signals:
    void message(const QString& data);
    void sockConnected();
    void sockDisconnected();
    void connectionError(QAbstractSocket::SocketError error);

private:
    void consume();
    int parsePlaintext(const QByteArray& data);

    QTcpSocket* socket;
    QSocketNotifier* notifier;
    QByteArray buffer;
    bool connected;

};

namespace Common // RFC854
{
    // Commands
    const uchar CEOF  = 236;
    const uchar SUSP  = 237;
    const uchar ABORT = 238;
    const uchar SE    = 240;
    const uchar NOP   = 241;
    const uchar DM    = 242;
    const uchar BRK   = 243;
    const uchar IP    = 244;
    const uchar AO    = 245;
    const uchar AYT   = 246;
    const uchar EC    = 247;
    const uchar EL    = 248;
    const uchar GA    = 249;
    const uchar SB    = 250;
    const uchar WILL  = 251;
    const uchar WONT  = 252;
    const uchar DO    = 253;
    const uchar DONT  = 254;
    const uchar IAC   = 255;

    // Types
    const char IS    = 0;
    const char SEND  = 1;

    const char Authentication = 37; // RFC1416,
    // implemented to always return NULL
    const char SuppressGoAhead = 3; // RFC858
    const char Echo = 1; // RFC857, not implemented (returns WONT/DONT)
    const char LineMode = 34; // RFC1184, implemented
    const uchar LineModeEOF = 236, // RFC1184, not implemented
                LineModeSUSP = 237,
                LineModeABORT = 238;
    const char Status = 5; // RFC859, should be implemented!
    const char Logout = 18; // RFC727, implemented
    const char TerminalType = 24; // RFC1091,
    // implemented to always return UNKNOWN
    const char NAWS = 31; // RFC1073, implemented
    const char TerminalSpeed = 32; // RFC1079, not implemented
    const char FlowControl = 33; // RFC1372, should be implemented?
    const char XDisplayLocation = 35; // RFC1096, not implemented
    const char EnvironmentOld = 36; // RFC1408, should not be implemented!
    const char Environment = 39; // RFC1572, should be implemented
    const char Encrypt = 38; // RFC2946, not implemented
};

namespace LineMode // RFC1184
{
    const char Mode = 1;
    const char ForwardMask = 2;
    const char SLC = 3;
    enum Modes
    {
        EDIT = 1,
        TRAPSIG = 2,
        MODE_ACK = 4,
        SOFT_TAB = 8,
        LIT_ECHO = 16
    };
    enum SLCs
    {
        SLC_SYNCH = 1,
        SLC_BRK = 2,
        SLC_IP = 3,
        SLC_AO =  4,
        SLC_AYT = 5,
        SLC_EOR = 6,
        SLC_ABORT = 7,
        SLC_EOF = 8,
        SLC_SUSP = 9,
        SLC_EC = 10,
        SLC_EL = 11,
        SLC_EW = 12,
        SLC_RP = 13,
        SLC_LNEXT = 14,
        SLC_XON = 15,
        SLC_XOFF = 16,
        SLC_FORW1 = 17,
        SLC_FORW2 = 18,
        SLC_MCL = 19,
        SLC_MCR = 20,
        SLC_MCWL = 21,
        SLC_MCWR = 22,
        SLC_MCBOL = 23,
        SLC_MCEOL = 24,
        SLC_INSRT = 25,
        SLC_OVER = 26,
        SLC_ECR = 27,
        SLC_EWR = 28,
        SLC_EBOL = 29,
        SLC_EEOL = 30,
        SLC_DEFAULT = 3,
        SLC_VALUE = 2,
        SLC_CANTCHANGE = 1,
        SLC_NOSUPPORT = 0,
        SLC_LEVELBITS = 3,
        SLC_ACK = 128,
        SLC_FLUSHIN = 64,
        SLC_FLUSHOUT = 32
    };
};

#endif

