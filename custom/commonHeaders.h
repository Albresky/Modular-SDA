#ifndef COMMONHEADERS_H
#define COMMONHEADERS_H

#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QIcon>
#include <QFileDialog>
#include <stdlib.h>

class MainWindow;
class ProjectPage;
class ThemeWidget;

extern QString prefix;

void updateGlobalVar(QString str);

struct AnalysisVaule
{
    int validValue;
    float baseFreq;
    int baseAmp;
    float _3rdFreq;
    int _3rdAmp;
    float _5thFreq;
    int _5thAmp;
    float _7thFreq;
    int _7thAmp;
};

#endif // COMMONHEADERS_H
