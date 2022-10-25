#ifndef CTABBUTTON_H
#define CTABBUTTON_H

#include <QWidget>
#include <QToolButton>
#include "qevent.h"
#include "qpainter.h"

class cTabButton : public QToolButton{

    Q_OBJECT

public:

    cTabButton(const QString& path, QWidget *parent=0);

     ~cTabButton();


protected:

    QString path;

    bool m_mousePressed;

    bool m_mouseHover;

    void doPaintStuff(int topColor, int centerColor, int bottomColor);

    void setButtonPressed(bool isPressed);

    void enterEvent(QEnterEvent *);

    void leaveEvent(QEvent *);

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
};

#endif
