#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QDebug>
#include "custom/DataItem.h"
#include "custom/mygraphicstextitem.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    // enum DiagramType { Step, Conditional, StartEnd, Io };
    enum ModuleType {Input, Output, FFT, IFFT, DFT, HanningWin, BlackmanWin, HT, Filter};


    DiagramItem(ModuleType moduleType, QMenu* contextMenu, QGraphicsItem* parent = nullptr);

    void removeArrow(Arrow* arrow);
    void removeArrows();

    ModuleType diagramType() const
    {
        return myDiagramType;
    }
    QPolygonF polygon() const
    {
        return myPolygon;
    }
    void addArrow(Arrow* arrow);
    QPixmap image() const;

    int type() const override
    {
        return Type;
    }


    QWidget* Widget()const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    ModuleType myDiagramType;
    QPolygonF myPolygon;
    QMenu* myContextMenu;
    QList<Arrow*> arrows;
    AV_DataItem* av_dataItem;
    QGraphicsTextItem* textItem;
    QString text;
};

#endif // DIAGRAMITEM_H
