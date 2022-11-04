#include "themewidget.h"

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

ThemeWidget::ThemeWidget(QWidget* parent, MainWindow* mainWindow) : QWidget(parent),
    mainWindow(mainWindow),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(7),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    initCharts();
    initLayout();

    populateThemeBox();
    populateAnimationBox();
    populateLegendBox();


    // Set defaults
    antialiasCheckBox->setChecked(true);

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);

    updateUI();
}

ThemeWidget::~ThemeWidget()
{
}

void  ThemeWidget::initGridChart(QLayout* layout)
{
    XCustomPlot* gridPlot = new XCustomPlot();
    XPlots.append(gridPlot);
    layout->addWidget(gridPlot);
    gridPlot->showTracer(true);

    gridPlot->plotLayout()->insertRow(0);
    gridPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gridPlot, "波形图", QFont("黑体", 15, QFont::Bold)));
    gridPlot->legend->setVisible(false);

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    gridPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(gridPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), gridPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(gridPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), gridPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    gridPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // generate some data:
    int nCount = 100;
    QVector<double> x(nCount), y(nCount); // initialize with entries 0..100
    for (int i = 0; i < nCount; ++i)
    {
        x[i] = i; // x goes from -1 to 1
        y[i] = qSin(i * 10.0f / nCount); //sin

    }
    // create graph and assign data to it:
//    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);

    QCPGraph* pGraph = gridPlot->addGraph();

    pGraph->setData(x, y);
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

void  ThemeWidget::initSpectrumChart(QLayout* layout)
{

    XCustomPlot* spectrumPlot = new XCustomPlot();
    XPlots.append(spectrumPlot);
    layout->addWidget(spectrumPlot);
    spectrumPlot->showTracer(true);

    spectrumPlot->plotLayout()->insertRow(0);
    spectrumPlot->plotLayout()->addElement(0, 0, new QCPTextElement(spectrumPlot, "频谱图", QFont("黑体", 15, QFont::Bold)));
    spectrumPlot->legend->setVisible(false);

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    spectrumPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignCenter);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(spectrumPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), spectrumPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(spectrumPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), spectrumPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    spectrumPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // generate some data:
    int nCount = 100;
    QVector<double> x(nCount), y(nCount); // initialize with entries 0..100
    for (int i = 0; i < nCount; ++i)
    {
        x[i] = i; // x goes from -1 to 1
        y[i] = qSin(i * 10.0f / nCount); //sin

    }
    // create graph and assign data to it:
    QPen graphPen;
    graphPen.setColor(QColor(106, 176, 121));
    graphPen.setWidth(2);

    QCPGraph* pGraph = spectrumPlot->addGraph();

    pGraph->setData(x, y);
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

void ThemeWidget::initCharts()
{
    /*   QChartView* chartView;
       chartsLayout = new QVBoxLayout;
       chartView = new QChartView(createSplineChart("网格图"));
       chartsLayout->addWidget(chartView);
       m_charts << chartView;
       spline_chartView = chartView;
    */
    chartsLayout = new QVBoxLayout;
    inputLayout = new QHBoxLayout;
    outputLayout = new QHBoxLayout;
    initGridChart(inputLayout);         // 0
    initSpectrumChart(inputLayout);     // 1
    initGridChart(outputLayout);        // 2
    initSpectrumChart(outputLayout);    // 3
    chartsLayout->addLayout(inputLayout);
    chartsLayout->addLayout(outputLayout);

}

void ThemeWidget::initLayout()
{
    cbox_legend = new QComboBox();
    cbox_theme = new QComboBox();
    cbox_animation = new QComboBox();
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
    browser_validValue->setReadOnly(true);
    browser_baseFreq->setReadOnly(true);
    browser_baseAmplitude->setReadOnly(true);
    browser_thirdOrderFreq->setReadOnly(true);
    browser_thirdOrderAmplitude->setReadOnly(true);
    browser_fifthOrderFreq->setReadOnly(true);
    browser_fifthOrderAmplitude->setReadOnly(true);
    browser_seventhOrderFreq->setReadOnly(true);
    browser_seventhOrderAmplitude->setReadOnly(true);

    QObject::connect(cbox_legend, &QComboBox::currentIndexChanged, this, &ThemeWidget::updateUI);
    QObject::connect(cbox_theme, &QComboBox::currentIndexChanged, this, &ThemeWidget::updateUI);
    QObject::connect(cbox_animation, &QComboBox::currentIndexChanged, this, &ThemeWidget::updateUI);
    QObject::connect(antialiasCheckBox, &QCheckBox::stateChanged, this, &ThemeWidget::updateUI);

    QHBoxLayout* themeLayout = new QHBoxLayout;
    QLabel* themeLabel = new QLabel("主题");
    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(cbox_theme);

    QHBoxLayout* legendLayout = new QHBoxLayout;
    QLabel* legendLabel = new QLabel("图例");
    themeLayout->addWidget(legendLabel);
    themeLayout->addWidget(cbox_legend);

    QHBoxLayout* animationLayout = new QHBoxLayout;
    QLabel* animationLabel = new QLabel("动画");
    themeLayout->addWidget(animationLabel);
    themeLayout->addWidget(cbox_animation);

    btn_updatePlot = new QPushButton();
    btn_updatePlot->setText("更新波形图");
    QObject::connect(btn_updatePlot, &QPushButton::clicked, mainWindow, &MainWindow::sendSerialStart);


    QFrame* dividingLine1 = new QFrame();
    dividingLine1->setFrameShape(QFrame::HLine);
    dividingLine1->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* validValueLayout = new QHBoxLayout;
    QLabel* validValueLabel = new QLabel("有效值");
    validValueLabel->setAlignment(Qt::AlignCenter);
    validValueLabel->setFixedWidth(80);
    validValueLayout->addWidget(validValueLabel);
    validValueLayout->addWidget(browser_validValue);

    QHBoxLayout* baseFreqLayout = new QHBoxLayout;
    QLabel* baseFreqLabel = new QLabel("基波频率");
    baseFreqLabel->setAlignment(Qt::AlignCenter);
    baseFreqLabel->setFixedWidth(80);
    baseFreqLayout->addWidget(baseFreqLabel);
    baseFreqLayout->addWidget(browser_baseFreq);

    QHBoxLayout* baseAmplitudeLayout = new QHBoxLayout;
    QLabel* baseAmplitudeLabel = new QLabel("基波幅值");
    baseAmplitudeLabel->setAlignment(Qt::AlignCenter);
    baseAmplitudeLabel->setFixedWidth(80);
    baseAmplitudeLayout->addWidget(baseAmplitudeLabel);
    baseAmplitudeLayout->addWidget(browser_baseAmplitude);

    QHBoxLayout* thirdOrderFreqLayout = new QHBoxLayout;
    QLabel* thirdOrderFreqLabel = new QLabel("三次谐波频率");
    thirdOrderFreqLabel->setFixedWidth(80);
    thirdOrderFreqLayout->addWidget(thirdOrderFreqLabel);
    thirdOrderFreqLayout->addWidget(browser_thirdOrderFreq);

    QHBoxLayout* thirdOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* thirdOrderAmplitudeLabel = new QLabel("三次谐波幅值");
    thirdOrderAmplitudeLabel->setFixedWidth(80);
    thirdOrderAmplitudeLayout->addWidget(thirdOrderAmplitudeLabel);
    thirdOrderAmplitudeLayout->addWidget(browser_thirdOrderAmplitude);

    QHBoxLayout* fifthOrderFreqLayout = new QHBoxLayout;
    QLabel* fifthOrderFreqLabel = new QLabel("五次谐波频率");
    fifthOrderFreqLabel->setFixedWidth(80);
    fifthOrderFreqLayout->addWidget(fifthOrderFreqLabel);
    fifthOrderFreqLayout->addWidget(browser_fifthOrderFreq);

    QHBoxLayout* fifthOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* fifthOrderAmplitudeLabel = new QLabel("五次谐波幅值");
    fifthOrderAmplitudeLabel->setFixedWidth(80);
    fifthOrderAmplitudeLayout->addWidget(fifthOrderAmplitudeLabel);
    fifthOrderAmplitudeLayout->addWidget(browser_fifthOrderAmplitude);

    QHBoxLayout* seventhOrderFreqLayout = new QHBoxLayout;
    QLabel* seventhOrderFreqLabel = new QLabel("七次谐波频率");
    seventhOrderFreqLabel->setFixedWidth(80);
    seventhOrderFreqLayout->addWidget(seventhOrderFreqLabel);
    seventhOrderFreqLayout->addWidget(browser_seventhOrderFreq);

    QHBoxLayout* seventhOrderAmplitudeLayout = new QHBoxLayout;
    QLabel* seventhOrderAmplitudeLabel = new QLabel("七次谐波幅值");
    seventhOrderAmplitudeLabel->setFixedWidth(80);
    seventhOrderAmplitudeLayout->addWidget(seventhOrderAmplitudeLabel);
    seventhOrderAmplitudeLayout->addWidget(browser_seventhOrderAmplitude);

    QVBoxLayout* parametersLayout = new QVBoxLayout;
    parametersLayout->addLayout(themeLayout);
    parametersLayout->addLayout(legendLayout);
    parametersLayout->addLayout(animationLayout);
    parametersLayout->addWidget(antialiasCheckBox);
    parametersLayout->addWidget(ingoreEmptyDataCheckBox);
    parametersLayout->addWidget(ingoreInvalidDataCheckBox);
    parametersLayout->addWidget(btn_updatePlot);
    parametersLayout->addWidget(dividingLine1);
    parametersLayout->addLayout(validValueLayout);
    parametersLayout->addLayout(baseFreqLayout);
    parametersLayout->addLayout(baseAmplitudeLayout);
    parametersLayout->addLayout(thirdOrderFreqLayout);
    parametersLayout->addLayout(thirdOrderAmplitudeLayout);
    parametersLayout->addLayout(fifthOrderFreqLayout);
    parametersLayout->addLayout(fifthOrderAmplitudeLayout);
    parametersLayout->addLayout(seventhOrderFreqLayout);
    parametersLayout->addLayout(seventhOrderAmplitudeLayout);
    QWidget* paramsBox = new QWidget();
    paramsBox->setFixedWidth(200);
    paramsBox->setLayout(parametersLayout);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addLayout(chartsLayout);
    layout->addWidget(paramsBox);
    this->setLayout(layout);
}

DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // generate random data
    for (int i(0); i < listCount; i++)
    {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++)
        {
            yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal)valueCount);
            QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal)m_valueMax / (qreal)valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

void ThemeWidget::populateThemeBox()
{
    // add items to theme combobox
    cbox_theme->addItem("Light", QChart::ChartThemeLight);
    cbox_theme->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    cbox_theme->addItem("Dark", QChart::ChartThemeDark);
    cbox_theme->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    cbox_theme->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    cbox_theme->addItem("High Contrast", QChart::ChartThemeHighContrast);
    cbox_theme->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    cbox_theme->addItem("Qt", QChart::ChartThemeQt);
}

void ThemeWidget::populateAnimationBox()
{
    // add items to animation combobox
    cbox_animation->addItem("No Animations", QChart::NoAnimation);
    cbox_animation->addItem("GridAxis Animations", QChart::GridAxisAnimations);
    cbox_animation->addItem("Series Animations", QChart::SeriesAnimations);
    cbox_animation->addItem("All Animations", QChart::AllAnimations);
}

void ThemeWidget::populateLegendBox()
{
    // add items to legend combobox
    cbox_legend->addItem("No Legend ", 0);
    cbox_legend->addItem("Legend Top", Qt::AlignTop);
    cbox_legend->addItem("Legend Bottom", Qt::AlignBottom);
    cbox_legend->addItem("Legend Left", Qt::AlignLeft);
    cbox_legend->addItem("Legend Right", Qt::AlignRight);
}


QChart* ThemeWidget::createBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart* chart = new QChart();
    chart->setTitle("Bar chart");

    QBarSeries* series = new QBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++)
    {
        QBarSet* set = new QBarSet("Bar set " + QString::number(i));
        for (const Data& data : m_dataTable[i])
        {
            *set << data.first.y();
        }
        series->append(set);
    }
    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, m_valueMax * 2);
    // Add space to label to add space between labels and axis
    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f");

    return chart;
}


QChart* ThemeWidget::createSplineChart(QString title)
{
    QChart* chart = new QChart();
    chart->setTitle(title);
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList& list : m_dataTable)
    {
        QSplineSeries* series = new QSplineSeries(chart);
        for (const Data& data : list)
        {
            series->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }


    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, m_valueMax);
    chart->axes(Qt::Vertical).first()->setRange(0, m_valueCount);

    // Add space to label to add space between labels and axis

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    pen.setWidth(1);

    QList<QAbstractAxis*> xAxies = chart->axes(Qt::Horizontal);
    foreach(QAbstractAxis* axis, xAxies)
    {
        axis->setLinePen(pen);
        axis->setGridLinePen(pen);
    }

    QList<QAbstractAxis*> yAxies = chart->axes(Qt::Horizontal);
    foreach(QAbstractAxis* axis, yAxies)
    {
        axis->setLinePen(pen);

        axis->setGridLinePen(pen);
    }

    QValueAxis* axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    Q_ASSERT(axisX);
    axisX->setLabelFormat("%d");
    axisX->setTitleFont(QFont("宋体"));
    axisX->setGridLineVisible(true);
    axisX->setGridLineColor(Qt::black);
    axisX->setGridLinePen(pen);
    axisX->setMinorGridLineVisible(false);


    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.2f");
    axisY->setTitleFont(QFont("宋体"));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(pen);
    axisY->setGridLineColor(Qt::black);
    axisY->setMinorGridLineVisible(false);

    return chart;
}



void ThemeWidget::updateUI()
{

    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(cbox_theme->itemData(cbox_theme->currentIndex()).toInt());

    const auto charts = m_charts;
    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->theme() != theme)
    {
        for (QChartView* chartView : charts)
        {
            chartView->chart()->setTheme(theme);
        }

        // Set palette colors based on selected theme

        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight)
        {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeDark)
        {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        }
        else if (theme == QChart::ChartThemeBlueCerulean)
        {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        }
        else if (theme == QChart::ChartThemeBrownSand)
        {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeBlueNcs)
        {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else if (theme == QChart::ChartThemeHighContrast)
        {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        }
        else if (theme == QChart::ChartThemeBlueIcy)
        {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        else
        {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }

    // Update antialiasing
    bool checked = antialiasCheckBox->isChecked();
    for (QChartView* chart : charts)
    {
        chart->setRenderHint(QPainter::Antialiasing, checked);
    }

    // Update animation options
    QChart::AnimationOptions options(
        cbox_animation->itemData(cbox_animation->currentIndex()).toInt());
    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->animationOptions() != options)
    {
        for (QChartView* chartView : charts)
        {
            chartView->chart()->setAnimationOptions(options);
        }
    }

    // Update legend alignment
    Qt::Alignment alignment(
        cbox_legend->itemData(cbox_legend->currentIndex()).toInt());

    if (!alignment)
    {
        for (QChartView* chartView : charts)
        {
            chartView->chart()->legend()->hide();
        }
    }
    else
    {
        for (QChartView* chartView : charts)
        {
            chartView->chart()->legend()->setAlignment(alignment);
            chartView->chart()->legend()->show();
        }
    }
}



void ThemeWidget::updateChartData(QVector<double> pointsDataX, QVector<double> pointsDataY, int index)
{
    XCustomPlot* p = XPlots[index];
    p->graph()->setData(pointsDataX, pointsDataY);
    p->rescaleAxes(true);
    p->replot();
}

void ThemeWidget::updateAnalyses(AnalysisVaule analysisValue)
{
    browser_validValue->setText(QString::number(analysisValue.validValue));
    browser_baseFreq->setText(QString::number(analysisValue.baseFreq));
    browser_baseAmplitude->setText(QString::number(analysisValue.baseAmp));
    browser_thirdOrderFreq->setText(QString::number(analysisValue._3rdFreq));
    browser_thirdOrderAmplitude->setText(QString::number(analysisValue._3rdAmp));
    browser_fifthOrderFreq->setText(QString::number(analysisValue._5thFreq));
    browser_fifthOrderAmplitude->setText(QString::number(analysisValue._5thAmp));
    browser_seventhOrderFreq->setText(QString::number(analysisValue._7thFreq));
    browser_seventhOrderAmplitude->setText(QString::number(analysisValue._7thAmp));
}


bool ThemeWidget::isIngoreEmptyDataChecked()
{
    return ingoreEmptyDataCheckBox->isChecked();
}

bool ThemeWidget::isIngoreInvalidDataChecked()
{
    return ingoreInvalidDataCheckBox->isChecked();
}


void ThemeWidget::updateAxisXMax(int value)
{
    axisXMax = value;
}

void ThemeWidget::updateAxisYMax(int value)
{
    axisYMax = value;
}
