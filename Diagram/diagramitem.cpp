#include "diagramitem.h"
#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagramItem::DiagramItem(ModuleType moduleType, QMenu* contextMenu,
                         QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent), myDiagramType(moduleType)
    , myContextMenu(contextMenu)
{
    QPainterPath path_startend;
    path_startend.moveTo(200, 50);
    path_startend.arcTo(150, 0, 50, 50, 0, 90);
    path_startend.arcTo(50, 0, 50, 50, 90, 90);
    path_startend.arcTo(50, 50, 50, 50, 180, 90);
    path_startend.arcTo(150, 50, 50, 50, 270, 90);
    path_startend.lineTo(200, 25);

    QPolygonF polygon_step;
    polygon_step << QPointF(-100, -100) << QPointF(100, -100)
                 << QPointF(100, 100) << QPointF(-100, 100)
                 << QPointF(-100, -100);

    QPolygonF polygon_conditional;
    polygon_conditional << QPointF(-100, 0) << QPointF(0, 100)
                        << QPointF(100, 0) << QPointF(0, -100)
                        << QPointF(-100, 0);

    QPolygonF polygon_io;
    polygon_io << QPointF(-100, -100) << QPointF(100, -100)
               << QPointF(100, 100) << QPointF(-100, 100)
               << QPointF(-100, -100);
    switch (myDiagramType)
    {
        case Input:
            myPolygon = polygon_conditional;
            av_dataItem = new AV_Input();
            break;
        case Output:
            myPolygon = polygon_conditional;
            av_dataItem = new AV_DataItem();
            break;
        case FFT:
            myPolygon = polygon_step;
            av_dataItem = new AV_FFT_IFFT_DFT();
            break;
        case DFT:
            myPolygon = polygon_step;
            av_dataItem = new AV_FFT_IFFT_DFT();
            break;
        case IFFT:
            myPolygon = polygon_step;
            av_dataItem = new AV_FFT_IFFT_DFT();
            break;
        case HanningWin:
            myPolygon = polygon_step;
            av_dataItem = new AV_HanningWin_BlackmanWin();
            break;
        case BlackmanWin:
            myPolygon = polygon_step;
            av_dataItem = new AV_HanningWin_BlackmanWin();
            break;
        case HT:
            myPolygon = polygon_step;
            av_dataItem = new AV_HT();
            break;
        case Filter:
            myPolygon = polygon_step;
            av_dataItem = new AV_Filter();
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::removeArrow(Arrow* arrow)
{
    arrows.removeAll(arrow);
}

void DiagramItem::removeArrows()
{
    // need a copy here since removeArrow() will
    // modify the arrows container
    const auto arrowsCopy = arrows;
    for (Arrow* arrow : arrowsCopy)
    {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow* arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);
    painter.drawText(10, 5, "FFT");
    painter.setFont(QFont("黑体", 15, QFont::Bold));
    painter.setRenderHint(QPainter::Antialiasing, true);
    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        for (Arrow* arrow : qAsConst(arrows))
        {
            arrow->updatePosition();
        }
    }

    return value;
}

QWidget* DiagramItem::Widget()const
{
    return av_dataItem->Widget();
}
