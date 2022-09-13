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
#include "qsplitter.h"
#include "syntax/Codeeditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "syntax/typedef.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void  action_build_clicked();

    void  action_make_clean_clicked();

    void on_open_file_project_dir_triggered();

private:

    Ui::MainWindow *ui;
    QPlainTextEdit *cmd_readOut;
    QPushButton *btn_execute;
    QProcess *process;
    QLineEdit *console;
    QString output;
    QString projectDir;
    QFileSystemModel *fileSystemModel;
    CodeEditor *configEditor;

    const QString WindowTitle="R IDE";

    QAction *action_build;
    QAction *action_make_clean;

    void readOutput();
    QString executeCMD(QString command);
    void showFileSystem();
    void initToolBar();
    void updateFileSystem();
    void updateWindowTitle();
    QString getProjectDirSysDiskPartitionSymbol();
};
#endif // MAINWINDOW_H
