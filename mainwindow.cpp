#include "mainwindow.h"
#include "ui_mainwindow.h"



QString MainWindow::projectDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(MainWindowTitle);
    setWindowIcon(QIcon(":/res/icons/logo.ico"));

    signalMapper = new QSignalMapper(this);

    initLayout();

    QObject::connect(action_build, &QAction::triggered, this, &MainWindow::action_build_clicked);
    QObject::connect(action_make_clean, &QAction::triggered, this, &MainWindow::action_make_clean_clicked);
    QObject::connect(this, &MainWindow::transmitProDir, codePage, &CodePage::slot_updateProDir);
    QObject::connect(ui->btn_design, &QPushButton::clicked, this, &MainWindow::showDesignerToolBars);
    QObject::connect(ui->btn_code, &QPushButton::clicked, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(ui->btn_charts, &QPushButton::clicked, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(ui->btn_visualize, &QPushButton::clicked, this, &MainWindow::hideDesignerToolBars);

    qDebug() << "Desktop=>" << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) << "\n" << projectDir;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLayout()
{

    // initialize toolbar
    initToolBar();

    // initialize sidebar buttons
    initSideBar();

    // initialize statusBar
    initStatusBar();

    // initialize actions
    createActions();

    // initialize menuBar
    initMenubar();

    // initialize QStackedWidget

    /* initialize CodePage */
    qStackedWidget = ui->stackedWidget;
    codePage = new CodePage(this);
    ui->stackedWidget->addWidget(codePage);
    ui->stackedWidget->setCurrentWidget(codePage);

    /* initialize Charts */
    qCharts = new ThemeWidget();
    ui->stackedWidget->addWidget(qCharts);

    /* initialize scene */
    initScene();

    /* initialize Designer Page */
    designerPage = new DesignerPage(view, scene, itemMenu);
    ui->stackedWidget->addWidget(designerPage);



    // backgroundButtonGroup = new QButtonGroup(this);

}


void MainWindow::initScene()
{
    // initialize DiagramScene
    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    scene->setBackgroundBrush(QPixmap(":/res/images/background1.png"));
    connect(scene, &DiagramScene::itemInserted,
            this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,
            this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,
            this, &MainWindow::itemSelected);

    view = new QGraphicsView(scene);
}

void MainWindow::initSideBar()
{
//    QStringList btn_bgImages;
//    QStringList btn_names;
//    btn_bgImages << ":res/sidebar/code.png" << ":res/sidebar/pencil.png" << ":res/sidebar/visualize.png" << ":res/sidebar/graph.png";
//    btn_names << "编辑" << "设计" << "可视化" << "图表";

//    for(int i = 0; i < btn_bgImages.size(); i++)
//    {
//        QPushButton* ctBtn = new QPushButton(btn_names.at(i));
////        cTabButton* ctBtn = new cTabButton(btn_bgImages.at(i));
////        ctBtn->setText(btn_names.at(i));
//        signalMapper->setMapping(ctBtn, i);
//        QObject::connect(ctBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
//        ui->sidebar->addWidget(ctBtn, 0, Qt::AlignTop);
//    }
//    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(switchPage(int)));

}

void MainWindow::initStatusBar()
{
    // log window
    QPushButton* btn_show_log_window = new QPushButton();
    btn_show_log_window->setText("编译输出");
    ui->statusBar->addWidget(btn_show_log_window);
}



QString MainWindow::executeCMD(QString command)
{
    QProcess qp;
    qp.start("cmd.exe", QStringList() << "/C" << command);
    qp.waitForFinished();
    QByteArray qba = qp.readAllStandardOutput();
    QTextCodec* pText  = QTextCodec::codecForName("System");
    QString str = pText->toUnicode(qba);
    return str;
}


void MainWindow:: initToolBar()
{
    action_build = new QAction();
    action_build->setText("构建");
    action_build->setIcon(QIcon(":res/icons/build.ico"));
    ui->toolBar->addAction(action_build);

    action_make_clean = new QAction();
    action_make_clean->setText("清理");
    action_make_clean->setIcon(QIcon(":/res/icons/clean.ico"));
    ui->toolBar->addAction(action_make_clean);

    createDesignerToolbars();
}


void MainWindow::initMenubar()
{
//    fileMenu = menuBar()->addMenu(tr("文件"));
//    fileMenu->addAction()

    itemMenu = ui->Edit;
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
}

void MainWindow:: action_build_clicked()
{
    qDebug() << "action BUILD clicked";
    //    QString command=ui->cmd_edit->text();
    QString res = executeCMD(getProjectDirSysDiskPartitionSymbol() + " && cd " + MainWindow::projectDir + " && make");
    qDebug() << res;
//    ui->cmd_readOut->setPlainText(res);
}

void  MainWindow::action_make_clean_clicked()
{
    qDebug() << "action make clean clicked";
    QString res = executeCMD(getProjectDirSysDiskPartitionSymbol() + " && cd " + MainWindow::projectDir + " && make clean");
    qDebug() << res;
//    ui->cmd_readOut->setPlainText(res);
}


QString MainWindow::getProjectDirSysDiskPartitionSymbol()
{
    QString sysPartitionSymbol = "";
    QStringList dirList = projectDir.split('\\');
    sysPartitionSymbol = dirList[0];
    return sysPartitionSymbol;
}


void MainWindow::on_open_file_project_dir_triggered()
{
    QString ManualSelectedDir =
        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Save Path"), QDir::currentPath()));
    if(!ManualSelectedDir.isEmpty())
    {
        MainWindow::projectDir = ManualSelectedDir;
        emit transmitProDir();
    }
}


QString MainWindow::getProjectDir()
{
    return MainWindow::projectDir;
}

//void MainWindow::switchPage(int index)
//{
//    qDebug() << "switchPage() triggered=" << index;
//    switch(index)
//    {
//        case 0:
//            qStackedWidget->setCurrentWidget(codePage);
//            break;
//        case 1:
//            break;
//        case 2:
//            break;
//        case 3:
//            qStackedWidget->setCurrentWidget(qCharts);
//            break;
//        default:
//            break;
//    }
//}


void MainWindow::on_btn_code_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(codePage);

}


void MainWindow::on_btn_charts_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(qCharts);
}


void MainWindow::on_btn_design_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(designerPage);
}


void MainWindow::itemInserted(DiagramItem* item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
//    buttonGroup_1->button(int(item->diagramType()))->setChecked(false);
    designerPage->unCheckButtonGroupItem(int(item->diagramType()));
}

void MainWindow::itemSelected(QGraphicsItem* item)
{
    DiagramTextItem* textItem =
        qgraphicsitem_cast<DiagramTextItem*>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/res/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);


    sendBackAction = new QAction(QIcon(":/res/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

    deleteAction = new QAction(QIcon(":/res/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

//    exitAction = new QAction(tr("E&xit"), this);
//    exitAction->setShortcuts(QKeySequence::Quit);
//    exitAction->setStatusTip(tr("Quit Scenediagram example"));
//    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/res/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    italicAction = new QAction(QIcon(":/res/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    underlineAction = new QAction(QIcon(":/res/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}


void MainWindow::createDesignerToolbars()
{
//    editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(deleteAction);
//    editToolBar->addAction(toFrontAction);
//    editToolBar->addAction(sendBackAction);



    fontCombo = new QFontComboBox();
    connect(fontCombo, &QFontComboBox::currentFontChanged,
            this, &MainWindow::currentFontChanged);

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
    {
        fontSizeCombo->addItem(QString().setNum(i));
    }
    QIntValidator* validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::fontSizeChanged);

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/res/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::textButtonTriggered);


    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/res/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::fillButtonTriggered);


    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/res/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::lineButtonTriggered);


    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
//    textToolBar->addAction(boldAction);
//    textToolBar->addAction(italicAction);
//    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton* pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/res/images/pointer.png"));
    QToolButton* linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/res/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);


    QToolButton* blueGridBgBtn  = new QToolButton;
    blueGridBgBtn->setCheckable(false);
    blueGridBgBtn->setChecked(false);
    blueGridBgBtn->setIcon(QIcon(":/res/images/background1.png"));
    connect(blueGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundBlueGridClicked);
    QToolButton* whiteGridBgBtn  = new QToolButton;
    whiteGridBgBtn->setCheckable(false);
    whiteGridBgBtn->setChecked(false);
    whiteGridBgBtn->setIcon(QIcon(":/res/images/background2.png"));
    connect(whiteGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundWhiteGridClicked);
    QToolButton* grayGridBgBtn  = new QToolButton;
    grayGridBgBtn->setCheckable(false);
    grayGridBgBtn->setChecked(false);
    grayGridBgBtn->setIcon(QIcon(":/res/images/background3.png"));
    connect(grayGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundGrayGridClicked);
    QToolButton* noGridBgBtn  = new QToolButton;
    noGridBgBtn->setCheckable(false);
    noGridBgBtn->setChecked(false);
    noGridBgBtn->setIcon(QIcon(":/res/images/background4.png"));
    connect(noGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundNoGridClicked);


    backgroundToolBar = addToolBar(tr("Set Background"));
    backgroundToolBar->addWidget(blueGridBgBtn);
    backgroundToolBar->addWidget(whiteGridBgBtn);
    backgroundToolBar->addWidget(grayGridBgBtn);
    backgroundToolBar->addWidget(noGridBgBtn);

    hideDesignerToolBars();
}


void MainWindow::hideDesignerToolBars()
{
    textToolBar->hide();
//    editToolBar->hide();
    colorToolBar->hide();
    pointerToolbar->hide();
    backgroundToolBar->hide();
}


void MainWindow::showDesignerToolBars()
{
    textToolBar->show();
//    editToolBar->show();
    colorToolBar->show();
    pointerToolbar->show();
    backgroundToolBar->show();
}

void MainWindow::currentFontChanged(const QFont&)
{
    handleFontChange();
}

void MainWindow::fontSizeChanged(const QString&)
{
    handleFontChange();
}

void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction*>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/res/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}

void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction*>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/res/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction*>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/res/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}

void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}

QMenu* MainWindow::createColorMenu(const char* slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu* colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i)
    {
        QAction* action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
        {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString& imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

void MainWindow::pointerGroupClicked()
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::sceneScaleChanged(const QString& scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}

void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About RiscV-IDE"),
                       tr("The <b>RiscV-IDE</b> a demo "
                          "for FPGA."));
}

void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem* item : overlapItems)
    {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
        {
            zValue = item->zValue() + 0.1;
        }
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
    {
        return;
    }

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem* item : overlapItems)
    {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
        {
            zValue = item->zValue() - 0.1;
        }
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::deleteItem()
{
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    for (QGraphicsItem* item : qAsConst(selectedItems))
    {
        if (item->type() == Arrow::Type)
        {
            scene->removeItem(item);
            Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    selectedItems = scene->selectedItems();
    for (QGraphicsItem* item : qAsConst(selectedItems))
    {
        if (item->type() == DiagramItem::Type)
        {
            qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
        }
        scene->removeItem(item);
        delete item;
    }
}


void MainWindow::backgroundBlueGridClicked()
{
    qDebug() << "backgroundBlueGridClicked() triggerred.";
    scene->setBackgroundBrush(QPixmap(":/res/images/background1.png"));
    scene->update();
    view->update();
}

void MainWindow::backgroundWhiteGridClicked()
{
    qDebug() << "backgroundWhiteGridClicked() triggerred.";
    scene->setBackgroundBrush(QPixmap(":/res/images/background2.png"));
    scene->update();
    view->update();
}

void MainWindow::backgroundGrayGridClicked()
{
    qDebug() << "backgroundGrayGridClicked() triggerred.";
    scene->setBackgroundBrush(QPixmap(":/res/images/background3.png"));
    scene->update();
    view->update();
}

void MainWindow::backgroundNoGridClicked()
{
    qDebug() << "backgroundNoGridClicked() triggerred.";
    scene->setBackgroundBrush(QPixmap(":/res/images/background4.png"));
    scene->update();
    view->update();
}


void MainWindow::textInserted(QGraphicsTextItem*)
{
    designerPage->unCheckButtonGroupTextItem();
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
