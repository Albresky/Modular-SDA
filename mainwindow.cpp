#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QSignalMapper>

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

    qDebug() << "Desktop=>" << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) << "\n" << projectDir;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLayout()
{

    // initialize QStackedWidget
    /* initialize CodePage */
    qStackedWidget = ui->stackedWidget;
    codePage = new CodePage(this);
    ui->stackedWidget->addWidget(codePage);
    ui->stackedWidget->setCurrentWidget(codePage);

    /* initialize Charts */
    qCharts = new ThemeWidget();
    ui->stackedWidget->addWidget(qCharts);
//    ui->stackedWidget->setCurrentWidget(qCharts);

    /* initialize Designer Page */
    designerPage = new DesignerPage();
    ui->stackedWidget->addWidget(designerPage);

    // initialize toolbar
    initToolBar();

    // initialize sidebar buttons
    initSideBar();

    // initialize statusBar
    initStatusBar();
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


void MainWindow::on_bnt_code_clicked()
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

