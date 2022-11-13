#ifndef DATAITEM_H
#define DATAITEM_H

#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>

struct AnalysisVaule
{
    double validValue;
    double baseFreq;
    double baseAmp;
    double _3rdFreq;
    double _3rdAmp;
    double _5thFreq;
    double _5thAmp;
    double _7thFreq;
    double _7thAmp;
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


class AV_FFT_IFFT: public AV_DataItem
{
public:
    AV_FFT_IFFT(int pointCnt = 0, int sampleFreq = 0);
    ~AV_FFT_IFFT();

    QWidget* Widget() const override
    {
        return widget;
    }
    int getPointCnt();
    int getSampleFreq();
private:
    QWidget* widget;
    QSpinBox* PointCnt;
    QSpinBox* SampleFreq;
};

class AV_DFT: public AV_DataItem
{
public:
    AV_DFT(int pointCnt = 0, int sampleFreq = 0, int calcuFreq = 0);
    ~AV_DFT();

    QWidget* Widget() const override
    {
        return widget;
    }
    int getPointCnt();
    int getSampleFreq();
    int getCalcuFreq();
private:
    QWidget* widget;
    QSpinBox* PointCnt;
    QSpinBox* SampleFreq;
    QSpinBox* CalcuFreq;
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
    AV_HT(int pointCnt = 0, int sampleFreq = 0);
    ~AV_HT();
    int getPointCnt();
    int getSampleFreq();

    QWidget* Widget() const override
    {
        return widget;
    }
private:
    QWidget* widget;
    QSpinBox* PointCnt;
    QSpinBox* SampleFreq;
};

class AV_Filter: public AV_DataItem
{
public:
    AV_Filter(int ufl = 0, int dfl = 0, double passbandgain = 0.0, double stopbandgain = 0.0);
    ~AV_Filter();
    int getUFL();
    int getDFL();
    double getPassBandGain();
    double getStopBandGain();

    QWidget* Widget() const override
    {
        return widget;
    }
private:
    QWidget* widget;
    QSpinBox* UFL;
    QSpinBox* DFL;
    QDoubleSpinBox* PassBandGain;
    QDoubleSpinBox* StopBandGain;
};

#endif // DATAITEM_H
