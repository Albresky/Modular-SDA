#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::projectDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(MainWindowTitle);
    setWindowIcon(QIcon(":/res/icons/logo.ico"));

    signalMapper = new QSignalMapper(this);
    valueMap["iw"] = 0;
    valueMap["is"] = 1;
    valueMap["ow"] = 2;
    valueMap["os"] = 3;
    valueMap["dft"] = 4;

    initLayout();

    QObject::connect(action_build, &QAction::triggered, this, &MainWindow::action_build_clicked);
    QObject::connect(action_make_clean, &QAction::triggered, this, &MainWindow::action_make_clean_clicked);
    QObject::connect(action_convert, &QAction::triggered, this, &MainWindow::action_convert_triggerred);
    QObject::connect(this, &MainWindow::transmitProDir, codePage, &CodePage::slot_updateProDir);
    QObject::connect(this, &MainWindow::serialReadDone, this, &MainWindow::serialBuf2Plot);

    qDebug() << "Desktop=>" << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) << "\n"
             << projectDir;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLayout()
{
    // initialize toolBar
    initBuildToolBar();
    initDownloadToolBar();
    initSerialPortToolBar();
    createDesignerToolbars();

    // initialize sideBar
    initSideBar();

    // initialize statusBar
    initStatusBar();

    // initialize actions
    createActions();

    // initialize menuBar
    initMenubar();

    // initialize logWindow
    initLogWindow();

    // initialize QStackedWidget
    initStackedPage();

    // initialize Command Processor
    initQProcess();
}

void MainWindow::initScene()
{
    // initialize DiagramScene
    scene = new DiagramScene(itemMenu, this);
    scene->setMainWindow(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    scene->setBackgroundBrush(QPixmap(":/res/images/background5.png"));
    connect(scene, &DiagramScene::itemInserted,
            this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,
            this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,
            this, &MainWindow::itemSelected);

    view = new DiagramView(scene);
}

void MainWindow::initStackedPage()
{
    /* initialize CodePage */

    qStackedWidget = new QStackedWidget;
    codePage = new CodePage(this);
    qStackedWidget->addWidget(codePage);
    qStackedWidget->setCurrentWidget(codePage);

    /* initialize Charts */
    analyzerPage = new AnalyzerPage(nullptr, this);
    qStackedWidget->addWidget(analyzerPage);

    /* initialize scene */
    initScene();
    analyzerPage->setDiagramScene(scene);

    /* initialize Designer Page */
    designerPage = new DesignerPage(view, scene, itemMenu);
    qStackedWidget->addWidget(designerPage);
    scene->setDesignerpage(designerPage);
    scene->setAnalyzerPage(analyzerPage);

    /* initialize Project Page */
    projectPage = new ProjectPage(nullptr, this);
    qStackedWidget->addWidget(projectPage);

    /* conbine stackedPage with logWindow */
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->setStyleSheet("QSplitter::handle {background-color: rgb(192,192,192);}");
    splitter->setHandleWidth(5);

    splitter->addWidget(qStackedWidget);
    splitter->addWidget(logWindow);
    splitter->widget(1)->hide();

    QHBoxLayout* mainWinLayout = new QHBoxLayout;
    mainWinLayout->addWidget(sidebar);
    mainWinLayout->addWidget(splitter);
    this->centralWidget()->setLayout(mainWinLayout);
}

void MainWindow::initSideBar()
{
    sidebar = new SideBar;

    sidebar_welcome = new QAction();
    sidebar_edit = new QAction();
    sidebar_designer = new QAction();
    sidebar_charts = new QAction();
    sidebar_tool = new QAction();

    // bind signals with toolbar
    QObject::connect(sidebar_welcome, &QAction::triggered, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(sidebar_edit, &QAction::triggered, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(sidebar_designer, &QAction::triggered, this, &MainWindow::showDesignerToolBars);
    QObject::connect(sidebar_charts, &QAction::triggered, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(sidebar_tool, &QAction::triggered, this, &MainWindow::hideDesignerToolBars);

    // bind signals with stackedPage
    QObject::connect(sidebar_welcome, &QAction::triggered, this, &MainWindow::hideDesignerToolBars);
    QObject::connect(sidebar_edit, &QAction::triggered, this, &MainWindow::action_edit_clicked);
    QObject::connect(sidebar_designer, &QAction::triggered, this, &MainWindow::action_designer_clicked);
    QObject::connect(sidebar_charts, &QAction::triggered, this, &MainWindow::action_charts_clicked);
    QObject::connect(sidebar_tool, &QAction::triggered, this, &MainWindow::action_project_clicked);

    sidebar->addAction(sidebar_welcome, "欢迎", QString("welcome"));
    sidebar->addAction(sidebar_edit, "编辑", QString("edit"));
    sidebar->addAction(sidebar_designer, "设计", QString("circuit"));
    sidebar->addAction(sidebar_charts, "观察", QString("charts"));
    sidebar->addAction(sidebar_tool, "项目", QString("tool"));
    sidebar->setFixedWidth(80);
}

void MainWindow::initStatusBar()
{
    // show serial port status
    action_com_state = new QLabel("未连接串口");
    action_com_state->setMargin(2);
    action_com_state->setFixedWidth(75);
    action_com_state->setAlignment(Qt::AlignCenter);
    action_com_state->setStyleSheet("QLabel { background-color : gray; color : darkgray; }");
    ui->statusBar->addPermanentWidget(action_com_state);
}

void MainWindow::executeCmd(QString command)
{
    qProcess->start("cmd.exe", QStringList() << "/C" << command);
    qProcess->waitForFinished();
}

void MainWindow::CmdExit(int exitCode)
{
    QString str = "";
    if (exitCode == QProcess::NormalExit)
    {
        QByteArray qba = qProcess->readAllStandardOutput();

        QTextCodec* pText = QTextCodec::codecForName("System");
        str = pText->toUnicode(qba);
        logWindow->setPlainText(str);
    }
    else
    {
        logWindow->setPlainText(str);
    }
}

void MainWindow::initBuildToolBar()
{
    buildToolBar = addToolBar(tr("Build"));

    action_build = new QAction();
    action_build->setText("构建");
    action_build->setIcon(QIcon(":res/imgs/hammer.png"));
    buildToolBar->addAction(action_build);

    action_make_clean = new QAction();
    action_make_clean->setText("清理");
    action_make_clean->setIcon(QIcon(":/res/imgs/clean.png"));
    buildToolBar->addAction(action_make_clean);

    action_convert = new QAction();
    action_convert->setText("生成");
    action_convert->setIcon(QIcon(":/res/imgs/convert.png"));
    buildToolBar->addAction(action_convert);
}

void MainWindow::initDownloadToolBar()
{
    downloadToolBar = addToolBar(tr("下载"));

    // connect to device
    action_connect = new QAction();
    action_connect->setText("连接");
    action_connect->setIcon(QIcon(":/res/imgs/connect.png"));
    action_connect->setCheckable(true);
    downloadToolBar->addAction(action_connect);
    QObject::connect(action_connect, &QAction::triggered, this, &MainWindow::action_connect_clicked);

    // download to device
    action_download = new QAction();
    action_download->setText("下载");
    action_download->setIcon(QIcon(":/res/imgs/download.png"));
    downloadToolBar->addAction(action_download);
    QObject::connect(action_download, &QAction::triggered, this, &MainWindow::action_download_clicked);
}

void MainWindow::initSerialPortToolBar()
{
    serialPortBar = addToolBar(tr("端口"));

    // show log window
    action_show_logWin = new QAction();
    action_show_logWin->setCheckable(true);
    action_show_logWin->setIcon(QIcon(":/res/imgs/console.png"));
    serialPortBar->addAction(action_show_logWin);
    QObject::connect(action_show_logWin, &QAction::triggered, this, &MainWindow::showLogWindow);

    // connect serial port
    showSerialPortsInfo = new QAction();
    showSerialPortsInfo->setText("显示串口信息");
    showSerialPortsInfo->setIcon(QIcon(":/res/imgs/serialPort.png"));
    showSerialPortsInfo->setCheckable(true);
    QObject::connect(showSerialPortsInfo, &QAction::triggered, this, &MainWindow::openSerialPort);
    serialPortBar->addAction(showSerialPortsInfo);
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

void MainWindow::initQProcess()
{
    qProcess = new QProcess();
    connect(qProcess, &QProcess::finished, this, &MainWindow::CmdExit);
}

void MainWindow::action_build_clicked()
{
    qDebug() << "action BUILD clicked";
    executeCmd(getProjectDirSysDiskPartitionSymbol() + " && cd " + MainWindow::projectDir + " && make");
}

void MainWindow::action_make_clean_clicked()
{
    qDebug() << "action make clean clicked";
    executeCmd(getProjectDirSysDiskPartitionSymbol() + " && cd " + MainWindow::projectDir + " && make clean");
}

void MainWindow::action_convert_triggerred()
{
    qDebug() << "action convert triggerred";
    // executeCmd(getProjectDirSysDiskPartitionSymbol() + " && cd " + MainWindow::projectDir + " && make convert");
    logWindow->append("设计合法，开始生成...");
    QThread::msleep(500);
    logWindow->append("生成成功！");
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
    if (!ManualSelectedDir.isEmpty())
    {
        MainWindow::projectDir = ManualSelectedDir;
        emit transmitProDir();
    }
}

QString MainWindow::getProjectDir()
{
    return MainWindow::projectDir;
}

void MainWindow::action_designer_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(designerPage);
}

void MainWindow::action_edit_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(codePage);
}

void MainWindow::action_charts_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(analyzerPage);
}

void MainWindow::action_project_clicked()
{
    qDebug() << "current page:" << qStackedWidget->currentIndex();
    qStackedWidget->setCurrentWidget(projectPage);
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

    QToolButton* blueGridBgBtn = new QToolButton;
    blueGridBgBtn->setCheckable(false);
    blueGridBgBtn->setChecked(false);
    blueGridBgBtn->setIcon(QIcon(":/res/images/background1.png"));
    connect(blueGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundBlueGridClicked);
    QToolButton* whiteGridBgBtn = new QToolButton;
    whiteGridBgBtn->setCheckable(false);
    whiteGridBgBtn->setChecked(false);
    whiteGridBgBtn->setIcon(QIcon(":/res/images/background2.png"));
    connect(whiteGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundWhiteGridClicked);
    QToolButton* grayGridBgBtn = new QToolButton;
    grayGridBgBtn->setCheckable(false);
    grayGridBgBtn->setChecked(false);
    grayGridBgBtn->setIcon(QIcon(":/res/images/background3.png"));
    connect(grayGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundGrayGridClicked);
    QToolButton* noGridBgBtn = new QToolButton;
    noGridBgBtn->setCheckable(false);
    noGridBgBtn->setChecked(false);
    noGridBgBtn->setIcon(QIcon(":/res/images/background4.png"));
    connect(noGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundNoGridClicked);
    QToolButton* dotGridBgBtn = new QToolButton;
    dotGridBgBtn->setCheckable(false);
    dotGridBgBtn->setChecked(false);
    dotGridBgBtn->setIcon(QIcon(":/res/images/background5.png"));
    connect(dotGridBgBtn, &QToolButton::clicked, this, &MainWindow::backgroundDotGridClicked);

    backgroundToolBar = addToolBar(tr("Set Background"));
    backgroundToolBar->addWidget(blueGridBgBtn);
    backgroundToolBar->addWidget(whiteGridBgBtn);
    backgroundToolBar->addWidget(grayGridBgBtn);
    backgroundToolBar->addWidget(noGridBgBtn);
    backgroundToolBar->addWidget(dotGridBgBtn);
    hideDesignerToolBars();
}

void MainWindow::initLogWindow()
{
    logWindow = new QTextBrowser();
    // logWindow->setMinimumHeight(200);
    logWindow->setStyleSheet(QString("background-color: rgb(51, 51, 51); ") +
                             QString("color:rgb(231, 231, 231); ") +
                             QString("border-radius:4px;"));
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
            scene->removeDiagramItemInMap(qgraphicsitem_cast<DiagramItem*>(item)->diagramType());
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

void MainWindow::backgroundDotGridClicked()
{
    qDebug() << "backgroundDotGridClicked() triggerred.";
    scene->setBackgroundBrush(QPixmap(":/res/images/background5.png"));
    scene->update();
    view->update();
}

void MainWindow::textInserted(QGraphicsTextItem*)
{
    designerPage->unCheckButtonGroupTextItem();
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::showLogWindow()
{
    if (logWindow->isVisible())
    {
        logWindow->hide();
        logWindow->setMinimumHeight(200);
    }
    else
    {
        logWindow->show();
        logWindow->setMinimumHeight(0);
    }
}

void MainWindow::update_action_com_state(QString portName)
{
    action_com_state->setText(portName);
    action_com_state->setStyleSheet("QLabel { background-color : rgb(47, 219, 85); color : black;}");
}

void MainWindow::closeSerialPort()
{
    serialPort->close();
    serialPort = nullptr;
    serialPortOnline = false;
    logWindow->append("已关闭串口");
    action_com_state->setText("未连接串口");
    action_com_state->setStyleSheet("QLabel { background-color : gray; color : darkgray; }");
}

void MainWindow::openSerialPort()
{
    bool isChecked = showSerialPortsInfo->isChecked();
    if (serialPort != nullptr && serialPort->isOpen())
    {
        closeSerialPort();
    }
    if (isChecked)
    {
        serialPort = new QSerialPort();
        serialPort->setPortName(projectPage->getCOM());
        serialPort->setBaudRate(projectPage->getBaudRate());
        serialPort->setDataBits(projectPage->getDataBits());
        serialPort->setParity(projectPage->getParity());
        serialPort->setStopBits(projectPage->getStopBits());
        serialPort->setFlowControl(projectPage->getFlowControl());
        if (serialPort->open(QIODevice::ReadWrite))
        {
            serialPortOnline = true;
            logWindow->append("已打开串口");
            update_action_com_state(serialPort->portName());
            // action_com_state->setText(serialPort->portName());
            // action_com_state->setStyleSheet("QLabel { background-color : rgb(47, 219, 85); color : black;}");
            QObject::connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::ReadPortData);
        }
        else
        {
            logWindow->append("打开串口失败");
        }
    }
}



void MainWindow::getCOMs()
{
    bool flag = true;
    list_qSerialPortInfo.clear();
    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPortInfo* p_info = new QSerialPortInfo(info);
        list_qSerialPortInfo.append(p_info);
        flag = false;
    }
    logWindow->append(flag ? "无串口信息" : "");
    projectPage->setCOMs(list_qSerialPortInfo);
}

QList<QSerialPortInfo*> MainWindow::get_qSerialPortInfo()
{
    return list_qSerialPortInfo;
}

void MainWindow::ReadPortData()
{
    qDebug() << "buf size:" << buf.size();
    buf += serialPort->readAll();
    QString str = "";
    if (!buf.isEmpty())
    {
        str += QSerialPort::tr(buf);
    }
    QStringList formatted = str.split("#");

    int size = formatted.size();
    int index = size >= 2 ? size - 2 : 0;

    if (formatted[index] == "z")
    {
        buf.clear();
        serialBuf.append(formatted);
        logWindow->append("开始读取串口数据...");
        logWindow->append(str);
        qDebug() << str;
        emit serialReadDone();
    }
}

void MainWindow::sendSerialStart()
{
    serialBuf.clear();
    buf.clear();
    char updateSignal = 's';
    serialPort->write(&updateSignal);
}

void MainWindow::serialBuf2Plot()
{
    qDebug() << "serialBuf2Plot()";
    if (serialBuf.size() == 0)
    {
        logWindow->append("串口缓冲区为空");
        return;
    }

    QVector<double> iw_x, iw_y;
    QVector<double> is_x, is_y;
    QVector<double> ow_x, ow_y;
    QVector<double> os_x, os_y;
    double dft_res = -1;
    AnalysisVaule analysisVaule;
    int pointCnt = analyzerPage->getPointCnt();
    int sampleFreq = analyzerPage->getSampleFreq();

    foreach (QString str, serialBuf)
    {
        if (str.contains(":"))
        {
            QStringList seg = str.split("_");
            QString indentifier = seg[0];
            QStringList value = seg[1].split(":");

            double x = value[0].toDouble();
            double y = value[1].toDouble();
            switch (valueMap[indentifier])
            {
                // "iw"
                case 0:
                    iw_x.append(x);
                    iw_y.append(y);
                    break;

                // "is"
                case 1:
                    is_x.append(x);
                    is_y.append(y);
                    break;

                // "ow"
                case 2:
                    ow_x.append(x);
                    ow_y.append(y);
                    break;

                // "os"
                case 3:
                    os_x.append(x);
                    os_y.append(y);
                    break;
                // "dft"
                case 4:
                    dft_res = y;
                default:
                    break;
            }
        }

        else if ((str == "" && analyzerPage->isIngoreEmptyDataChecked()) || analyzerPage->isIngoreInvalidDataChecked())
        {
            continue;
        }
        else
        {
            logWindow->append("串口缓冲区数据格式错误 =>" + str);
            return;
        }
    }

    // transform data
    int iw_size = iw_x.size() + iw_y.size();
    int is_size = is_x.size() + is_y.size();
    int ow_size = ow_x.size() + ow_y.size();
    int os_size = os_x.size() + os_y.size();

    if (iw_size != 0)
    {
        for(int i = 0; i < pointCnt; i++)
        {
            iw_y[i] /= 1000;
        }
    }
    else
    {
        logWindow->append("串口未收到输入波形数据");
    }
    if (is_size != 0)
    {
        is_x[0] *= sampleFreq / pointCnt;
        is_y[0] /= 1000 * pointCnt;
        for(int i = 1; i < pointCnt; i++)
        {
            is_x[i] *= sampleFreq / pointCnt;
            is_y[i] /= 500 * pointCnt;
        }

        /*
        *   [计算输入数据]
        *
        *   基波频率：_x[1]
        *   基波幅值：_y[1]
        *   三次     _x[3]
        *
        */

        analysisVaule.validValue = is_y[0];
        analysisVaule.baseFreq = is_x[1];
        analysisVaule.baseAmp = is_y[1];
        analysisVaule._3rdFreq = is_x[3];
        analysisVaule._3rdAmp = is_y[3];
        analysisVaule._5thFreq = is_x[5];
        analysisVaule._5thAmp = is_y[5];
        analysisVaule._7thFreq = is_x[7];
        analysisVaule._7thAmp = is_y[7];
        analyzerPage->updateAnalyses(analysisVaule);
    }
    else
    {
        logWindow->append("串口未收到输入频谱数据");
    }
    if (ow_size != 0)
    {
        for(int i = 0; i < pointCnt; i++)
        {
            ow_y[i] /= 1000;
        }
    }
    else
    {
        logWindow->append("串口未收到输出波形数据");
    }
    if (os_size != 0)
    {
        os_x[0] *= sampleFreq / pointCnt;
        os_y[0] /= 1000 * pointCnt;
        for(int i = 1; i < pointCnt; i++)
        {
            os_x[i] *= sampleFreq / pointCnt;
            os_y[i] /= 500 * pointCnt;
        }
    }
    else
    {
        logWindow->append("串口未收到输出频谱数据");
    }

    // for (int i = 1; i < pointCnt; i++)
    // {
    //     iw_y[i] /= 1000;
    //     ow_y[i] /= 1000;
    //     is_x[i] *= sampleFreq / pointCnt;
    //     os_x[i] *= sampleFreq / pointCnt;
    //     is_y[i] /= 500 * pointCnt;
    //     os_y[i] /= 500 * pointCnt;
    // }



    if (iw_size)
    {
        analyzerPage->updateChartData(iw_x, iw_y, 0);
    }
    if (is_size)
    {
        analyzerPage->updateChartData(is_x, is_y, 1);
    }
    if (ow_size)
    {
        analyzerPage->updateChartData(ow_x, ow_y, 2);
    }
    if (os_size)
    {
        analyzerPage->updateChartData(os_x, os_y, 3);
    }
    if(dft_res >= 0)
    {
        analyzerPage->updateDFT(dft_res / 1000.0);
    }
    serialBuf.clear();
}


bool MainWindow::isSerialPortOnline()
{
    return serialPortOnline;
}

void MainWindow::showStatusBarMessage(const QString msg)
{
    ui->statusBar->showMessage(msg);
}

void MainWindow::action_connect_clicked()
{
    if(!action_connect->isChecked() && process_connect != nullptr)
    {
        process_connect->close();
        logWindow->append("设备已断开!");
        return;
    }
    process_connect = new QProcess();
    QString command = "C:\\Users\\Albre\\Desktop\\R-IDE\\openocd\\openocd.exe";
    QStringList args;
    args << "-f";
    args << "C:\\Users\\Albre\\Desktop\\R-IDE\\openocd\\tinyriscv.cfg";
    process_connect->start(command, args);
    if(process_connect->waitForStarted())
    {
        QProcess::ProcessState state = process_connect->state();
        if(state == QProcess::NotRunning)
        {
            logWindow->append("设备连接失败!");
            action_connect->setChecked(false);
        }
        else if(state == QProcess::Running)
        {
            logWindow->append(readProcessOutput(process_connect));
            logWindow->append("设备已连接!");
        }
    }

}


void MainWindow::action_download_clicked()
{
    if(process_download == nullptr)
    {
        process_download = new QProcess();
    }
    else if(process_download->state() == QProcess::Running)
    {
        logWindow->append("当前正在下载...");
        delete process_download;
        return;
    }

//    QProcess* telnet = process_download;

    QString telnet = "telnet";
    QStringList args;
    args << "localhost" << "4444";

    process_download->start(telnet, args);
    if(process_download->waitForStarted())
    {
        logWindow->append(readProcessOutput(process_download));
    }

    QProcess::ProcessState state = process_download->state();
    if(state == QProcess::Running)
    {
        logWindow->append("已进入Telnet");
    }
    else
    {
        logWindow->append("进入Telnet失败");
        delete process_download;
        return;
    }


    const char* HALT = "halt \n";
    process_download->write(HALT);
    process_download->waitForReadyRead();
//    logWindow->append(HALT);


//    QString LOAD_IMAGE = "load_image";
//    QString BIN_PATH = " C:/Users/Albre/Desktop/R-IDE/example/SigDesign/main.bin";
//    QString LOAD_IMAGE_args = " bin 0x0 0x1000000\n";

//    QString DOWNLOAD_COMMAND_STRING = QString(LOAD_IMAGE + BIN_PATH + LOAD_IMAGE_args);
//    qDebug() << DOWNLOAD_COMMAND_STRING;

//    const char* DOWNLOAD_COMMAND = DOWNLOAD_COMMAND_STRING.toUtf8().data();
//    qDebug() << DOWNLOAD_COMMAND;

    process_download->write("load_image C:\\Users\\Albre\\Desktop\\R-IDE\\example\\SigDesign\\main.bin 0x0 bin 0x0 0x1000000\n");
    process_download->waitForReadyRead();
    logWindow->append(readProcessOutput(process_download));

    process_download->write("resume 0 \n");
    process_download->waitForReadyRead();
//    logWindow->append("resume 0 \n");

    process_download->write("exit \n");
    process_download->waitForFinished();
    logWindow->append(readProcessOutput(process_download));

}

QString MainWindow::readProcessOutput(QProcess* process)
{
    QByteArray qba = process->readAllStandardOutput();
    QTextCodec* pText = QTextCodec::codecForName("System");
    QString str = pText->toUnicode(qba);
    return str;
}
