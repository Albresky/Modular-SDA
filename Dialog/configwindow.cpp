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

    make_groupbox = new QGroupBox(tr("Make工具路径"));
    make_path_edit = new QLineEdit();
    QString make_path = getDirFromPATH("MinGW");
    if(make_path.isEmpty())
    {
        make_path_edit->setPlaceholderText("make.exe所在路径");
    }
    else
    {
        make_path_edit->setText(make_path);
    }
    make_path_button = new QToolButton();
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

    toolchain_groupbox = new QGroupBox(tr("GCC工具链路径"));
    toolchain_path_edit = new QLineEdit();
    QString toolchain_path = getDirFromPATH("riscv");
    if(toolchain_path.isEmpty())
    {
        toolchain_path_edit->setPlaceholderText("riscv-gcc工具链所在路径");
    }
    else
    {
        toolchain_path_edit->setText(make_path);
    }
    toolchain_path_button = new QToolButton();
    QObject::connect(toolchain_path_button, &QToolButton::clicked, this, &ConfigWindow::setToolChainPath);

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

    openOCD_path_groupbox = new QGroupBox(tr("OpenOCD路径"));
    openOCD_path_edit = new QLineEdit();
    openOCD_path_button = new QToolButton();
    QObject::connect(toolchain_path_button, &QToolButton::clicked, this, &ConfigWindow::setOpenOCDPath);

    QString openOCD_path = getDirFromPATH("openocd");
    if(openOCD_path.isEmpty())
    {
        openOCD_path_edit->setText("openocd/openocd.exe");
    }
    else
    {
        openOCD_path_edit->setText(make_path + "/openocd.exe");
    }

    QHBoxLayout* openOCD_path_layout = new QHBoxLayout;
    openOCD_path_layout->addWidget(openOCD_path_edit);
    openOCD_path_layout->addWidget(openOCD_path_button);
    openOCD_path_groupbox->setLayout(openOCD_path_layout);

    openOCD_config_groupbox = new QGroupBox("OpenOCD配置文件");
    openOCD_config_edit = new QLineEdit();
    openOCD_config_button = new QToolButton();
    QObject::connect(openOCD_path_button, &QToolButton::clicked, this, &ConfigWindow::setOpenOCDConfigPath);

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

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    layout->addWidget(dialogBtnBox);
    this->setLayout(layout);
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
    make_path_edit->setText(openExplorer());
}

void ConfigWindow::setToolChainPath()
{
    toolchain_path_edit->setText(openExplorer());
}

void ConfigWindow::setOpenOCDPath()
{
    openOCD_path_edit->setText(openExplorer());
}

void ConfigWindow::setOpenOCDConfigPath()
{
    openOCD_config_edit->setText(openExplorer());
}

QString ConfigWindow::openExplorer()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("选择文件夹"),  QDir::currentPath());
    return dir;
}
