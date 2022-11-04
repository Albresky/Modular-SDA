#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSplineSeries>
#include <QTextBrowser>
#include "lib/XCustomPlot.h"
#include "mainwindow.h"
#include "custom/commonHeaders.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
    ~ThemeWidget();

    void updateChartData(QVector<double> pointsDataX, QVector<double> pointsDataY, int index);
    void updateAnalyses(AnalysisVaule analysisValue);
    bool isIngoreEmptyDataChecked();
    bool isIngoreInvalidDataChecked();
    void updateAxisXMax(int value);
    void updateAxisYMax(int value);

private Q_SLOTS:
    void updateUI();


private:
    MainWindow* mainWindow;

    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    int axisXMax;
    int axisYMax;
    QPen pen;
    QList<XCustomPlot*> XPlots;
    QList<QChartView*> m_charts;
    QChartView* spline_chartView;
    DataTable m_dataTable;
    QVBoxLayout* chartsLayout;
    QHBoxLayout* inputLayout;
    QHBoxLayout* outputLayout;
    QPushButton* btn_updatePlot;
    QCheckBox* antialiasCheckBox;
    QCheckBox* ingoreEmptyDataCheckBox;
    QCheckBox* ingoreInvalidDataCheckBox;
    QComboBox* cbox_legend;
    QComboBox* cbox_theme;
    QComboBox* cbox_animation;
    QLineEdit* browser_validValue;
    QLineEdit* browser_baseFreq;
    QLineEdit* browser_baseAmplitude;
    QLineEdit* browser_thirdOrderFreq;
    QLineEdit* browser_thirdOrderAmplitude;
    QLineEdit* browser_fifthOrderFreq;
    QLineEdit* browser_fifthOrderAmplitude;
    QLineEdit* browser_seventhOrderFreq;
    QLineEdit* browser_seventhOrderAmplitude;

    QVector<QPointF> spline_data;
    QSplineSeries* splineSeries;


    // fuction
    void initLayout();
    void initCharts();
    void initGridChart(QLayout* layout);
    void initSpectrumChart(QLayout* layout);
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void populateThemeBox();
    void populateAnimationBox();
    void populateLegendBox();
    void connectSignals();
    QChart* createBarChart(int valueCount) const;
    QChart* createSplineChart(QString title);


//    void initSerialPortToolBar();
};

#endif /* THEMEWIDGET_H */
