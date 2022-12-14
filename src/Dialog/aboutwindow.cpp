#include "aboutwindow.h"


AboutWindow::AboutWindow(QWidget* parent)
    : QWidget{parent}
{

    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    setWindowIcon(QIcon(":/res/icons/logo.ico"));
    setWindowTitle(tr("关于 RiscV-IDE"));
    setFixedSize(350, 200);

    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* top = new QHBoxLayout;

    QFrame* logo = new QFrame();
    logo->setGeometry(QRect(10, 10, 48, 48));
    logo->setStyleSheet("image: url(:/res/icons/logo.ico);");

    QLabel* head = new QLabel("Risc-V IDE");
    head->setFont(QFont("微软雅黑", 16));

    QLabel* subHead = new QLabel("Risc-V 集成开发环境");
    subHead->setFont(QFont("微软雅黑", 8));

    QLabel* version = new QLabel("版本: 0.1.2");
    subHead->setFont(QFont("微软雅黑", 8));

    QLabel* platform = new QLabel("Based on Qt 6.3.0(MinGW, 64 bit)");
    subHead->setFont(QFont("微软雅黑", 8));

    QLabel* buildDate = new QLabel("Built on Nov 11 2022 18:44");
    subHead->setFont(QFont("微软雅黑", 8));


    QFrame* Author = new QFrame();
    QGridLayout* authorLayout = new QGridLayout;

    QLabel* authorTxt = new QLabel("博客:");
    authorTxt->setFixedWidth(100);
    authorTxt->setAlignment(Qt::AlignRight);
    authorTxt->setFont(QFont("微软雅黑", 8));
    authorLink = new QLabel("<a href='url'>https://cosyspark.space</a>");


    QLabel* githubTxt = new QLabel("Github:");
    githubTxt->setFixedWidth(100);
    githubTxt->setAlignment(Qt::AlignRight);
    githubTxt->setFont(QFont("微软雅黑", 8));
    githubLink = new QLabel("<a href='url'>https://github.com/Albresky</a>");

    authorLayout->addWidget(authorTxt, 0, 0);
    authorLayout->addWidget(authorLink, 0, 1);
    authorLayout->addWidget(githubTxt, 1, 0);
    authorLayout->addWidget(githubLink, 1, 1);

    QObject::connect(authorLink, &QLabel::linkActivated, this, &AboutWindow::blog_linkActivated);
    QObject::connect(githubLink, &QLabel::linkActivated, this, &AboutWindow::github_linkActivated);
    Author->setLayout(authorLayout);

    QVBoxLayout* infoLayout = new QVBoxLayout;
    infoLayout->addWidget(head);
    infoLayout->addWidget(subHead);
    infoLayout->addWidget(version);
    infoLayout->addWidget(platform);
    infoLayout->addWidget(buildDate);

    top->addWidget(logo);
    top->addLayout(infoLayout);
    layout->addLayout(top);
    layout->addWidget(Author, Qt::AlignHCenter);
    this->setLayout(layout);
}

void AboutWindow::blog_linkActivated()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://cosyspark.space")));
    qDebug() << "blog hyperLink triggered";
}


void AboutWindow::github_linkActivated()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/Albresky")));
    qDebug() << "github hyperLink triggered";
}
