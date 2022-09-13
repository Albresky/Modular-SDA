#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("R-IDE");
    setWindowIcon(QIcon(":/res/icons/logo.ico"));

    showFileSystem();
    initToolBar();

    // editor
    configEditor = new CodeEditor();
    configEditor->setMode(EDIT);
    ui->gridLayout_editor->addWidget(configEditor);
    MyHighLighter *highlighter = new MyHighLighter(configEditor->document());


    QObject::connect(action_build,&QAction::triggered,this,&MainWindow::action_build_clicked);
    QObject::connect(action_make_clean,&QAction::triggered,this,&MainWindow::action_make_clean_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showFileSystem()
{
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath(QDir::currentPath());
    ui->file_sys_treeview->setModel(fileSystemModel);
    ui->file_sys_treeview->setColumnHidden(1,true);
    ui->file_sys_treeview->setColumnHidden(2,true);
    ui->file_sys_treeview->setColumnHidden(3,true);
}

void MainWindow::updateFileSystem()
{
    fileSystemModel->setRootPath(projectDir);
    ui->file_sys_treeview->setModel(fileSystemModel);
    ui->file_sys_treeview->setRootIndex(fileSystemModel->index(projectDir));
}

QString MainWindow::executeCMD(QString command){
    QProcess qp;
    qp.start("cmd.exe",QStringList()<<"/C"<<command);
    qp.waitForFinished();
    QByteArray qba=qp.readAllStandardOutput();
    QTextCodec *pText  =QTextCodec::codecForName("System");
    QString str=pText->toUnicode(qba);
    return str;
}


void MainWindow:: initToolBar()
{

    action_build= new QAction();
    action_build->setText("构建");
    action_build->setIcon(QIcon(":res/icons/build.ico"));
    ui->toolBar->addAction(action_build);

    action_make_clean=new QAction();
    action_make_clean->setText("清理");
    action_make_clean->setIcon(QIcon(":/res/icons/clean.ico"));
    ui->toolBar->addAction(action_make_clean);
}


void MainWindow:: action_build_clicked()
{
    qDebug()<<"action BUILD clicked";
//    QString command=ui->cmd_edit->text();
    QString res=executeCMD(getProjectDirSysDiskPartitionSymbol()+" && cd "+projectDir+" && make");
    qDebug()<<res;
    ui->cmd_readOut->setPlainText(res);
}

void  MainWindow::action_make_clean_clicked()
{
    qDebug()<<"action make clean clicked";

    QString res=executeCMD(getProjectDirSysDiskPartitionSymbol()+" && cd "+projectDir+" && make clean");
    qDebug()<<res;
    ui->cmd_readOut->setPlainText(res);
}


QString MainWindow::getProjectDirSysDiskPartitionSymbol()
{
    QString sysPartitionSymbol="";
    QStringList dirList=projectDir.split('\\');
    sysPartitionSymbol=dirList[0];
    return sysPartitionSymbol;
}

void MainWindow::on_open_file_project_dir_triggered()
{
    QString ManualSelectDir =
        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("Save Path"), QDir::currentPath()));
    if(!ManualSelectDir.isEmpty())
    {
        projectDir= ManualSelectDir;
        updateFileSystem();
    }
}

