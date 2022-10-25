#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtCore5Compat/QTextCodec>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QProcess>
#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QCommandLineParser>
#include <QTreeView>
#include <QToolBar>
#include <QAction>
#include <QStandardPaths>
#include <QDebug>
#include <QSignalMapper>
#include <Pages/designerpage.h>

#include "qsplitter.h"
#include "syntax/CodeEditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "syntax/typedef.h"
#include "QListView"
#include "custom/cTabButton.h"
#include "QStackedWidget"
#include "codepage.h"
#include "codepage.h"
#include "themewidget.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

    static QString getProjectDir();

signals:

    void transmitProDir();


private slots:

    void action_build_clicked();

    void action_make_clean_clicked();

    void on_open_file_project_dir_triggered();

//    void switchPage(int);

    void on_bnt_code_clicked();

    void on_btn_charts_clicked();

    void on_btn_design_clicked();

private:

    // variable
    Ui::MainWindow* ui;
    QPlainTextEdit* cmd_readOut;
    QPushButton* btn_execute;
    QProcess* process;
    QLineEdit* console;
    QString output;
    static QString projectDir;
    CodeEditor* configEditor;
    QListView* sidebar;
    MyHighLighter* highlighter;
    const QString MainWindowTitle = "RiscV-IDE";
    QStackedWidget* qStackedWidget;
    QAction* action_build;
    QAction* action_make_clean;
    CodePage* codePage;
    ThemeWidget* qCharts;
    DesignerPage* designerPage;
    QSignalMapper* signalMapper;

    // function
    QString executeCMD(QString command);
    void initToolBar();
    void updateFileSystem();
    void updateWindowTitle();
    QString getProjectDirSysDiskPartitionSymbol();
    void initStatusBar();
    void initSideBar();
    void initLayout();
};
#endif // MAINWINDOW_H
