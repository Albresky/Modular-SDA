#include "charts.h"
#include "ui_charts.h"

Charts::Charts(QWidget* parent) :
    QWidget(parent)
{
    ui->setupUi(this);

    initCharts();
}

Charts::~Charts()
{
    delete ui;
}


void Charts::initCharts()
{
    constexpr qreal marker_size = 20.;

    QSplineSeries* series = new QSplineSeries();
    series->append({QPointF(0., 0.),
                    QPointF(0.5, 2.27),
                    QPointF(1.5, 2.2),
                    QPointF(3.3, 1.7),
                    QPointF(4.23, 3.1),
                    QPointF(5.3, 2.3),
                    QPointF(6.47, 4.1)
                   });
    series->setMarkerSize(marker_size);
    series->setLightMarker(Utilities::redRectangle(marker_size));
    series->setSelectedLightMarker(Utilities::blueTriangle(marker_size));
    QObject::connect(series, &QXYSeries::clicked, series, [&](const QPointF & point)
    {
        int index = series->points().indexOf(point);
        if (index != -1)
            series->toggleSelection({index});
    });

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    //    chart->setAxisX()
    chart->legend()->setVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget* controlWidget = new QWidget(this);
    QGridLayout* controlLayout = new QGridLayout(this);

    QComboBox* charPointCombobox = new QComboBox();
    QComboBox* charPointSelectedCombobox = new QComboBox();
    QComboBox* lineColorCombobox = new QComboBox();
    QCheckBox* showUnselectedPointsCheckbox = new QCheckBox();

    QLabel* charPoint = new QLabel(QCoreApplication::tr("Char point: "));
    charPointCombobox->addItems({QCoreApplication::tr("Red rectangle"),
                                 QCoreApplication::tr("Green triangle"),
                                 QCoreApplication::tr("Orange circle")
                                });
    QObject::connect(charPointCombobox, &QComboBox::currentIndexChanged, series, [&](const int index)
    {
        if (showUnselectedPointsCheckbox->isChecked())
        {
            series->setLightMarker(Utilities::getPointRepresentation(Utilities::PointType(index), marker_size));
        }
    });

    QLabel* charPointSelected = new QLabel(QCoreApplication::tr("Char point selected: "));
    charPointSelectedCombobox->addItems({QCoreApplication::tr("Blue triangle"),
                                         QCoreApplication::tr("Yellow rectangle"),
                                         QCoreApplication::tr("Lavender circle")
                                        });
    QObject::connect(charPointSelectedCombobox, &QComboBox::currentIndexChanged, series, [&](const int index)
    {
        series->setSelectedLightMarker(Utilities::getSelectedPointRepresentation(Utilities::SelectedPointType(index), marker_size));
    });

    QLabel* lineColorLabel = new QLabel(QCoreApplication::tr("Line color: "));
    lineColorCombobox->addItems({QCoreApplication::tr("Blue"),
                                 QCoreApplication::tr("Black"),
                                 QCoreApplication::tr("Mint")
                                });
    QObject::connect(lineColorCombobox, &QComboBox::currentIndexChanged, series, [&](const int index)
    {
        series->setColor(Utilities::makeLineColor(Utilities::LineColor(index)));
    });

    QLabel* showUnselectedPointsLabel = new QLabel(QCoreApplication::tr("Display unselected points: "));
    showUnselectedPointsCheckbox->setChecked(true);
    QObject::connect(showUnselectedPointsCheckbox, &QCheckBox::stateChanged, series, [&](const int state)
    {
        if (state)
        {
            series->setLightMarker(Utilities::getPointRepresentation(Utilities::PointType(charPointCombobox->currentIndex()), marker_size));
        }
        else
        {
            series->setLightMarker(QImage());
        }
    });

    controlLayout->addWidget(charPoint, 0, 0);
    controlLayout->addWidget(charPointCombobox, 0, 1);

    controlLayout->addWidget(charPointSelected, 1, 0);
    controlLayout->addWidget(charPointSelectedCombobox, 1, 1);

    controlLayout->addWidget(lineColorLabel, 2, 0);
    controlLayout->addWidget(lineColorCombobox, 2, 1);

    controlLayout->addWidget(showUnselectedPointsLabel, 3, 0);
    controlLayout->addWidget(showUnselectedPointsCheckbox, 3, 1, 1, 2);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(chartView);
    mainLayout->addWidget(controlWidget);
    setLayout(mainLayout);
}
