#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QSettings>
#include <QCoreApplication>


struct settings_config
{
    QString nodeName;
    QString make_path;
    QString toolchain_path;
    QString openocd_path;       // executable file path
    QString openocd_config;     // OpenOCD config file path
};

class Config
{
public:
    Config(QString cfgFile = "");
    virtual ~Config(void);

    void Set(QString, QString, QVariant);
    QVariant Get(QString, QString) const;
    void loadConfig(QMap<QString, QString> configMap);

private:
    QString pri_cfgFile;
    QSettings* pri_pSettings;
};

#endif // CONFIG_H
