#ifndef DATAITEM_H
#define DATAITEM_H

#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>

struct AnalysisVaule
{
    int validValue;
    float baseFreq;
    int baseAmp;
    float _3rdFreq;
    int _3rdAmp;
    float _5thFreq;
    int _5thAmp;
    float _7thFreq;
    int _7thAmp;
};


class AV_DataItem
{
public:
    AV_DataItem();
    ~AV_DataItem();

    virtual QWidget* Widget() const
    {
        return new QLabel("当前模块无参数");
    };
};


class AV_Input: public AV_DataItem
{
public:
    AV_Input(int sampleFreq = 0, int dataLen = 0);
    ~AV_Input();

    QWidget* Widget() const override
    {
        return widget;
    }

    int getSampleFreq();
    int getDataLen();
private:
    QWidget* widget;
    QSpinBox* SampleFreq;
    QSpinBox* DataLen;
};


class AV_FFT_IFFT_DFT: public AV_DataItem
{
public:
    AV_FFT_IFFT_DFT(int pointCnt = 0, int signalFreq = 0, int sampleFreq = 0);
    ~AV_FFT_IFFT_DFT();

    QWidget* Widget() const override
    {
        return widget;
    }
    int getPointCnt();
    int getSignalFreq();
    int getSampleFreq();
private:
    QWidget* widget;
    QSpinBox* PointCnt;
    QSpinBox* SignalFreq;
    QSpinBox* SampleFreq;
};

class AV_HanningWin_BlackmanWin: public AV_DataItem
{
public:
    AV_HanningWin_BlackmanWin(int pointCnt = 0);
    ~AV_HanningWin_BlackmanWin();

    QWidget* Widget() const override
    {
        return widget;
    }
    int getPointCnt();
private:
    QWidget* widget;
    QSpinBox* PointCnt;

};

class AV_HT : public AV_DataItem
{
public:
    AV_HT(int pointCnt = 0, int signalFreq = 0, int sampleFreq = 0);
    ~AV_HT();
    int getPointCnt();
    int getSignalFreq();
    int getSampleFreq();

    QWidget* Widget() const override
    {
        return widget;
    }
private:
    QWidget* widget;
    QSpinBox* PointCnt;
    QSpinBox* SignalFreq;
    QSpinBox* SampleFreq;
};

class AV_Filter: public AV_DataItem
{
public:
    AV_Filter(int ufl = 0, int dfl = 0, float gain = 0.0);
    ~AV_Filter();
    int getUFL();
    int getDFL();
    float getGain();


    QWidget* Widget() const override
    {
        return widget;
    }
private:
    QWidget* widget;
    QSpinBox* UFL;
    QSpinBox* DFL;
    QDoubleSpinBox* Gain;
};

#endif // DATAITEM_H
