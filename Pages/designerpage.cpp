#include "designerpage.h"
#include "ui_designerpage.h"

#include <QSplitter>
#include <QThread>



DesignerPage::DesignerPage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::DesignerPage)
{
    ui->setupUi(this);

    init();

}

DesignerPage::~DesignerPage()
{
    delete ui;
}

QWidget* DesignerPage::getInstance()
{
    return designBoard;
}


void DesignerPage::init()
{
    QThread::msleep(200);

    // init Layout
    toolBoxName_0 = ui->label;
    toolBoxName_1 = ui->label_2;
    toolBoxName_2 = ui->label_3;
    toolBox_0 = ui->toolBox;
    toolBox_1 = ui->toolBox_2;
    toolBox_2 = ui->toolBox_3;


    toolBoxName_0->setText("工具箱 1");
    toolBoxName_1->setText("工具箱 2");
    toolBoxName_2->setText("工具箱 3");

    for(int j = 0; j < 3; j++)
    {
        QListWidget* q = nullptr;
        switch(j)
        {
            case 0:
                q = toolBox_0;
                break;
            case 1:
                q = toolBox_1;
                break;
            case 2:
                q = toolBox_2;
                break;
        }
        q->setStyleSheet("background-color:#f0f0f0");
        for(int i = 0; i < 6; i++)
        {
            QListWidgetItem* item = new QListWidgetItem();
            item->setIcon(QIcon(":/res/imgs/tool.png"));
            item->setText("Tool " + QString::number(i));
//            item->set
            q->addItem(item);
        }
    }

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(toolBoxName_0);
    vBoxLayout->addWidget(toolBox_0);
    vBoxLayout->addWidget(toolBoxName_1);
    vBoxLayout->addWidget(toolBox_1);
    vBoxLayout->addWidget(toolBoxName_2);
    vBoxLayout->addWidget(toolBox_2);

    toolBoxes = new QWidget();
    toolBoxes->setLayout(vBoxLayout);

    designBoard = new QWidget();
    designBoard->setStyleSheet("background-color:#FFFFFF;");


    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(toolBoxes);
    splitter->addWidget(designBoard);
//    splitter->setStyleSheet("QSplitter::handle{border:solid black 2px}");
//    splitter->setStyleSheet("background-color: #008787;");
//    splitter->setStyleSheet("QSplitter:handle{background-color: #000000;border: solid 1px;}");
    this->layout()->replaceWidget(ui->widget, splitter);
    ui->widget->hide();
}


