#include "custom/DataItem.h"


AV_DataItem::AV_DataItem() {}
AV_DataItem::~AV_DataItem() {}


AV_Input::AV_Input(int sampleFreq, int dataLen)
{
    SampleFreq = new QSpinBox;
    DataLen = new QSpinBox;
    SampleFreq->setValue(sampleFreq);
    DataLen->setValue(dataLen);
    widget = new QWidget;
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样频率"), 0, 0);
    layout->addWidget(SampleFreq, 0, 1);
    layout->addWidget(new QLabel("数据长度"), 1, 0);
    layout->addWidget(DataLen, 1, 1);
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


AV_FFT_IFFT_DFT::AV_FFT_IFFT_DFT(int pointCnt, int signalFreq, int sampleFreq)
{
    PointCnt = new QSpinBox;
    SignalFreq = new QSpinBox;
    SampleFreq = new QSpinBox;
    PointCnt->setValue(pointCnt);
    SignalFreq->setValue(signalFreq);
    SampleFreq->setValue(sampleFreq);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数："), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    layout->addWidget(new QLabel("信号频率："), 1, 0);
    layout->addWidget(SignalFreq, 1, 1);
    layout->addWidget(new QLabel("采样点数："), 2, 0);
    layout->addWidget(SampleFreq, 2, 1);
    widget = new QWidget;
    widget->setLayout(layout);

}
AV_FFT_IFFT_DFT:: ~AV_FFT_IFFT_DFT()
{
    delete PointCnt;
    delete SignalFreq;
    delete SampleFreq;
    delete widget;
}

int AV_FFT_IFFT_DFT::getPointCnt()
{
    return PointCnt->value();
}
int AV_FFT_IFFT_DFT::getSignalFreq()
{
    return SignalFreq->value();
}
int AV_FFT_IFFT_DFT::getSampleFreq()
{
    return SampleFreq->value();
}



AV_HanningWin_BlackmanWin::AV_HanningWin_BlackmanWin(int pointCnt)
{
    PointCnt = new QSpinBox;
    PointCnt->setValue(pointCnt);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数："), 0, 0);
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



AV_HT::AV_HT(int pointCnt, int signalFreq, int sampleFreq)
{
    PointCnt = new QSpinBox;
    SignalFreq = new QSpinBox;
    SampleFreq = new QSpinBox;
    PointCnt->setValue(pointCnt);
    SignalFreq->setValue(signalFreq);
    SampleFreq->setValue(sampleFreq);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("采样点数："), 0, 0);
    layout->addWidget(PointCnt, 0, 1);
    layout->addWidget(new QLabel("信号频率："), 1, 0);
    layout->addWidget(SignalFreq, 1, 1);
    layout->addWidget(new QLabel("采样频率："), 2, 0);
    layout->addWidget(SampleFreq, 2, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_HT::~AV_HT()
{
    delete PointCnt;
    delete SignalFreq;
    delete SampleFreq;
    delete widget;
}
int AV_HT::getPointCnt()
{
    return PointCnt->value();
}
int AV_HT::getSignalFreq()
{
    return SignalFreq->value();
}
int AV_HT::getSampleFreq()
{
    return SampleFreq->value();
}


AV_Filter::AV_Filter(int ufl, int dfl, float gain)
{
    UFL = new QSpinBox;
    DFL = new QSpinBox;
    Gain = new QDoubleSpinBox;
    UFL->setValue(ufl);
    DFL->setValue(dfl);
    Gain->setValue(gain);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(new QLabel("上限频率："), 0, 0);
    layout->addWidget(UFL, 0, 1);
    layout->addWidget(new QLabel("下限频率："), 1, 0);
    layout->addWidget(DFL, 1, 1);
    layout->addWidget(new QLabel("增益："), 2, 0);
    layout->addWidget(Gain, 2, 1);
    widget = new QWidget;
    widget->setLayout(layout);
}
AV_Filter::~AV_Filter()
{
    delete UFL;
    delete DFL;
    delete Gain;
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
float AV_Filter::getGain()
{
    return Gain->value();
}
