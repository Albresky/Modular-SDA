#include "projectpage.h"



ProjectPage::ProjectPage(QWidget* parent, MainWindow* mainWindow)
    : QWidget{parent},
      mainWindow(mainWindow)
{
    initLayout();
}

void ProjectPage::initComboBox()
{
    QList<QString> baudRateList = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
    mapBaudRate.insert("1200", QSerialPort::Baud1200);
    mapBaudRate.insert("2400", QSerialPort::Baud2400);
    mapBaudRate.insert("4800", QSerialPort::Baud4800);
    mapBaudRate.insert("9600", QSerialPort::Baud9600);
    mapBaudRate.insert("19200", QSerialPort::Baud19200);
    mapBaudRate.insert("38400", QSerialPort::Baud38400);
    mapBaudRate.insert("57600", QSerialPort::Baud57600);
    mapBaudRate.insert("115200", QSerialPort::Baud115200);

    QList<QString> parityList = {"None", "Even", "Odd", "Space", "Mark"};
    mapParity.insert("None", QSerialPort::NoParity);
    mapParity.insert("Even", QSerialPort::EvenParity);
    mapParity.insert("Odd", QSerialPort::OddParity);
    mapParity.insert("Space", QSerialPort::SpaceParity);
    mapParity.insert("Mark", QSerialPort::MarkParity);
    QList<QString> dataBitsList = {"5", "6", "7", "8"};
    mapDataBits.insert("5", QSerialPort::Data5);
    mapDataBits.insert("6", QSerialPort::Data6);
    mapDataBits.insert("7", QSerialPort::Data7);
    mapDataBits.insert("8", QSerialPort::Data8);
    QList<QString> stopBitsList = {"1", "1.5", "2"};
    mapStopBits.insert("1", QSerialPort::OneStop);
    mapStopBits.insert("1.5", QSerialPort::OneAndHalfStop);
    mapStopBits.insert("2", QSerialPort::TwoStop);

    QList<QString> flowControlList = {"None", "Hardware", "Software"};
    mapFlowControl.insert("None", QSerialPort::NoFlowControl);
    mapFlowControl.insert("Hardware", QSerialPort::HardwareControl);
    mapFlowControl.insert("Software", QSerialPort::SoftwareControl);

    cbox_BaudRate->addItems(baudRateList);
    cbox_BaudRate->setCurrentIndex(7);
    cbox_Parity->addItems(parityList);
    cbox_DataBits->addItems(dataBitsList);
    cbox_DataBits->setCurrentIndex(3);
    cbox_StopBits->addItems(stopBitsList);
    cbox_FlowControl->addItems(flowControlList);
}

void ProjectPage::initLayout()
{
    cbox_COM = new QComboBox;
    cbox_BaudRate = new QComboBox;
    cbox_Parity = new QComboBox;
    cbox_DataBits = new QComboBox;
    cbox_StopBits = new QComboBox;
    cbox_FlowControl = new QComboBox;
    btn_refresh = new QPushButton;
    btn_refresh->setIcon(QIcon(":/res/imgs/refresh.png"));
    btn_refresh->setStyleSheet("QPushButton{background: transparent;}");
    btn_refresh->setStyleSheet("QPushButton#pushButton:hover {background-color: gray;}");
    btn_refresh->setIconSize(QSize(24, 24));
    btn_refresh->setFixedSize(QSize(24, 24));
    QObject::connect(btn_refresh, &QPushButton::clicked, mainWindow, &MainWindow::getCOMs);

    initComboBox();

    QHBoxLayout* COM_layout = new QHBoxLayout;

    COM_layout->addWidget(new QLabel("COM"));
    COM_layout->addWidget(btn_refresh);
    COM_layout->addWidget(cbox_COM);


    QHBoxLayout* baudRate_layout = new QHBoxLayout;
    baudRate_layout->addWidget(new QLabel("波特率"));
    baudRate_layout->addWidget(cbox_BaudRate);

    QHBoxLayout* parity_layout = new QHBoxLayout;
    parity_layout->addWidget(new QLabel("校验位"));
    parity_layout->addWidget(cbox_Parity);

    QHBoxLayout* dataBits_layout = new QHBoxLayout;
    dataBits_layout->addWidget(new QLabel("数据位"));
    dataBits_layout->addWidget(cbox_DataBits);

    QHBoxLayout* stopBits_layout = new QHBoxLayout;
    stopBits_layout->addWidget(new QLabel("停止位"));
    stopBits_layout->addWidget(cbox_StopBits);

    QHBoxLayout* flowControl_layout = new QHBoxLayout;
    flowControl_layout->addWidget(new QLabel("流控制"));
    flowControl_layout->addWidget(cbox_FlowControl);

    QVBoxLayout* serialPortConfig_layout = new QVBoxLayout;
    serialPortConfig_layout->addLayout(COM_layout);
    serialPortConfig_layout->addLayout(baudRate_layout);
    serialPortConfig_layout->addLayout(parity_layout);
    serialPortConfig_layout->addLayout(dataBits_layout);
    serialPortConfig_layout->addLayout(stopBits_layout);
    serialPortConfig_layout->addLayout(flowControl_layout);

    QWidget* placeHolder = new QWidget();
    QWidget* serialPortConfig = new QWidget();
    serialPortConfig->setLayout(serialPortConfig_layout);
    serialPortConfig->setFixedWidth(200);
    QHBoxLayout* main_layout = new QHBoxLayout;
    main_layout->addWidget(placeHolder);
    main_layout->addWidget(serialPortConfig);

    this->setLayout(main_layout);
}

void ProjectPage::setCOMs(QList<QSerialPortInfo*> list)
{
    cbox_COM->clear();
    foreach(QSerialPortInfo* info, list)
    {
        cbox_COM->addItem(info->portName());
    }
}

QString ProjectPage::getCOM()
{
    return cbox_COM->currentText();
}

QSerialPort::BaudRate ProjectPage::getBaudRate()
{
    return mapBaudRate.value(cbox_BaudRate->currentText());
}

QSerialPort::Parity ProjectPage::getParity()
{
    return mapParity.value(cbox_Parity->currentText());
}

QSerialPort::DataBits ProjectPage::getDataBits()
{
    return mapDataBits.value(cbox_DataBits->currentText());
}

QSerialPort::StopBits ProjectPage::getStopBits()
{
    return mapStopBits.value(cbox_StopBits->currentText());
}

QSerialPort::FlowControl ProjectPage::getFlowControl()
{
    return mapFlowControl.value(cbox_FlowControl->currentText());
}
