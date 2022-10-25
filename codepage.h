#ifndef CODEPAGE_H
#define CODEPAGE_H

#include <QWidget>
#include "QFileSystemModel"
#include "syntax/CodeEditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "FileOperator/FileOperator.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QGridLayout>
#include <QRegExp>
#include <QTabWidget>

namespace Ui
{
    class CodePage;
}

class CodePage : public QWidget
{
    Q_OBJECT

public slots:

    void slot_updateProDir();


public:

    explicit CodePage(QWidget* parent = nullptr);
    ~CodePage();

private slots:

    void on_file_sys_treeview_doubleClicked(const QModelIndex& index);

    void tabClose(int index);

private:

    // varibles
    Ui::CodePage* ui;
    QFileSystemModel* fileSystemModel;
    QList<QSyntaxHighlighter*> highlighters;
    QList<CodeEditor*> codeEditors;
    QTabWidget* codePageTabs;
    int tabCount;

    // fucntions
    void initElements();
    void showFileSystem();
    void updateFileSystem();
    QString readFromFile(QString);
    int newTab(const QString&, const QString&, const QIcon&);

};

#endif // CODEPAGE_H
