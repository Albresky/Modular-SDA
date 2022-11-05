#ifndef MYGRAPHICSTEXTITEM_H
#define MYGRAPHICSTEXTITEM_H

#include <QBrush>
#include <QGraphicsTextItem>

class MyGraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyGraphicsTextItem(QBrush brush = QBrush(Qt::white), QGraphicsItem* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
private:
    QBrush brush;
};

#endif // MYGRAPHICSTEXTITEM_H
