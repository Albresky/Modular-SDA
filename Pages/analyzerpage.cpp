#include "analyzerpage.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

AnalyzerPage::AnalyzerPage(QWidget* parent, MainWindow* mainWindow) : QWidget(parent),
    mainWindow(mainWindow)
{
    initCharts();
    initLayout();
}

AnalyzerPage::~AnalyzerPage()
{
}

void  AnalyzerPage::initGridChart(QGridLayout* layout, int row, int column, QString title)
{
    XCustomPlot* gridPlot = new XCustomPlot();
    XPlots.append(gridPlot);

    QFrame* frame = new QFrame;
    frame->setStyleSheet(".QFrame{border-radius: 6px;background-color:rgb(255, 255, 255);}");
    auto frameLayout = new QVBoxLayout;
    frameLayout->addWidget(gridPlot);
    frame->setLayout(frameLayout);
    layout->addWidget(frame, row, column);

    gridPlot->showTracer(true);

    gridPlot->plotLayout()->insertRow(0);
    gridPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gridPlot, title, QFont("黑体", 15, QFont::Bold)));
    gridPlot->legend->setVisible(false);

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    gridPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(gridPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), gridPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(gridPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), gridPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    gridPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

//     create graph and assign data to it:
    pen.setColor(Qt::blue);
    pen.setWidth(2);

    QCPGraph* pGraph = gridPlot->addGraph();

    pGraph->setData(default_xdata, default_ydata);
    pGraph->setPen(pen);


    // give the axes some labels:
    gridPlot->xAxis->setVisible(true);
    gridPlot->yAxis->setVisible(true);
    gridPlot->xAxis2->setVisible(true);
    gridPlot->yAxis2->setVisible(true);

    gridPlot->xAxis->setTickLabels(false);
    gridPlot->xAxis2->setTickLabels(false);

    gridPlot->yAxis->setLabel("电压/V");
    // set axes ranges, so we see all data:
    gridPlot->rescaleAxes(true);

    gridPlot->replot();

    showMaximized();
}

void  AnalyzerPage::initSpectrumChart(QGridLayout* layout, int row, int column, QString title)
{

    XCustomPlot* spectrumPlot = new XCustomPlot();
    XPlots.append(spectrumPlot);

    QFrame* frame = new QFrame;
    frame->setStyleSheet(".QFrame{border-radius: 6px;background-color:rgb(255, 255, 255);}");
    auto frameLayout = new QVBoxLayout;
    frameLayout->addWidget(spectrumPlot);
    frame->setLayout(frameLayout);
    layout->addWidget(frame, row, column);

    // layout->addWidget(spectrumPlot, row, column);
    spectrumPlot->showTracer(true);

    spectrumPlot->plotLayout()->insertRow(0);
    spectrumPlot->plotLayout()->addElement(0, 0, new QCPTextElement(spectrumPlot, title, QFont("黑体", 15, QFont::Bold)));
    spectrumPlot->legend->setVisible(false);

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    spectrumPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignCenter);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(spectrumPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), spectrumPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(spectrumPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), spectrumPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    spectrumPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // create graph and assign data to it:
    QPen graphPen;
    graphPen.setColor(QColor(106, 176, 121));
    graphPen.setWidth(2);

    QCPGraph* pGraph = spectrumPlot->addGraph();

    pGraph->setData(default_xdata, default_ydata);
    pGraph->setPen(graphPen);
    pGraph->setLineStyle(QCPGraph::LineStyle::lsImpulse);
    pGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));


    // give the axes some labels:
    spectrumPlot->xAxis->setLabel("频率/Hz");
    spectrumPlot->yAxis->setLabel("幅值/V");

    // set axes ranges, so we see all data:
    spectrumPlot->rescaleAxes(true);

    spectrumPlot->xAxis->setVisible(true);
    spectrumPlot->yAxis->setVisible(true);
    spectrumPlot->xAxis2->setVisible(true);
    spectrumPlot->yAxis2->setVisible(true);

    spectrumPlot->replot();

    showMaximized();
}

void AnalyzerPage::initCharts()
{
    initDefaultData();
    chartsLayout = new QGridLayout;
    initGridChart(chartsLayout, 0, 0, "输入信号波形图");     // 0 input grid
    initSpectrumChart(chartsLayout, 0, 1, "输入信号频谱图"); // 1 input spectrum
    initGridChart(chartsLayout, 1, 0, "输出信号波形图");     // 2 output grid
    initSpectrumChart(chartsLayout, 1, 1, "输出信号频谱图"); // 3 output spectrum
}

void AnalyzerPage::initLayout()
{
    antialiasCheckBox = new QCheckBox("抗锯齿");
    ingoreEmptyDataCheckBox = new QCheckBox("忽略空数据");
    ingoreInvalidDataCheckBox = new QCheckBox("忽略非法据");
    browser_validValue = new QLineEdit();
    browser_baseFreq = new QLineEdit();
    browser_baseAmplitude = new QLineEdit();
    browser_thirdOrderFreq = new QLineEdit();
    browser_thirdOrderAmplitude = new QLineEdit();
    browser_fifthOrderFreq = new QLineEdit();
    browser_fifthOrderAmplitude = new QLineEdit();
    browser_seventhOrderFreq = new QLineEdit();
    browser_seventhOrderAmplitude = new QLineEdit();
    browser_dftResult = new QLineEdit();
    browser_validValue->setReadOnly(true);
    browser_validValue->setFixedWidth(80);
    browser_baseFreq->setReadOnly(true);
    browser_baseFreq->setFixedWidth(80);
    browser_baseAmplitude->setReadOnly(true);
    browser_baseAmplitude->setFixedWidth(80);
    browser_thirdOrderFreq->setReadOnly(true);
    browser_thirdOrderFreq->setFixedWidth(80);
    browser_thirdOrderAmplitude->setReadOnly(true);
    browser_thirdOrderAmplitude->setFixedWidth(80);
    browser_fifthOrderFreq->setReadOnly(true);
    browser_fifthOrderFreq->setFixedWidth(80);
    browser_fifthOrderAmplitude->setReadOnly(true);
    browser_fifthOrderAmplitude->setFixedWidth(80);
    browser_seventhOrderFreq->setReadOnly(true);
    browser_seventhOrderFreq->setFixedWidth(80);
    browser_seventhOrderAmplitude->setReadOnly(true);
    browser_seventhOrderAmplitude->setFixedWidth(80);

    browser_dftResult->setReadOnly(true);
    browser_dftResult->setFixedWidth(80);

    // QObject::connect(cbox_legend, &QComboBox::currentIndexChanged, this, &AnalyzerPage::updateUI);
    // QObject::connect(cbox_theme, &QComboBox::currentIndexChanged, this, &AnalyzerPage::updateUI);
    // QObject::connect(cbox_animation, &QComboBox::currentIndexChanged, this, &AnalyzerPage::updateUI);
    // QObject::connect(antialiasCheckBox, &QCheckBox::stateChanged, this, &AnalyzerPage::updateUI);


    btn_updatePlot = new QPushButton();
    btn_updatePlot->setText("更新波形图");
    QObject::connect(btn_updatePlot, &QPushButton::clicked, this, &AnalyzerPage::loadParameters);
    QObject::connect(this, &AnalyzerPage::updatePlot, mainWindow, &MainWindow::sendSerialStart);

    btn_clearPlot = new QPushButton();
    btn_clearPlot->setText("重置波形图");
    QObject::connect(btn_clearPlot, &QPushButton::clicked, this, &AnalyzerPage::btn_clear_plot_clicked);

    QFrame* dividingLine1 = new QFrame();
    dividingLine1->setFrameShape(QFrame::HLine);
    dividingLine1->setFrameShadow(QFrame::Sunken);

    commonAnalysesResultsFrame = new QFrame();
    QVBoxLayout* commonAnalysesResultsFrame_layout = new QVBoxLayout;

    QFrame* validValueFrame = new QFrame();
    QHBoxLayout* validValueLayout = new QHBoxLayout;
    QLabel* validValueLabel = new QLabel("直流偏置");
    validValueLabel->setAlignment(Qt::AlignCenter);
    validValueLabel->setFixedWidth(80);
    validValueLayout->addWidget(validValueLabel);
    validValueLayout->addWidget(browser_validValue);
    validValueLayout->addWidget(new QLabel("V"));
    validValueFrame->setLayout(validValueLayout);

    QFrame* baseFreqFrame = new QFrame();
    QHBoxLayout* baseFreqLayout = new QHBoxLayout;
    QLabel* baseFreqLabel = new QLabel("基波频率");
    baseFreqLabel->setAlignment(Qt::AlignCenter);
    baseFreqLabel->setFixedWidth(80);
    baseFreqLayout->addWidget(baseFreqLabel);
    baseFreqLayout->addWidget(browser_baseFreq);
    baseFreqLayout->addWidget(new QLabel("Hz"));
    baseFreqFrame->setLayout(baseFreqLayout);

    QFrame* baseAmplitudeFrame = new QFrame();
    QHBoxLayout* baseAmplitudeLayout = new QHBoxLayout;
    QLabel* baseAmplitudeLabel = new QLabel("基波幅值");
    baseAmplitudeLabel->setAlignment(Qt::AlignCenter);
    baseAmplitudeLabel->setFixedWidth(80);
    baseAmplitudeLayout->addWidget(baseAmplitudeLabel);
    baseAmplitudeLayout->addWidget(browser_baseAmplitude);
    baseAmplitudeLayout->addWidget(new QLabel("V"));
    baseAmplitudeFrame->setLayout(baseAmplitudeLayout);

    QFrame* thirdOrderFreqFrame = new QFrame();
    QHBoxLayout* thirdOrderFreqLayout = new QHBoxLayout;
    QLabel* thirdOrderFreqLabel = new QLabel("三次谐波频率");
    thirdOrderFreqLabel->setFixedWidth(80);
    thirdOrderFreqLayout->addWidget(thirdOrderFreqLabel);
    thirdOrderFreqLayout->addWidget(browser_thirdOrderFreq);
    thirdOrderFreqLayout->addWidget(new QLabel("Hz"));
    thirdOrderFreqFrame->setLayout(thirdOrderFreqLayout);

    QFrame* thirdOrderAmplitudeFrame = new QFrame();
    QHBoxLayout* thirdOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* thirdOrderAmplitudeLabel = new QLabel("三次谐波幅值");
    thirdOrderAmplitudeLabel->setFixedWidth(80);
    thirdOrderAmplitudeLayout->addWidget(thirdOrderAmplitudeLabel);
    thirdOrderAmplitudeLayout->addWidget(browser_thirdOrderAmplitude);
    thirdOrderAmplitudeLayout->addWidget(new QLabel("V"));
    thirdOrderAmplitudeFrame->setLayout(thirdOrderAmplitudeLayout);

    QFrame* fifthOrderFreqFrame = new QFrame();
    QHBoxLayout* fifthOrderFreqLayout = new QHBoxLayout;
    QLabel* fifthOrderFreqLabel = new QLabel("五次谐波频率");
    fifthOrderFreqLabel->setFixedWidth(80);
    fifthOrderFreqLayout->addWidget(fifthOrderFreqLabel);
    fifthOrderFreqLayout->addWidget(browser_fifthOrderFreq);
    fifthOrderFreqLayout->addWidget(new QLabel("Hz"));
    fifthOrderFreqFrame->setLayout(fifthOrderFreqLayout);

    QFrame* fifthOrderAmplitudeFrame = new QFrame();
    QHBoxLayout* fifthOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* fifthOrderAmplitudeLabel = new QLabel("五次谐波幅值");
    fifthOrderAmplitudeLabel->setFixedWidth(80);
    fifthOrderAmplitudeLayout->addWidget(fifthOrderAmplitudeLabel);
    fifthOrderAmplitudeLayout->addWidget(browser_fifthOrderAmplitude);
    fifthOrderAmplitudeLayout->addWidget(new QLabel("V"));
    fifthOrderAmplitudeFrame->setLayout(fifthOrderAmplitudeLayout);

    QFrame* seventhOrderFreqFrame = new QFrame();
    QHBoxLayout* seventhOrderFreqLayout = new QHBoxLayout;
    QLabel* seventhOrderFreqLabel = new QLabel("七次谐波频率");
    seventhOrderFreqLabel->setFixedWidth(80);
    seventhOrderFreqLayout->addWidget(seventhOrderFreqLabel);
    seventhOrderFreqLayout->addWidget(browser_seventhOrderFreq);
    seventhOrderFreqLayout->addWidget(new QLabel("Hz"));
    seventhOrderFreqFrame->setLayout(seventhOrderFreqLayout);

    QFrame* seventhOrderAmplitudeFrame = new QFrame();
    QHBoxLayout* seventhOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* seventhOrderAmplitudeLabel = new QLabel("七次谐波幅值");
    seventhOrderAmplitudeLabel->setFixedWidth(80);
    seventhOrderAmplitudeLayout->addWidget(seventhOrderAmplitudeLabel);
    seventhOrderAmplitudeLayout->addWidget(browser_seventhOrderAmplitude);
    seventhOrderAmplitudeLayout->addWidget(new QLabel("V"));
    seventhOrderAmplitudeFrame->setLayout(seventhOrderAmplitudeLayout);

    defResultFrame = new QFrame();
    QHBoxLayout* dftResult_layout = new QHBoxLayout;
    QLabel* dft_Result_label = new QLabel("DFT结果");
    dft_Result_label->setFixedWidth(80);
    dftResult_layout->addWidget(dft_Result_label);
    dftResult_layout->addWidget(browser_dftResult);
    dftResult_layout->addWidget(new QLabel("V"));
    defResultFrame->setLayout(dftResult_layout);
    defResultFrame->hide();

    commonAnalysesResultsFrame_layout->addWidget(validValueFrame);
    commonAnalysesResultsFrame_layout->addWidget(baseFreqFrame);
    commonAnalysesResultsFrame_layout->addWidget(baseAmplitudeFrame);
    commonAnalysesResultsFrame_layout->addWidget(thirdOrderFreqFrame);
    commonAnalysesResultsFrame_layout->addWidget(thirdOrderAmplitudeFrame);
    commonAnalysesResultsFrame_layout->addWidget(fifthOrderFreqFrame);
    commonAnalysesResultsFrame_layout->addWidget(fifthOrderAmplitudeFrame);
    commonAnalysesResultsFrame_layout->addWidget(seventhOrderFreqFrame);
    commonAnalysesResultsFrame_layout->addWidget(seventhOrderAmplitudeFrame);
    commonAnalysesResultsFrame->setLayout(commonAnalysesResultsFrame_layout);
    commonAnalysesResultsFrame->hide();

    QVBoxLayout* parametersLayout = new QVBoxLayout;

    parametersLayout->addWidget(antialiasCheckBox);
    parametersLayout->addWidget(ingoreEmptyDataCheckBox);
    parametersLayout->addWidget(ingoreInvalidDataCheckBox);
    parametersLayout->addWidget(btn_updatePlot);
    parametersLayout->addWidget(btn_clearPlot);
    parametersLayout->addWidget(dividingLine1);
    parametersLayout->addWidget(defResultFrame);
    parametersLayout->addWidget(commonAnalysesResultsFrame);

    QWidget* paramsBox = new QWidget();
    paramsBox->setFixedWidth(250);
    paramsBox->setLayout(parametersLayout);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addLayout(chartsLayout);
    layout->addWidget(paramsBox);
    this->setLayout(layout);
}


void AnalyzerPage::initDefaultData()
{
    for (int i = 0; i < 70; ++i)
    {
        default_xdata.append(i);
        default_ydata.append(0);
    }
}



void AnalyzerPage::updateChartData(QVector<double> pointsDataX, QVector<double> pointsDataY, int index)
{
    XCustomPlot* p = XPlots[index];
    p->graph()->setData(pointsDataX, pointsDataY);
    p->rescaleAxes(true);
    p->replot();
}

void AnalyzerPage::updateAnalyses(AnalysisVaule analysisValue)
{
    browser_validValue->setText(QString::number(analysisValue.validValue, 'f', 4));
    browser_baseFreq->setText(QString::number(analysisValue.baseFreq));
    browser_baseAmplitude->setText(QString::number(analysisValue.baseAmp, 'f', 4));
    browser_thirdOrderFreq->setText(QString::number(analysisValue._3rdFreq));
    browser_thirdOrderAmplitude->setText(QString::number(analysisValue._3rdAmp, 'f', 4));
    browser_fifthOrderFreq->setText(QString::number(analysisValue._5thFreq));
    browser_fifthOrderAmplitude->setText(QString::number(analysisValue._5thAmp, 'f', 4));
    browser_seventhOrderFreq->setText(QString::number(analysisValue._7thFreq));
    browser_seventhOrderAmplitude->setText(QString::number(analysisValue._7thAmp, 'f', 4));
}


bool AnalyzerPage::isIngoreEmptyDataChecked()
{
    return ingoreEmptyDataCheckBox->isChecked();
}

bool AnalyzerPage::isIngoreInvalidDataChecked()
{
    return ingoreInvalidDataCheckBox->isChecked();
}


void AnalyzerPage::btn_clear_plot_clicked()
{
    foreach(XCustomPlot* p, XPlots)
    {
        p->graph()->setData(default_xdata, default_ydata);
        p->replot();
    }
}

void AnalyzerPage::setDftVisibility(bool visiable)
{
    qDebug() << "setDftVisibility()";
    visiable ? defResultFrame->show() : defResultFrame->hide();
}

void AnalyzerPage::set_FFT_IFFT_HT_Visibility(bool visiable)
{
    qDebug() << "set_FFT_IFFT_HT_Visibility()";
    visiable ? commonAnalysesResultsFrame->show() : commonAnalysesResultsFrame->hide();
}

void AnalyzerPage::setDiagramScene(DiagramScene* scene)
{
    this->scene = scene;
}

void AnalyzerPage::loadParameters()
{
    bool flag = true;
    if(!commonAnalysesResultsFrame->isHidden())
    {
        /* TODO
        *  select DiagramItem::Module
        */
        if(scene->isModuleAdded(DiagramItem::HT))
        {
            AV_HT* module = dynamic_cast<AV_HT*>(scene->getModule(DiagramItem::HT)->av_DataItem());
            pointCnt = module->getPointCnt();
            sampleFreq = module->getSampleFreq();
        }
        else
        {
            AV_FFT_IFFT* module = dynamic_cast<AV_FFT_IFFT*>(scene->getModule(DiagramItem::FFT)->av_DataItem());
            pointCnt = module->getPointCnt();
            sampleFreq = module->getSampleFreq();
        }
        flag = false;
        qDebug() << "pointCnt:" << pointCnt;
        qDebug() << "sampleFreq:" << sampleFreq;
    }
    if(!defResultFrame->isHidden())
    {
        AV_DFT* module = dynamic_cast<AV_DFT*>(scene->getModule(DiagramItem::DFT)->av_DataItem());
        calcuFreq = module->getCalcuFreq();
        flag = false;
        qDebug() << "calcuFreq:" << calcuFreq;
    }
    if(flag)
    {
        QMessageBox::warning(this, tr("警告"), tr("未选择算法模块！"));
        return;
    }
    if(!mainWindow->isSerialPortOnline())
    {
        QMessageBox::warning(this, tr("警告"), tr("串口未连接！"));
        return;
    }

    emit updatePlot();
}

int AnalyzerPage::getPointCnt()
{
    return pointCnt;
}

int AnalyzerPage::getSampleFreq()
{
    return sampleFreq;
}

int AnalyzerPage::getCalcuFreq()
{
    return calcuFreq;
}


void AnalyzerPage::updateDFT(double value)
{
    browser_dftResult->setText(QString::number(value));
}
