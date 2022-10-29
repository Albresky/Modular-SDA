#include "mainwindow.h"
#include "sidebar.h"

#include <QDebug>
#include <QEvent>
#include <QMetaMethod>
#include <QPaintEvent>
#include <QPainter>

#define action_height 75

SideBar::SideBar(QWidget* parent)
    : QWidget(parent), mCheckedAction(NULL), mOverAction(NULL)
{
    setMouseTracking(true);
    iconMap = new QMap<QString, QString>();
    iconPath = ":/res/imgs/";
    iconPostfix = ".png";
}

void SideBar::paintEvent(QPaintEvent* event)
{
    QPainter p(this);


    QFont fontText(p.font());
    fontText.setFamily("Microsoft YaHei");
    p.setFont(fontText);

    int action_y = 0;
    p.fillRect(rect(), QColor(100, 100, 100));
    for (auto action : mActions)
    {

        QRect actionRect(0, action_y, event->rect().width(), action_height);

        if (action->isChecked())
        {
            p.fillRect(actionRect, QColor(35, 35, 35));
        }

        if (action == mOverAction)
        {
            p.fillRect(actionRect, QColor(150, 150, 150));
        }

        p.setPen(QColor(255, 255, 255));
        QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect actionTextRect(QPoint(actionRect.width() / 2 - size.width() / 2,
                                    actionRect.bottom() - size.height() - 12),
                             size);
        p.drawText(actionTextRect, Qt::AlignCenter, action->text());

        QRect actionIconRect(0, action_y + 10, actionRect.width(),
                             actionRect.height() - 2 * actionTextRect.height() -
                             10);
        QIcon actionIcon(action->icon());
        actionIcon.paint(&p, actionIconRect);

        action_y += actionRect.height();
    }
}

QSize SideBar::minimumSizeHint() const
{
    return action_height * QSize(1, mActions.size());
}

void SideBar::addAction(QAction* action)
{
    mActions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction* SideBar::addAction(QAction* action, const QString& text, const QString& iconName)
{
    iconMap->insert(text, iconName);
    action->setIcon(QIcon(iconPath + iconName + iconPostfix));
    action->setText(text);
    action->setCheckable(true);
    mActions.push_back(action);
    update();
    return action;
}

QAction* SideBar::addAction(const QString& text, const QString& iconName)
{
    iconMap->insert(text, iconName);
    QAction* action = new QAction(QIcon(iconPath + iconName + iconPostfix), text, this);
    action->setCheckable(true);
    mActions.push_back(action);
    update();
    return action;
}

void SideBar::mousePressEvent(QMouseEvent* event)
{
    QAction* tempAction = actionAt(event->pos());
    if (tempAction == NULL || tempAction->isChecked())
    {
        return;
    }
    qDebug() << "clicked";
    if (mCheckedAction)
    {
        QString unCheckedIcon = iconPath + QString(iconMap->find(mCheckedAction->iconText()).value()) + iconPostfix;
        qDebug() << "Previous Action:" << mCheckedAction->iconText();
        qDebug() << "Previous Action unChecked Icon:" << unCheckedIcon;
        mCheckedAction->setChecked(false);
        mCheckedAction->setIcon(QIcon(unCheckedIcon));
        update();
    }
    if (mOverAction == tempAction)
    {
        mOverAction = NULL;
    }
    qDebug() << "Current Action:" << tempAction->text();
    tempAction->setIcon(QIcon(iconPath + QString(iconMap->find(tempAction->iconText()).value() + "_selected" ) + iconPostfix));
    mCheckedAction = tempAction;
    tempAction->setChecked(true);
    tempAction->trigger();
    update();
    QWidget::mousePressEvent(event);
}

void SideBar::mouseMoveEvent(QMouseEvent* event)
{
    QAction* tempAction = actionAt(event->pos());
    if (tempAction == NULL)
    {
        mOverAction = NULL;
        update();
        return;
    }
    if (tempAction->isChecked() || mOverAction == tempAction)
    {
        return;
    }
    mOverAction = tempAction;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBar::leaveEvent(QEvent* event)
{
    mOverAction = NULL;
    update();
    QWidget::leaveEvent(event);
}

QAction* SideBar::actionAt(const QPoint& at)
{
    int action_y = 0;
    for (auto action : mActions)
    {
        QRect actionRect(0, action_y, rect().width(), action_height);
        if (actionRect.contains(at))
        {
            return action;
        }
        action_y += actionRect.height();
    }
    return NULL;
}

#undef action_height
