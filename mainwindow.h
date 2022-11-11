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
#include <QRegularExpression>
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
#include "custom/sidebar.h"
#include "lib/qtelnet.h"
#include "Pages/designerpage.h"
#include "Pages/analyzerpage.h"
#include "Pages/codepage.h"
#include "Pages/projectpage.h"
#include "syntax/Codeeditor.h"
#include "syntax/SyntaxHighlighter.h"
#include "syntax/typedef.h"
#include "Diagram/diagramview.h"
#include "Diagram/diagramscene.h"
#include "Diagram/arrow.h"
#include "Dialog/aboutwindow.h"
#include "custom/config.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    static QString getProjectDir();
    static const int InsertTextButton = 10;

    void showStatusBarMessage(const QString msg);
    bool isSerialPortOnline();

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
    void action_connect_clicked();
    void action_halt_resume_clicked();
    void action_download_clicked();
    void action_open_file_project_dir_triggered();
    void action_about_triggered();
    void action_config_triggered();
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
    void hideDesignerToolBars();
    void showDesignerToolBars();
    void showLogWindow();
    void ReadPortData();
    void openSerialPort();
    void serialBuf2Plot();
    void telnetError(QAbstractSocket::SocketError error);
    void connectEstablished();
    void openOcdConnectedOutput();
    void message2logWindow(const QString& msg);
    void loadConfig();

public slots:

    void getCOMs();
    void sendSerialStart();

private:

    /* variables */
    QWidget* centralwidget;

    QPlainTextEdit* cmd_readOut;
    QPushButton* btn_execute;

    static QString projectDir;


    QLineEdit* console;
    QString output;
    CodeEditor* configEditor;
    MyHighLighter* highlighter;
    const QString MainWindowTitle = "RiscV-IDE";
    QStackedWidget* qStackedWidget;
    QTextBrowser* logWindow;
    QAction* action_build;
    QAction* action_make_clean;
    QAction* action_connect;
    QAction* action_halt_resume;
    QAction* action_download;
    QAction* action_convert;
    QAction* sidebar_welcome;
    QAction* sidebar_edit;
    QAction* sidebar_designer;
    QAction* sidebar_charts;
    QAction* sidebar_tool;
    QAction* showSerialPortsInfo;

    SideBar* sidebar;
    CodePage* codePage;
    AnalyzerPage* analyzerPage;
    DesignerPage* designerPage;
    ProjectPage* projectPage;
    QSignalMapper* signalMapper;
    QSerialPort* serialPort = nullptr;

    QMetaEnum metaEnum;

    QList<QSerialPortInfo*> list_qSerialPortInfo;
    QList<QString> serialBuf;
    QByteArray buf;
    QMap<QString, int> valueMap;

    bool serialPortOnline = false;
    bool action_is_halt = false;
    bool openOCD_online = false;

    QProcess* qProcess;
    QProcess* process_connect = nullptr;
    QProcess* process_download = nullptr;
    QTelnet* telnet = nullptr;
    MyThread* thread_telnet = nullptr;

    DiagramView* view = nullptr;
    DiagramScene* scene = nullptr;

    /* MenuBar */
    QMenuBar* menubar;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* action_encoding; // submenu of editMenu
    QMenu* itemMenu; // submenu of editMenu
    QMenu* toolMenu;
    QAction* action_config; //submenu of toolMenu
    QMenu* aboutMenu;
    QAction* action_open_project_dir;
    QAction* action_exit;



    QToolBar* buildToolBar;
    QToolBar* downloadToolBar;
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

    QAction* action_show_logWin;
    QLabel* action_com_state;
    QLabel* openOCD_state;
    QStatusBar* statusBar;
    Config* configINI = nullptr;

    // Config Variables
    QMap<QString, QString> configs;

    // function

    void initBuildToolBar();
    void initDownloadToolBar();
    void initSerialPortToolBar();
    void updateFileSystem();
    void updateWindowTitle();
    void initSerialPort();
    void initStackedPage();
    void initStatusBar();
    void initSideBar();
    void initLayout();
    void initActions();
    void initMenubar();
    void initScene();
    void initLogWindow();
    void initQProcess();
    void createDesignerToolbars();
    void closeSerialPort();

    void switch_openOCD_state();
    void update_action_com_state(QString portName);
    void update_telnet_functions();
    void updateLogWindowCursor();
    void executeCmd(QString command);
    void CmdExit(int exitCode);
    QString strFilter(const QString& str);
    QString getProjectDirSysDiskPartitionSymbol();
    QIcon createColorToolButtonIcon(const QString& image, QColor color);
    QIcon createColorIcon(QColor color);
    QString readProcessOutput(QProcess* process);
    QMenu* createColorMenu(const char* slot, QColor defaultColor);
    QWidget* createBackgroundCellWidget(const QString& text, const QString& image);

};
#endif // MAINWINDOW_H
