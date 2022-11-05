#include "mygraphicstextitem.h"
#include "qpainter.h"

MyGraphicsTextItem::MyGraphicsTextItem(QBrush brush, QGraphicsItem* parent): QGraphicsTextItem(parent), brush(brush)
{
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setBrush(brush);
    painter->setPen(QPen(Qt::black));
    painter->drawRect(boundingRect());
    QGraphicsTextItem::paint(painter, option, widget);
}
