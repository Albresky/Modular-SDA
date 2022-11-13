#include "config.h"
#include "FileOperator/FileOperator.h"

Config::Config(QString cfgFile)
{
    if(!isFile(cfgFile) && createFile(cfgFile))
    {
        pri_cfgFile = QCoreApplication::applicationDirPath() + cfgFile;
    }
    else
    {
        pri_cfgFile = cfgFile;
        qDebug() << "Config already exists => " << pri_cfgFile;
    }
    pri_pSettings = new QSettings(pri_cfgFile, QSettings::IniFormat);
}


Config::~Config(void)
{
    delete pri_pSettings;
    pri_pSettings = 0;
}


void Config::Set(QString nodeName, QString keyName, QVariant value)
{
    pri_pSettings->setValue(QString("/%1/%2").arg(nodeName, keyName), value);
}


QVariant Config::Get(QString nodeName, QString keyName) const
{
    QVariant qvar = pri_pSettings->value(QString("/%1/%2").arg(nodeName, keyName));
    return qvar;
}

void Config::loadConfig(QMap<QString, QString> configMap)
{
    QMap<QString, QString>::iterator it;
    for(it = configMap.begin(); it != configMap.end(); it++)
    {
        if(it.key()!="nodename"){
              this->Set(configMap.value("nodename"), it.key(), it.value());
        }
    }
}
