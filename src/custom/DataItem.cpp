#include "custom/DataItem.h"


AV_DataItem::AV_DataItem() {}
AV_DataItem::~AV_DataItem() {}


AV_Input::AV_Input(int sampleFreq, int dataLen)
{
    SampleFreq = new QSpinBox;
    DataLen = new QSpinBox;
    SampleFreq->setValue(sampleFreq);
    DataLen->setValue(dataLen);
    SampleFreq->setRange(0, 10e6);
    DataLen->setRange(0, 1024);
    widget = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样频率"), 0, 0, Qt::AlignTop);
    layout->addWidget(SampleFreq, 0, 1, Qt::AlignTop);
    layout->addWidget(new QLabel("数据长度"), 1, 0, Qt::AlignTop);
    layout->addWidget(DataLen, 1, 1, Qt::AlignTop);
    widget->setLayout(layout);
}

AV_Input::~AV_Input()
{
    delete SampleFreq;
    delete DataLen;
    delete widget;
}


int AV_Input::getSampleFreq()
{
    return SampleFreq->value();
}

int AV_Input::getDataLen()
{
    return DataLen->value();
}


AV_FFT_IFFT::AV_FFT_IFFT(int pointCnt, int sampleFreq)
{
    PointCnt = new QSpinBox;
    SampleFreq = new QSpinBox;
    PointCnt->setValue(pointCnt);
    SampleFreq->setValue(sampleFreq);
    PointCnt->setRange(0, 1024);
    SampleFreq->setRange(0, 10e6);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数"), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    layout->addWidget(new QLabel("采样频率"), 1, 0);
    layout->addWidget(SampleFreq, 1, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_FFT_IFFT:: ~AV_FFT_IFFT()
{
    delete PointCnt;
    delete SampleFreq;
    delete widget;
}

int AV_FFT_IFFT::getPointCnt()
{
    return PointCnt->value();
}
int AV_FFT_IFFT::getSampleFreq()
{
    return SampleFreq->value();
}

AV_DFT::AV_DFT(int pointCnt, int sampleFreq, int calcuFreq)
{
    PointCnt = new QSpinBox;
    SampleFreq = new QSpinBox;
    CalcuFreq = new QSpinBox;
    PointCnt->setValue(pointCnt);
    SampleFreq->setValue(sampleFreq);
    CalcuFreq->setValue(calcuFreq);
    PointCnt->setRange(0, 1024);
    SampleFreq->setRange(0, 10e6);
    CalcuFreq->setRange(0, 10e6);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数"), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    layout->addWidget(new QLabel("采样频率"), 1, 0);
    layout->addWidget(SampleFreq, 1, 1);
    layout->addWidget(new QLabel("计算频率"), 2, 0);
    layout->addWidget(CalcuFreq, 2, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}

AV_DFT:: ~AV_DFT()
{
    delete PointCnt;
    delete SampleFreq;
    delete widget;
}

int AV_DFT::getPointCnt()
{
    return PointCnt->value();
}
int AV_DFT::getSampleFreq()
{
    return SampleFreq->value();
}
int AV_DFT::getCalcuFreq()
{
    return CalcuFreq->value();
}


AV_HanningWin_BlackmanWin::AV_HanningWin_BlackmanWin(int pointCnt)
{
    PointCnt = new QSpinBox;
    PointCnt->setValue(pointCnt);
    PointCnt->setRange(0, 1024);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数"), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_HanningWin_BlackmanWin::~AV_HanningWin_BlackmanWin()
{
    delete PointCnt;
    delete widget;
}
int AV_HanningWin_BlackmanWin::getPointCnt()
{
    return PointCnt->value();
}



AV_HT::AV_HT(int pointCnt, int sampleFreq)
{
    PointCnt = new QSpinBox;
    SampleFreq = new QSpinBox;
    PointCnt->setValue(pointCnt);
    SampleFreq->setValue(sampleFreq);
    PointCnt->setRange(0, 1024);
    SampleFreq->setRange(0, 10e6);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数"), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    layout->addWidget(new QLabel("采样频率"), 1, 0);
    layout->addWidget(SampleFreq, 1, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_HT::~AV_HT()
{
    delete PointCnt;
    delete SampleFreq;
    delete widget;
}
int AV_HT::getPointCnt()
{
    return PointCnt->value();
}
int AV_HT::getSampleFreq()
{
    return SampleFreq->value();
}


AV_Filter::AV_Filter(int ufl, int dfl, double passbandgain, double stopbandgain)
{
    UFL = new QSpinBox;
    DFL = new QSpinBox;
    PassBandGain = new QDoubleSpinBox;
    StopBandGain = new QDoubleSpinBox;
    UFL->setValue(ufl);
    DFL->setValue(dfl);
    PassBandGain->setValue(passbandgain);
    StopBandGain->setValue(stopbandgain);
    UFL->setRange(0, 10e6);
    DFL->setRange(0, 10e6);
    PassBandGain->setRange(1, 10);
    StopBandGain->setRange(0, 1);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("上限频率"), 0, 0);
    layout->addWidget(UFL, 0, 1);
    layout->addWidget(new QLabel("下限频率"), 1, 0);
    layout->addWidget(DFL, 1, 1);
    layout->addWidget(new QLabel("通带增益"), 2, 0);
    layout->addWidget(PassBandGain, 2, 1);
    layout->addWidget(new QLabel("阻带增益"), 3, 0);
    layout->addWidget(StopBandGain, 3, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_Filter::~AV_Filter()
{
    delete UFL;
    delete DFL;
    delete PassBandGain;
    delete StopBandGain;
    delete widget;
}
int AV_Filter::getUFL()
{
    return UFL->value();
}
int AV_Filter::getDFL()
{
    return DFL->value();
}
double AV_Filter::getPassBandGain()
{
    return PassBandGain->value();
}
double AV_Filter::getStopBandGain()
{
    return StopBandGain->value();
}
