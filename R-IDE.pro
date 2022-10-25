QT       += core gui core5compat charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

requires(qtConfig(combobox))


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileOperator/FileOperator.cpp \
    Pages/designerpage.cpp \
    algorithmtoolbox.cpp \
    charts.cpp \
    codepage.cpp \
    custom/cTabButton.cpp \
    custom/mypushbutton.cpp \
    globalVar.cpp \
    main.cpp \
    mainwindow.cpp \
    syntax/CodeEditor.cpp \
    syntax/SyntaxHighlighter.cpp \
    themewidget.cpp \
    utilities.cpp \
    visualize.cpp

HEADERS += \
    FileOperator/FileOperator.h \
    Pages/designerpage.h \
    algorithmtoolbox.h \
    charts.h \
    codepage.h \
    commonHeaders.h \
    custom/cTabButton.h \
    custom/mypushbutton.h \
    globalVar.h \
    mainwindow.h \
    syntax/CodeEditor.h \
    syntax/SyntaxHighlighter.h \
    syntax/typedef.h \
    themewidget.h \
    utilities.h \
    visualize.h

FORMS += \
    Pages/designerpage.ui \
    charts.ui \
    codepage.ui \
    mainwindow.ui \
    themewidget.ui \
    visualize.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = $$PWD/res/icons/logo.ico

DISTFILES +=
