#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextBrowser>
#include <custom/myserialport.h>

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
    explicit ThemeWidget(QWidget* parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void populateThemeBox();
    void populateAnimationBox();
    void populateLegendBox();
    void connectSignals();
    QChart* createAreaChart() const;
    QChart* createBarChart(int valueCount) const;
    QChart* createPieChart() const;
    QChart* createLineChart() const;
    QChart* createSplineChart(QString title);
    QChart* createScatterChart() const;

    QGridLayout* gridLayout;

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    QList<QChartView*> m_charts;
    DataTable m_dataTable;
    QGridLayout* gridlayout;
    QCheckBox* antialiasCheckBox;
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


    // fuction
    void initLayout();
//    void initSerialPortToolBar();
};

#endif /* THEMEWIDGET_H */
