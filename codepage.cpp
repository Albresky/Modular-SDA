#include "codepage.h"
#include "ui_codepage.h"
#include "mainwindow.h"

CodePage::CodePage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::CodePage)
{
    ui->setupUi(this);

    initElements();

}

CodePage::~CodePage()
{
    delete ui;
}


void CodePage::initElements()
{

    //自定义比例
//    QSplitter *spliter = new QSplitter(Qt::Horizontal,this);
//    spliter->addWidget(ui->file_sys_treeview);
//    spliter->addWidget(ui->tabWidget);

    //file system
    showFileSystem();
    updateFileSystem();

    // initialize codePageTabWidget
    ui->tabWidget->setTabsClosable(true);
    codePageTabs = ui->tabWidget;
    this->tabCount = -1;

    QString tempFilePath = QDir::tempPath() + "r_ide_temp_001";
    createFile(tempFilePath);
    newTab("New", tempFilePath, QIcon(":/res/imgs/unknownfile.png"));

    connect(codePageTabs->tabBar(), &QTabBar::tabCloseRequested, codePageTabs->tabBar(), &QTabBar::removeTab);
}


void CodePage::showFileSystem()
{
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath(QDir::currentPath());
    ui->file_sys_treeview->setModel(fileSystemModel);
    ui->file_sys_treeview->setColumnHidden(1, true);
    ui->file_sys_treeview->setColumnHidden(2, true);
    ui->file_sys_treeview->setColumnHidden(3, true);
}



void CodePage::updateFileSystem()
{
    qDebug() << MainWindow::getProjectDir();
    QString projectDir = MainWindow::getProjectDir();
    fileSystemModel->setRootPath(projectDir);
    ui->file_sys_treeview->setModel(fileSystemModel);
    ui->file_sys_treeview->setRootIndex(fileSystemModel->index(projectDir));
}


void CodePage::slot_updateProDir()
{
    updateFileSystem();
}

void CodePage::on_file_sys_treeview_doubleClicked(const QModelIndex& index)
{
    // obtain file info
    QString fileName = fileSystemModel->fileName(index);
    QString filePath = fileSystemModel->filePath(index);
    QIcon fileIcon = fileSystemModel->fileIcon(index);
    if(isFile(filePath))
    {
        newTab(fileName, filePath, fileIcon);
    }
}

QString CodePage::readFromFile(QString filePath)
{

    QFile data(filePath);
    QString content = "";
    if (data.open(QFile::ReadOnly))
    {
        QTextStream s(&data);
        content = s.readAll();
//        qDebug() << "content:" << content;
    }
    data.close();

    // replace TAB with 4 space
//    content.replace(QRegularExpression("[\t]+"), "    ");
    return content;
}


int CodePage::newTab(const QString& fileName, const QString& filePath, const QIcon& fileIcon)
{
    QWidget* newTabWidget = new QWidget();

    // initialize default codeEditor
    CodeEditor* codeEditor = new CodeEditor(this);
    codeEditor->setMode(EDIT);
    codeEditor->setPlainText(readFromFile(filePath));
    QGridLayout* qGridLayout = new QGridLayout(this);
    qGridLayout->addWidget(codeEditor);
    MyHighLighter* highlighter = new MyHighLighter(codeEditor->document());
    newTabWidget->setLayout(qGridLayout);

    codeEditors.append(codeEditor);
    highlighters.append(highlighter);
    codePageTabs->addTab(newTabWidget, fileIcon, fileName);
    this->tabCount++;
    codePageTabs->setCurrentIndex(tabCount);

    return tabCount;
}

//void CodePage::tabClose(const QModelIndex& index)
//{
//    codeEditors.
//}
