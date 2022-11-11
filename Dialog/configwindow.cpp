#include "configwindow.h"



ConfigWindow::ConfigWindow(QWidget* parent)
    : QWidget{parent}
{
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    setWindowIcon(QIcon(":/res/icons/logo.ico"));
    setWindowTitle(tr("配置"));
    setFixedSize(400, 300);

    init();
}


void ConfigWindow::init()
{
    tabWidget = new QTabWidget();

    /* create tabs */

    // tab for Make
    make_widget = new QWidget();

    make_groupbox = new QGroupBox(tr("Make工具"));
    make_path_edit = new QLineEdit();
    make_path_button = new QToolButton();
    make_path_edit->setPlaceholderText("make工具可执行文件路径");
    QObject::connect(make_path_button, &QToolButton::clicked, this, &ConfigWindow::setMakePath);

    QHBoxLayout* make_groupbox_layout = new QHBoxLayout;
    make_groupbox_layout->addWidget(make_path_edit);
    make_groupbox_layout->addWidget(make_path_button);
    make_groupbox->setLayout(make_groupbox_layout);

    QVBoxLayout* make_layout = new QVBoxLayout;
    make_layout->addWidget(make_groupbox);
    make_widget->setLayout(make_layout);
    tabWidget->addTab(make_widget, tr("Make"));

    // tab for GCC toolchain
    toolchain_widget = new QWidget();

    toolchain_groupbox = new QGroupBox(tr("GCC工具链"));
    toolchain_path_edit = new QLineEdit();
    toolchain_path_button = new QToolButton();
    toolchain_path_edit->setPlaceholderText("GCC工具链可执行文件路径");
    QObject::connect(toolchain_path_button, &QToolButton::clicked, this, &ConfigWindow::setToolChainDir);

    QHBoxLayout* toolchain_groupbox_layout = new QHBoxLayout;
    toolchain_groupbox_layout->addWidget(toolchain_path_edit);
    toolchain_groupbox_layout->addWidget(toolchain_path_button);
    toolchain_groupbox->setLayout(toolchain_groupbox_layout);

    QVBoxLayout* toolchain_widget_layout = new QVBoxLayout;
    toolchain_widget_layout->addWidget(toolchain_groupbox);
    toolchain_widget->setLayout(toolchain_widget_layout);
    tabWidget->addTab(toolchain_widget, tr("工具链"));

    // tab for OpenOCD
    openOCD_widget = new QWidget();

    openOCD_path_groupbox = new QGroupBox(tr("OpenOCD调试器"));
    openOCD_path_edit = new QLineEdit();
    openOCD_path_button = new QToolButton();
    openOCD_path_edit->setPlaceholderText("OpenOCD可执行文件路径");
    QObject::connect(openOCD_path_button, &QToolButton::clicked, this, &ConfigWindow::setOpenOCDPath);

    QHBoxLayout* openOCD_path_layout = new QHBoxLayout;
    openOCD_path_layout->addWidget(openOCD_path_edit);
    openOCD_path_layout->addWidget(openOCD_path_button);
    openOCD_path_groupbox->setLayout(openOCD_path_layout);

    openOCD_config_groupbox = new QGroupBox("OpenOCD调试器配置");
    openOCD_config_edit = new QLineEdit();
    openOCD_config_button = new QToolButton();
    openOCD_config_edit->setPlaceholderText("OpenOCD配置文件路径");
    QObject::connect(openOCD_config_button, &QToolButton::clicked, this, &ConfigWindow::setOpenOCDConfigPath);

    QHBoxLayout* openOCD_config_layout = new QHBoxLayout;
    openOCD_config_layout->addWidget(openOCD_config_edit);
    openOCD_config_layout->addWidget(openOCD_config_button);
    openOCD_config_groupbox->setLayout(openOCD_config_layout);

    QVBoxLayout* openOCD_layout = new QVBoxLayout;
    openOCD_layout->addWidget(openOCD_path_groupbox);
    openOCD_layout->addWidget(openOCD_config_groupbox);

    openOCD_widget->setLayout(openOCD_layout);
    tabWidget->addTab(openOCD_widget, tr("OpenOCD"));

    dialogBtnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    btn_ok = dialogBtnBox->button(QDialogButtonBox::Ok);
    btn_cancel = dialogBtnBox->button(QDialogButtonBox::Cancel);
    btn_ok->setText(tr("确定"));
    btn_cancel->setText(tr("取消"));
    QObject::connect(btn_ok, &QPushButton::clicked, this, &ConfigWindow::btn_ok_clicked);
    QObject::connect(btn_cancel, &QPushButton::clicked, this, &ConfigWindow::btn_cancel_clicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    layout->addWidget(dialogBtnBox);
    this->setLayout(layout);

    // load config file
    configINI = new Config("config.ini");
    configs["nodename"] = "Settings";
    configs["make"] = configINI->Get("Settings", "make").toString();
    configs["Toolchain"] = configINI->Get("Settings", "Toolchain").toString();
    configs["OpenOcdExecutable"] = configINI->Get("Settings", "OpenOcdExecutable").toString();
    configs["OpenOcdConfig"] = configINI->Get("Settings", "OpenOcdConfig").toString();

    if(configs["make"] != "")
    {
        make_path_edit->setText(configs["make"]);
    }
    if(configs["Toolchain"] != "")
    {
        toolchain_path_edit->setText(configs["Toolchain"]);
    }
    if(configs["OpenOcdExecutable"] != "")
    {
        openOCD_path_edit->setText(configs["OpenOcdExecutable"]);
    }
    if(configs["OpenOcdConfig"] != "")
    {
        openOCD_config_edit->setText(configs["OpenOcdConfig"]);
    }
}


QString ConfigWindow::getDirFromPATH(QString type)
{
    QString path = QProcessEnvironment::systemEnvironment().value("PATH");
    QStringList pathList = path.split(";");
    for (int i = 0; i < pathList.size(); i++)
    {
        QString path = pathList.at(i);
        if (path.contains(type, Qt::CaseInsensitive))
        {
            return path;
        }
    }
    return "";
}

void ConfigWindow::setMakePath()
{
    QString dir = openExplorerForFile("make工具可执行文件");
    if(!dir.isEmpty())
    {
        make_path_edit->setText(dir);
    }

}

void ConfigWindow::setToolChainDir()
{
    QString dir = openExplorerForDir();
    if(!dir.isEmpty())
    {
        toolchain_path_edit->setText(dir);
    }
}

void ConfigWindow::setOpenOCDPath()
{
    QString path = openExplorerForFile("OpenOCD调试器可执行文件");
    if(!path.isEmpty())
    {
        openOCD_path_edit->setText(path);
    }
}

void ConfigWindow::setOpenOCDConfigPath()
{
    QString path = openExplorerForFile("OpenOCD调试器配置文件");
    if(!path.isEmpty())
    {
        openOCD_config_edit->setText(path);
    }
}

QString ConfigWindow::openExplorerForDir()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("选择文件夹"),  QDir::currentPath());
    return dir;
}

QString ConfigWindow::openExplorerForFile(QString type)
{
    QString path = QFileDialog::getOpenFileName(nullptr, type);
    return path;
}

void ConfigWindow::saveChanges()
{
    // ["nodename"] has been initialized in Constructor
    configs["make"] = make_path_edit->text();
    configs["Toolchain"] = make_path_edit->text();
    configs["OpenOcdExecutable"] = openOCD_path_edit->text();
    configs["OpenOcdConfig"] = openOCD_config_edit->text();
    configINI->loadConfig(configs);
}

void ConfigWindow::btn_ok_clicked()
{
    saveChanges();
    emit changesSaved();
    this->close();
}

void ConfigWindow::btn_cancel_clicked()
{
    this->close();
}
