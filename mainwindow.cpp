#include "mainwindow.h"



QString MainWindow::projectDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initLayout();

    signalMapper = new QSignalMapper(this);
    valueMap["iw"] = 0;
    valueMap["is"] = 1;
    valueMap["ow"] = 2;
    valueMap["os"] = 3;
    valueMap["dft"] = 4;

    QObject::connect(action_build, &QAction::triggered, this, &MainWindow::action_build_clicked);
    QObject::connect(action_make_clean, &QAction::triggered, this, &MainWindow::action_make_clean_clicked);
    QObject::connect(action_convert, &QAction::triggered, this, &MainWindow::action_convert_triggerred);
    QObject::connect(this, &MainWindow::transmitProDir, codePage, &CodePage::slot_updateProDir);
    QObject::connect(this, &MainWindow::serialReadDone, this, &MainWindow::serialBuf2Plot);

    qDebug() << "Desktop=>" << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) << "\n"
             << projectDir;
}

void MainWindow::initLayout()
{
    // [0] initialze window
    centralwidget = new QWidget;
    centralwidget->setMinimumWidth(800);
    centralwidget->setMinimumHeight(400);
    setCentralWidget(centralwidget);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setWindowTitle(MainWindowTitle);
    setWindowIcon(QIcon(":/res/icons/logo.ico"));

    // [1] initialize actions
    initActions();

    // initialize toolBar
    initBuildToolBar();
    initDownloadToolBar();
    initSerialPortToolBar();
    createDesignerToolbars();

    // initialize sideBar
    initSideBar();

    // initialize statusBar
    initStatusBar();

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
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    this->setStatusBar(statusBar);


    // [1] show OpenOCD service status
    openOCD_state = new QLabel("OpenOCD offline");
    openOCD_state->setMargin(2);
    openOCD_state->setFixedWidth(105);
    openOCD_state->setAlignment(Qt::AlignCenter);
    openOCD_state->setStyleSheet("QLabel { background-color : gray; color : darkgray; }");
    statusBar->addPermanentWidget(openOCD_state);

    // [2] show serial port status
    action_com_state = new QLabel("未连接串口");
    action_com_state->setMargin(2);
    action_com_state->setFixedWidth(75);
    action_com_state->setAlignment(Qt::AlignCenter);
    action_com_state->setStyleSheet("QLabel { background-color : gray; color : darkgray; }");
    statusBar->addPermanentWidget(action_com_state);
}

void MainWindow::executeCmd(QString command)
{
    qProcess->start("cmd.exe", QStringList() << "/C" << command);
//    qProcess->waitForFinished();
}

void MainWindow::CmdExit(int exitCode)
{
    QString str = "";
    if (exitCode == QProcess::NormalExit)
    {
        QByteArray qba = qProcess->readAllStandardOutput();
        QTextCodec* pText = QTextCodec::codecForName("System");
        str = pText->toUnicode(qba);
    }
    logWindow->append(str);
}

void MainWindow::openOcdConnectedOutput()
{
    QByteArray qba = process_connect->readAllStandardOutput();
    QTextCodec* pText = QTextCodec::codecForName("System");
    QString str = pText->toUnicode(qba);
    logWindow->append(str);
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

    // halt or resume device
    action_halt_resume = new QAction();
    action_halt_resume->setIcon(QIcon(":/res/imgs/halt.png"));
    action_halt_resume->setText("继续");
    action_halt_resume->setCheckable(true);
    action_halt_resume->setDisabled(true);
    downloadToolBar->addAction(action_halt_resume);
    QObject::connect(action_halt_resume, &QAction::triggered, this, &MainWindow::action_halt_resume_clicked);

    // download to device
    action_download = new QAction();
    action_download->setText("下载");
    action_download->setIcon(QIcon(":/res/imgs/download.png"));
    action_download->setDisabled(true);
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
    /* Initialize MenuBar */
    menubar = menuBar();

    /* Initialize MenuBar in Level-1 */
    fileMenu = menubar->addMenu(tr("文件"));
    editMenu = menubar->addMenu(tr("编辑"));
    toolMenu = menubar->addMenu(tr("工具"));
    aboutMenu = menubar->addMenu(tr("帮助"));

    /* Initialize FileMenu in Level-2 */
    action_open_project_dir = new QAction(fileMenu);
    action_open_project_dir->setText("打开文件或项目路径");
    fileMenu->addAction(action_open_project_dir);
    QObject::connect(action_open_project_dir, &QAction::triggered, this, &MainWindow::action_open_file_project_dir_triggered);

    exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("退出Rsic-V IDE"));
    fileMenu->addAction(exitAction);
    QObject::connect(exitAction, &QAction::triggered, this, &QWidget::close);

    /* Initialize EditMenu in Level-2 */
    action_encoding = new QMenu(tr("编码"), editMenu);
    editMenu->addMenu(action_encoding);
    editMenu->addSeparator();

    /* Initialize ToolMenu in Level-2 */

    /* Initialize itemMenu in Designer right click */
    itemMenu = new QMenu(tr("模块"), editMenu);
    itemMenu->addAction(deleteAction);
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    /* Initialize AboutMenu in Level-2 */
    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setStatusTip(tr("关于Rsic-V IDE"));
    aboutMenu->addAction(aboutAction);
    QObject::connect(aboutAction, &QAction::triggered, this, &MainWindow::action_about_triggered);


}

void MainWindow::initActions()
{

    deleteAction = new QAction(QIcon(":/res/images/delete.png"), tr("&删除"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("从设计页删除模块"));
    QObject::connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

    toFrontAction = new QAction(QIcon(":/res/images/bringtofront.png"), tr("置顶"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("将模块移到顶层"));
    QObject::connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);

    sendBackAction = new QAction(QIcon(":/res/images/sendtoback.png"), tr("置底"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("将模块移到底层"));
    QObject::connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

    boldAction = new QAction(tr("粗体"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/res/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    QObject::connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    italicAction = new QAction(QIcon(":/res/images/italic.png"), tr("斜体"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    QObject::connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    underlineAction = new QAction(QIcon(":/res/images/underline.png"), tr("下划线"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    QObject::connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);
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
    // ToDo
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

void MainWindow::action_open_file_project_dir_triggered()
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


void MainWindow::createDesignerToolbars()
{
    editToolBar = addToolBar(tr("编辑"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

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

    textToolBar = addToolBar(tr("字体"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("颜色"));
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
    logWindow->setStyleSheet(QString("background-color: rgb(51, 51, 51); ") +
                             QString("color:rgb(231, 231, 231); ") +
                             QString("border-radius:4px;"));

    metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
}

void MainWindow::hideDesignerToolBars()
{
    textToolBar->hide();
    editToolBar->hide();
    colorToolBar->hide();
    pointerToolbar->hide();
    backgroundToolBar->hide();
}

void MainWindow::showDesignerToolBars()
{
    textToolBar->show();
    editToolBar->show();
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
    names << tr("黑色") << tr("白色") << tr("红色") << tr("蓝色")
          << tr("黄色");

    QMenu* colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i)
    {
        QAction* action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        QObject::connect(action, SIGNAL(triggered()), this, slot);
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

void MainWindow::action_about_triggered()
{
    About* about_window = new About();
    about_window->show();
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


void MainWindow::switch_openOCD_state()
{
    if(!openOCD_online)
    {
        openOCD_state->setText("OpenOCD Offline");
        openOCD_state->setStyleSheet("QLabel { background-color : gray; color : darkgray; }");
    }
    else
    {
        openOCD_state->setText("OpenOCD Online");
        openOCD_state->setStyleSheet("QLabel { background-color : rgb(1, 135, 250); color : black;}");
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

void MainWindow::telnetError(QAbstractSocket::SocketError error)
{
    logWindow->append(QString("telnet>") + metaEnum.valueToKey(error));
}

bool MainWindow::isSerialPortOnline()
{
    return serialPortOnline;
}

void MainWindow::showStatusBarMessage(const QString msg)
{
    statusBar->showMessage(msg);
}

void MainWindow::update_telnet_functions()
{
    if(action_download->isEnabled())
    {
        action_halt_resume->setDisabled(true);
        action_download->setDisabled(true);
    }
    else if(openOCD_online)
    {
        action_halt_resume->setEnabled(true);
        action_download->setEnabled(true);
    }

}

void MainWindow::action_connect_clicked()
{
    if(telnet == nullptr)
    {
        telnet = new QTelnet(this);
        QObject::connect(telnet, &QTelnet::sockConnected, this, &MainWindow::connectEstablished);
        QObject::connect(telnet, &QTelnet::connectionError, this, &MainWindow::telnetError);
        QObject::connect(telnet, &QTelnet::message, this, &MainWindow::message2logWindow);
    }
    action_connect->setChecked(false);
    if(openOCD_online && process_connect != nullptr)
    {
        process_connect->close();
        telnet->disconnectTelnet();
        logWindow->append("OpenOCD已离线!");
        action_connect->setChecked(false);
        openOCD_online = false;
        switch_openOCD_state();
        update_telnet_functions();
        return;
    }
    delete process_connect;
    process_connect = new QProcess();
    QObject::connect(process_connect, &QProcess::readyReadStandardOutput, this, &MainWindow::openOcdConnectedOutput);
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
            openOCD_online = false;
            switch_openOCD_state();
            update_telnet_functions();
        }
        else if(state == QProcess::Running)
        {
            logWindow->append(readProcessOutput(process_connect));
            logWindow->append("设备已连接!");

            openOCD_online = true;
            switch_openOCD_state();

            // establish telnet connection to OpenOCD
            telnet->connectToHost("127.0.0.1", 4444);
        }
    }

}

void MainWindow::action_download_clicked()
{
    telnet->sendData("load_image C:/Users/Albre/Desktop/R-IDE/example/SigDesign/main.bin 0x0 bin 0x0 0x1000000 \r\n");
}

QString MainWindow::readProcessOutput(QProcess* process)
{
    QByteArray qba = process->readAllStandardOutput();
    QTextCodec* pText = QTextCodec::codecForName("System");
    QString str = pText->toUnicode(qba);
    return str;
}

void MainWindow::action_halt_resume_clicked()
{
    action_halt_resume->setChecked(false);
    if(action_connect->isChecked())
    {
        if(action_is_halt)
        {
            telnet->sendData("resume 0 \r\n");
            action_is_halt = false;
            action_halt_resume->setIcon(QIcon(":/res/imgs/resume.png"));
            action_halt_resume->setText("暂停");
            action_halt_resume->setChecked(true);
        }
        else
        {
            telnet->sendData("halt \r\n");
            action_is_halt = true;
            action_halt_resume->setIcon(QIcon(":/res/imgs/halt.png"));
            action_halt_resume->setText("继续");
            action_halt_resume->setChecked(true);
        }
    }
    else
    {
        qDebug() << "未连接设备！";
        logWindow->append("未连接设备!");
    }
}

void MainWindow::connectEstablished()
{
    qDebug() << "OpenOCD and telnet connected!";
    action_connect->setChecked(true);
    logWindow->append(QString("telnet>Connect established.\n"));
    update_telnet_functions();
}

QString MainWindow::strFilter(const QString& str)
{
    // filter redundant symbol from OpenOCD
    QString _str = QString(str);

    QRegularExpression ascii_eignt = QRegularExpression("[\010]+");
    if(_str.contains('\010'))
    {
        _str.replace(ascii_eignt, "");
    }
    if(_str.contains('\r'))
    {
        _str.replace('\r', "");
    }
    if(_str.contains('>'))
    {
        _str.replace('>', "");
    }

    // if str is all space or empty
    QString _temp_str = _str;
    _temp_str = _temp_str.simplified();
    if(_temp_str == "")
    {
        return QString("");
    }
    return _str;
}

void MainWindow::message2logWindow(const QString& msg)
{
    QString str = strFilter(msg);
    qDebug() << str;
    if(str == "")
    {
        return;
    }
    logWindow->insertPlainText(str);
}
