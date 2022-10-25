#include "visualize.h"
#include "ui_visualize.h"

Visualize::Visualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visualize)
{
    ui->setupUi(this);
}

Visualize::~Visualize()
{
    delete ui;
}
