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
#include <QTextBrowser>

#include "custom/commonHeaders.h"
#include "Diagram/diagramview.h"
#include "Pages/designerpage.h"
#include "Pages/analyzerpage.h"
#include "Pages/codepage.h"
#include "Pages/projectpage.h"
#include "syntax/Codeeditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "syntax/typedef.h"
#include "custom/sidebar.h"
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

    QList<QSerialPortInfo*> get_qSerialPortInfo();

signals:

    void transmitProDir();
    void serialReadDone();

private slots:

    void action_build_clicked();
    void action_make_clean_clicked();
    void action_convert_triggerred();
    void action_designer_clicked();
    void action_edit_clicked();
    void action_charts_clicked();
    void action_project_clicked();
    void on_open_file_project_dir_triggered();
    void currentFontChanged(const QFont& font);
    void fontSizeChanged(const QString&);
    void handleFontChange();
    void pointerGroupClicked();
    void backgroundBlueGridClicked();
    void backgroundGrayGridClicked();
    void backgroundWhiteGridClicked();
    void backgroundNoGridClicked();
    void backgroundDotGridClicked();
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
    void showLogWindow();
    void ReadPortData();
    void openSerialPort();
    void serialBuf2Plot();

public slots:

    void getCOMs();
    void sendSerialStart();

private:

    /* variables */
    Ui::MainWindow* ui;
    QPlainTextEdit* cmd_readOut;
    QPushButton* btn_execute;
    QPushButton* btn_show_log_window;
    QProcess* process;
    QLineEdit* console;
    QString output;
    static QString projectDir;
    CodeEditor* configEditor;
    MyHighLighter* highlighter;
    const QString MainWindowTitle = "RiscV-IDE";
    QStackedWidget* qStackedWidget;
    QTextBrowser* logWindow;
    QAction* action_build;
    QAction* action_make_clean;
    QAction* action_download;
    QAction* action_convert;
    QAction* sidebar_welcome;
    QAction* sidebar_edit;
    QAction* sidebar_designer;
    QAction* sidebar_charts;
    QAction* sidebar_tool;
    QAction* showSerialPortsInfo;
    QLabel* action_com_state;
    SideBar* sidebar;
    CodePage* codePage;
    AnalyzerPage* chartsPage;
    DesignerPage* designerPage;
    ProjectPage* projectPage;
    QSignalMapper* signalMapper;
    QProcess* qProcess;
    QSerialPort* serialPort = nullptr;
    QList<QSerialPortInfo*> list_qSerialPortInfo;
    QList<QString> serialBuf;
    QByteArray buf;
    QMap<QString, int> valueMap;


    DiagramView* view = nullptr;
    DiagramScene* scene = nullptr;
    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* toolMenu;
    QMenu* aboutMenu;

    QToolBar* buildToolBar;
    QToolBar* serialPortBar;
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

    void initBuildToolBar();
    void initSerialPortToolBar();
    void updateFileSystem();
    void updateWindowTitle();
    void initSerialPort();
    void initStackedPage();
    void initStatusBar();
    void initSideBar();
    void initLayout();
    void initMenubar();
    void initScene();
    void initLogWindow();
    void initQProcess();
    void createActions();
    void createDesignerToolbars();
    void closeSerialPort();


    void executeCmd(QString command);
    void CmdExit(int exitCode);
    QString getProjectDirSysDiskPartitionSymbol();
    QIcon createColorToolButtonIcon(const QString& image, QColor color);
    QIcon createColorIcon(QColor color);
    QMenu* createColorMenu(const char* slot, QColor defaultColor);
    QWidget* createBackgroundCellWidget(const QString& text, const QString& image);

};
#endif // MAINWINDOW_H
