#ifndef PROJECTPAGE_H
#define PROJECTPAGE_H

#include <QtWidgets/QWidget>

#include <QComboBox>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSpinBox>

#include "custom/commonHeaders.h"
#include <mainwindow.h>

class ProjectPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectPage(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);

    void setCOMs(QList<QSerialPortInfo*>);
    QString getCOM();
    QSerialPort::BaudRate getBaudRate();
    QSerialPort::Parity getParity();
    QSerialPort::DataBits getDataBits();
    QSerialPort::StopBits getStopBits();
    QSerialPort::FlowControl getFlowControl();

signals:


private:
    MainWindow* mainWindow;
    QComboBox* cbox_COM;
    QComboBox* cbox_BaudRate;
    QComboBox* cbox_Parity;
    QComboBox* cbox_DataBits;
    QComboBox* cbox_StopBits;
    QComboBox* cbox_FlowControl;
    QPushButton* btn_refresh;
    QMap<QString, QSerialPort::BaudRate> mapBaudRate;
    QMap<QString, QSerialPort::Parity> mapParity;
    QMap<QString, QSerialPort::DataBits> mapDataBits;
    QMap<QString, QSerialPort::StopBits> mapStopBits;
    QMap<QString, QSerialPort::FlowControl> mapFlowControl;

    void initComboBox();
    void initLayout();
};

#endif // PROJECTPAGE_H
