#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QTabWidget>
#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QProcessEnvironment>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDialogButtonBox>
#include <QPushButton>

#include "custom/config.h"


class ConfigWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWindow(QWidget* parent = nullptr);

signals:
    void changesSaved();

private slots:

    void setMakePath();
    void setToolChainDir();
    void setOpenOCDPath();
    void setOpenOCDConfigPath();
    void saveChanges();
    void btn_ok_clicked();
    void btn_cancel_clicked();
private:

    void init();
    QString getDirFromPATH(QString type);
    QString openExplorerForFile(QString type);
    QString openExplorerForDir();
    QPushButton* btn_ok;
    QPushButton* btn_cancel;

    QTabWidget* tabWidget;
    QWidget* make_widget;
    QWidget* toolchain_widget;
    QWidget* openOCD_widget;
    Config* configINI;
    QMap<QString, QString> configs;

    QGroupBox* make_groupbox;
    QGroupBox* toolchain_groupbox;
    QGroupBox* openOCD_path_groupbox;
    QGroupBox* openOCD_config_groupbox;

    QLineEdit* make_path_edit;
    QToolButton* make_path_button;

    QLineEdit* toolchain_path_edit;
    QToolButton* toolchain_path_button;

    QLineEdit* openOCD_path_edit;
    QToolButton* openOCD_path_button;

    QLineEdit* openOCD_config_edit;
    QToolButton* openOCD_config_button;

    QDialogButtonBox* dialogBtnBox;
};

#endif // CONFIGWINDOW_H
