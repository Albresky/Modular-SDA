#ifndef CHARTS_H
#define CHARTS_H

#include <QWidget>
#include <QSplineSeries>
#include "utilities.h"
#include <QChart>
#include <QChartView>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

namespace Ui
{
    class Charts;
}

class Charts : public QWidget
{
    Q_OBJECT

public:
    explicit Charts(QWidget* parent = nullptr);
    ~Charts();

private:
    Ui::Charts* ui;

    //function
    void initCharts();
};

#endif // CHARTS_H
