#ifndef ANALYZERPAGE_H
#define ANALYZERPAGE_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSplineSeries>
#include <QTextBrowser>
#include "lib/XCustomPlot.h"
#include "mainwindow.h"
#include "custom/DataItem.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_AnalyzerPageForm;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_USE_NAMESPACE

class AnalyzerPage: public QWidget
{
    Q_OBJECT
public:
    explicit AnalyzerPage(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
    ~AnalyzerPage();

    void updateChartData(QVector<double> pointsDataX, QVector<double> pointsDataY, int index);
    void updateAnalyses(AnalysisVaule analysisValue);
    bool isIngoreEmptyDataChecked();
    bool isIngoreInvalidDataChecked();
    void setDftVisibility(bool visiable);
    void set_FFT_IFFT_HT_Visibility(bool visiable);
    void setDiagramScene(DiagramScene* scene);
    int getPointCnt();
    int getSampleFreq();
    int getCalcuFreq();

private slots:
    void btn_clear_plot_clicked();
    void updateUI();
    void loadParameters();

signals:
    void updatePlot();

private:
    MainWindow* mainWindow;
    DiagramScene* scene;
    QPen pen;
    QList<XCustomPlot*> XPlots;
    QList<QChartView*> m_charts;
    QChartView* spline_chartView;
    QGridLayout* chartsLayout;
    QHBoxLayout* inputLayout;
    QHBoxLayout* outputLayout;
    QPushButton* btn_updatePlot;
    QPushButton* btn_clearPlot;
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
    QLineEdit* browser_dftResult;
    QWidget* dftResult_widget;
    QWidget* paramsBox;
    QFrame* commonAnalysesResultsFrame;
    int pointCnt;
    int sampleFreq;
    int calcuFreq;
    QVector<double> default_xdata, default_ydata;

    // fuction
    void initLayout();
    void initCharts();
    void initGridChart(QGridLayout* layout, int row, int column);
    void initSpectrumChart(QGridLayout* layout, int row, int column);
    void initDefaultData();
    void populateThemeBox();
    void populateAnimationBox();
    void populateLegendBox();
    void connectSignals();

};

#endif /* ANALYZERPAGE_H */
