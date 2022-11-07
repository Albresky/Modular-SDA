QT       += core gui core5compat charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

requires(qtConfig(combobox))


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Diagram/arrow.cpp \
    Diagram/diagramitem.cpp \
    Diagram/diagramscene.cpp \
    Diagram/diagramtextitem.cpp \
    Diagram/diagramview.cpp \
    FileOperator/FileOperator.cpp \
    Pages/analyzerpage.cpp \
    Pages/codepage.cpp \
    Pages/designerpage.cpp \
    Pages/projectpage.cpp \
    custom/DataItem.cpp \
    custom/commonHeaders.cpp \
    custom/globalVar.cpp \
    custom/sidebar.cpp \
    lib/XCustomPlot.cpp \
    lib/XTracer.cpp \
    lib/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp \
    syntax/CodeEditor.cpp \
    syntax/SyntaxHighlighter.cpp



HEADERS += \
    Diagram/arrow.h \
    Diagram/diagramitem.h \
    Diagram/diagramscene.h \
    Diagram/diagramtextitem.h \
    Diagram/diagramview.h \
    FileOperator/FileOperator.h \
    Pages/analyzerpage.h \
    Pages/codepage.h \
    Pages/designerpage.h \
    Pages/projectpage.h \
    custom/DataItem.h \
    custom/commonHeaders.h \
    custom/globalVar.h \
    custom/sidebar.h \
    lib/XCustomPlot.h \
    lib/XTracer.h \
    lib/qcustomplot.h \
    mainwindow.h \
    syntax/Codeeditor.h \
    syntax/SyntaxHighlighter.h \
    syntax/typedef.h



FORMS += \
    Pages/designerpage.ui \
    custom/codepage.ui \
    mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = $$PWD/res/icons/logo.ico

DISTFILES += \
    res/icons/build.ico \
    res/icons/clean.ico \
    res/icons/fft.ico \
    res/icons/logo.ico \
    res/images/background1.png \
    res/images/background2.png \
    res/images/background3.png \
    res/images/background4.png \
    res/images/background5.png \
    res/images/bold.png \
    res/images/bringtofront.png \
    res/images/delete.png \
    res/images/floodfill.png \
    res/images/italic.png \
    res/images/linecolor.png \
    res/images/linepointer.png \
    res/images/pointer.png \
    res/images/sendtoback.png \
    res/images/textpointer.png \
    res/images/underline.png \
    res/imgs/build.png \
    res/imgs/debug.png \
    res/imgs/help.png \
    res/imgs/run.png \
    res/imgs/splitter.png \
    res/imgs/unknownfile.png \
    res/sidebar/charts.png \
    res/sidebar/charts_selected.png \
    res/sidebar/circuit.png \
    res/sidebar/circuit_selected.png \
    res/sidebar/edit.png \
    res/sidebar/edit_selected.png \
    res/sidebar/tool.png \
    res/sidebar/tool_selected.png \
    res/sidebar/welcome.png \
    res/sidebar/welcome_selected.png
