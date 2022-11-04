#include "designerpage.h"
#include "ui_designerpage.h"



#include <mainwindow.h>



DesignerPage::DesignerPage(QGraphicsView* view, DiagramScene* scene, QMenu* itemMenu, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::DesignerPage),
    view(view),
    scene(scene),
    itemMenu(itemMenu)
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


    initAttributesBox();
    initToolBox();


    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(toolBoxes);
    splitter->addWidget(view);
    splitter->addWidget(attributesBox);

    splitter->setStyleSheet("QSplitter::handle {background-color: rgb(192,192,192);}");
    splitter->setHandleWidth(5);
    this->layout()->replaceWidget(ui->widget, splitter);
    ui->widget->hide();
}

void DesignerPage::initToolBox()
{
    toolBoxName_1 = new QLabel();
    toolBoxName_2 = new QLabel();
    toolBoxName_3 = new QLabel();
    toolBoxName_1->setText("工具箱1");
    // toolBoxName_2->setText("工具箱2");
    // toolBoxName_3->setText("工具箱3");


    buttonGroup_1 = new QButtonGroup(this);
    buttonGroup_2 = new QButtonGroup(this);
    buttonGroup_3 = new QButtonGroup(this);
    buttonGroup_1->setExclusive(false);

    connect(buttonGroup_1, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &DesignerPage::buttonGroupClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(createCellWidget(buttonGroup_1, tr("输入"), DiagramItem::Input));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("输出"), DiagramItem::Output));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("FFT"), DiagramItem::FFT));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("IFFT"), DiagramItem::IFFT));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("DFT"), DiagramItem::DFT));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("汉宁窗"), DiagramItem::HanningWin));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("黑人窗"), DiagramItem::BlackmanWin));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("HT"), DiagramItem::HT));
    layout->addWidget(createCellWidget(buttonGroup_1, tr("Filter"), DiagramItem::Filter));


    QToolButton* textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup_1->addButton(textButton, MainWindow::InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/res/images/textpointer.png")));
    textButton->setIconSize(QSize(32, 32));

    QVBoxLayout* textLayout = new QVBoxLayout;
    textLayout->addWidget(textButton, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("文本")), Qt::AlignCenter);

    QWidget* textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget);

    QWidget* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    toolBox_1 = new QToolBox;
    toolBox_1->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox_1->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox_1->addItem(itemWidget, tr("基础模块"));


    QVBoxLayout* vToolBoxesLayout = new QVBoxLayout;
    vToolBoxesLayout->addWidget(toolBoxName_1);
    vToolBoxesLayout->addWidget(toolBox_1);
    // vToolBoxesLayout->addWidget(toolBoxName_2);
    // vToolBoxesLayout->addWidget(toolBoxName_3);
    toolBoxes = new QFrame();
    toolBoxes->setLayout(vToolBoxesLayout);
    toolBoxes->setFrameShape(QFrame::Shape::Box);
    toolBoxes->setLineWidth(1);
    toolBoxes->setMaximumWidth(150);
}


QWidget* DesignerPage::createCellWidget(QButtonGroup* buttonGroup, const QString& text, DiagramItem::ModuleType type)
{

    DiagramItem item (type, itemMenu);
    QIcon icon(item.image());

    QToolButton* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

void DesignerPage::buttonGroupClicked(QAbstractButton* button)
{
    const QList<QAbstractButton*> buttons = buttonGroup_1->buttons();
    for (QAbstractButton* myButton : buttons)
    {
        if (myButton != button)
        {
            button->setChecked(false);
        }
    }

    const int id = buttonGroup_1->id(button);
    if (id == MainWindow::InsertTextButton)
    {
        scene->setMode(DiagramScene::InsertText);
    }
    else
    {
        // scene->setItemType(DiagramItem::DiagramType(id));
        scene->setItemType(DiagramItem::ModuleType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}


void DesignerPage::unCheckButtonGroupItem(int index)
{
    buttonGroup_1->button(index)->setChecked(false);
}

void DesignerPage::unCheckButtonGroupTextItem()
{
    buttonGroup_1->button(MainWindow::InsertTextButton)->setChecked(false);
}

void DesignerPage::initAttributesBox()
{
    noModuleSelected = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel(tr("当前未选中模块")), Qt::AlignCenter);
    noModuleSelected->setLayout(layout);

    attributesBox = new QStackedWidget();
    attributesBox->setLineWidth(1);
    attributesBox->setFrameShape(QFrame::Shape::Box);
    attributesBox->setMaximumWidth(150);

    attributesBox->addWidget(noModuleSelected);
}

int DesignerPage::getSamplePointCnt()
{
    return 512;
//    return qSpinBox_pointCnt->value();
}

int DesignerPage::getSampleFreq()
{
    return 10e6;
//    return qSpinBox_SampleFreq->value();
}

void DesignerPage::addAttributesBox(QWidget* widget)
{
    qDebug() << "addAttributesBox()";
    attributesBox->addWidget(widget);
}

void DesignerPage::updateAttributesBox(QWidget* widget)
{
    qDebug() << "updateAttributesBox()";
    attributesBox->setCurrentWidget(widget);
}

void DesignerPage::resetAttributesBox()
{
    qDebug() << "resetAttributesBox()";
    attributesBox->setCurrentWidget(noModuleSelected);
}
