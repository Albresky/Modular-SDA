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

ThemeWidget::ThemeWidget(QWidget* parent) : QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(7),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    initLayout();

    populateThemeBox();
    populateAnimationBox();
    populateLegendBox();

    // create charts

    QChartView* chartView;

    //    chartView = new QChartView(createAreaChart());
    //    m_ui->gridLayout->addWidget(chartView, 1, 0);
    //    m_charts << chartView;

    //    chartView = new QChartView(createPieChart());
    //    // Funny things happen if the pie slice labels do not fit the screen, so we ignore size policy
    //    chartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //    m_ui->gridLayout->addWidget(chartView, 1, 1);
    //    m_charts << chartView;

    //    chartView = new QChartView(createLineChart());
    //    m_ui->gridLayout->addWidget(chartView, 1, 2);

    //    m_charts << chartView;

    //    chartView = new QChartView(createBarChart(m_valueCount));
    //    m_ui->gridLayout->addWidget(chartView, 2, 0);
    //    m_charts << chartView;

    chartView = new QChartView(createSplineChart("Chart 1"));
    gridLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    chartView = new QChartView(createSplineChart("Chart 2"));
    gridLayout->addWidget(chartView, 1, 1);
    m_charts << chartView;

    chartView = new QChartView(createSplineChart("Chart 3"));
    gridLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    chartView = new QChartView(createSplineChart("Chart 4"));
    gridLayout->addWidget(chartView, 2, 1);
    m_charts << chartView;

    //    chartView = new QChartView(createScatterChart());
    //    m_ui->gridLayout->addWidget(chartView, 2, 2);
    //    m_charts << chartView;

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

void ThemeWidget::initLayout()
{
    cbox_legend = new QComboBox();
    cbox_theme = new QComboBox();
    cbox_animation = new QComboBox();
    antialiasCheckBox = new QCheckBox("抗锯齿");
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
    // parametersLayout->addLayout(themeLayout);
    // parametersLayout->addLayout(legendLayout);
    // parametersLayout->addLayout(animationLayout);
    // parametersLayout->addWidget(antialiasCheckBox);
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

    gridLayout = new QGridLayout;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addLayout(gridLayout);
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

QChart* ThemeWidget::createAreaChart() const
{
    QChart* chart = new QChart();
    chart->setTitle("Area chart");

    // The lower series initialized to zero values
    QLineSeries* lowerSeries = 0;
    QString name("Series ");
    int nameIndex = 0;
    for (int i(0); i < m_dataTable.count(); i++)
    {
        QLineSeries* upperSeries = new QLineSeries(chart);
        for (int j(0); j < m_dataTable[i].count(); j++)
        {
            Data data = m_dataTable[i].at(j);
            if (lowerSeries)
            {
                const auto& points = lowerSeries->points();
                upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
            }
            else
            {
                upperSeries->append(QPointF(j, data.first.y()));
            }
        }
        QAreaSeries* area = new QAreaSeries(upperSeries, lowerSeries);
        area->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(area);
        lowerSeries = upperSeries;
    }

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, m_valueCount - 1);
    chart->axes(Qt::Vertical).first()->setRange(0, m_valueMax);
    // Add space to label to add space between labels and axis
    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");

    return chart;
}

QChart* ThemeWidget::createBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart* chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries* series = new QStackedBarSeries(chart);
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
    axisY->setLabelFormat("%.1f  ");

    return chart;
}

QChart* ThemeWidget::createLineChart() const
{

    QChart* chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList& list : m_dataTable)
    {
        QLineSeries* series = new QLineSeries(chart);
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
    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");

    return chart;
}

QChart* ThemeWidget::createPieChart() const
{
    QChart* chart = new QChart();
    chart->setTitle("Pie chart");

    QPieSeries* series = new QPieSeries(chart);
    for (const Data& data : m_dataTable[0])
    {
        QPieSlice* slice = series->append(data.second, data.first.y());
        if (data == m_dataTable[0].first())
        {
            // Show the first slice exploded with label
            slice->setLabelVisible();
            slice->setExploded();
            slice->setExplodeDistanceFactor(0.5);
        }
    }
    series->setPieSize(0.4);
    chart->addSeries(series);

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
    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
    return chart;
}

QChart* ThemeWidget::createScatterChart() const
{
    // scatter chart
    QChart* chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList& list : m_dataTable)
    {
        QScatterSeries* series = new QScatterSeries(chart);
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
    QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
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




//void ThemeWidget::readSerialInfo2logWin()
//{
//    mySerialPort->showSerialPostsInfo();
//}
