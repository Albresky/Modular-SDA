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
#include <QFontComboBox>
#include <QButtonGroup>
#include <QIcon>
#include <QSignalMapper>
#include <QStackedWidget>
#include <QSplitter>
#include <QListView>
#include <QGraphicsView>
#include <QMessageBox>

#include "Diagram/diagramview.h"
#include "Pages/designerpage.h"
#include "syntax/Codeeditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "syntax/typedef.h"
#include "custom/cTabButton.h"
#include "codepage.h"
#include "codepage.h"
#include "themewidget.h"
#include "Diagram/diagramscene.h"
#include "Diagram/arrow.h"



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
    static const int InsertTextButton = 10;


signals:

    void transmitProDir();


private slots:

//    void switchPage(int);
    void action_build_clicked();
    void action_make_clean_clicked();
    void action_designer_clicked();
    void action_edit_clicked();
    void action_charts_clicked();
    void on_open_file_project_dir_triggered();
    void currentFontChanged(const QFont& font);
    void fontSizeChanged(const QString&);
    void handleFontChange();
    void pointerGroupClicked();
    void backgroundBlueGridClicked();
    void backgroundGrayGridClicked();
    void backgroundWhiteGridClicked();
    void backgroundNoGridClicked();
    void sceneScaleChanged(const QString& scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textInserted(QGraphicsTextItem* item);
    void itemInserted(DiagramItem* item);
    void itemSelected(QGraphicsItem* item);
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void bringToFront();
    void sendToBack();
    void deleteItem();
    void about();
    void hideDesignerToolBars();
    void showDesignerToolBars();

public slots:


private:

    /* variables */
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
    QAction* sidebar_welcome;
    QAction* sidebar_edit;
    QAction* sidebar_designer;
    QAction* sidebar_charts;
    QAction* sidebar_tool;
    CodePage* codePage;
    ThemeWidget* qCharts;
    DesignerPage* designerPage;
    QSignalMapper* signalMapper;

    DiagramView* view = nullptr;
    DiagramScene* scene = nullptr;
    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* toolMenu;
    QMenu* aboutMenu;

    QToolBar* textToolBar;
    QToolBar* editToolBar;
    QToolBar* colorToolBar;
    QToolBar* pointerToolbar;
    QToolBar* backgroundToolBar;
    QToolButton* fontColorToolButton;
    QToolButton* fillColorToolButton;
    QToolButton* lineColorToolButton;
    QButtonGroup* buttonGroup_1;
    QButtonGroup* pointerTypeGroup;
    QButtonGroup* backgroundButtonGroup;
    QAction* exitAction;
    QAction* addAction;
    QAction* deleteAction;
    QAction* toFrontAction;
    QAction* sendBackAction;
    QAction* aboutAction;
    QAction* boldAction;
    QAction* underlineAction;
    QAction* italicAction;
    QAction* textAction;
    QAction* fillAction;
    QAction* lineAction;


    QComboBox* sceneScaleCombo;
    QComboBox* itemColorCombo;
    QComboBox* textColorCombo;
    QComboBox* fontSizeCombo;
    QFontComboBox* fontCombo;

    // function
    QString executeCMD(QString command);
    void initToolBar();
    void updateFileSystem();
    void updateWindowTitle();
    QString getProjectDirSysDiskPartitionSymbol();
    void initStatusBar();
    void initSideBar();
    void initLayout();
    void initMenubar();
    void initScene();
    void createActions();
    void createDesignerToolbars();
    QMenu* createColorMenu(const char* slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString& image, QColor color);
    QIcon createColorIcon(QColor color);
    QWidget* createBackgroundCellWidget(const QString& text, const QString& image);

};
#endif // MAINWINDOW_H
