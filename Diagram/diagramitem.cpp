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
    polygon_step << QPointF(-100, -61.8) << QPointF(100, -61.8)
                 << QPointF(100, 61.8) << QPointF(-100, 61.8)
                 << QPointF(-100, -61.8);

    QPolygonF polygon_conditional;
    polygon_conditional  << QPointF(-64.45, -30.9) << QPointF(34.55, -30.9)
                         << QPointF(65.45, 0) << QPointF(34.55, 30.9)
                         << QPointF(-64.45, 30.9) << QPointF(-64.45, -30.9);
    switch (myDiagramType)
    {
        case Input:
            text = "输入";
            myPolygon = polygon_conditional;
            av_dataItem = new AV_Input();
            break;
        case Output:
            text = "输出";
            myPolygon = polygon_conditional;
            av_dataItem = new AV_DataItem();
            break;
        case FFT:
            text = "FFT";
            myPolygon = polygon_step;
            av_dataItem = new AV_FFT_IFFT();
            break;
        case DFT:
            text = "DFT";
            myPolygon = polygon_step;
            av_dataItem = new AV_DFT();
            break;
        case IFFT:
            text = "IFFT";
            myPolygon = polygon_step;
            av_dataItem = new AV_FFT_IFFT();
            break;
        case HanningWin:
            text = "HanningWin";
            myPolygon = polygon_step;
            av_dataItem = new AV_HanningWin_BlackmanWin();
            break;
        case BlackmanWin:
            text = "BlackmanWin";
            myPolygon = polygon_step;
            av_dataItem = new AV_HanningWin_BlackmanWin();
            break;
        case HT:
            text = "HT";
            myPolygon = polygon_step;
            av_dataItem = new AV_HT();
            break;
        case Filter:
            text = "Filter";
            myPolygon = polygon_step;
            av_dataItem = new AV_Filter();
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }

    setCursor(Qt::ClosedHandCursor);

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setFont(QFont("黑体", 20, QFont::Bold));
    painter->setRenderHint(QPainter::Antialiasing);
//    QPen pen = painter->pen();
//    pen.setStyle(Qt::SolidLine);
//    pen.setWidth(2);
//    pen.setColor(QColor(Qt::blue));
//    painter->setPen(pen);
    QGraphicsPolygonItem::paint(painter, option, widget);
    painter->drawText(boundingRect(), Qt::AlignCenter, text);
}

void DiagramItem::removeArrow(Arrow* arrow)
{
    arrows.removeAll(arrow);
}

void DiagramItem::removeArrows()
{
    // need a copy here since removeArrow() will modify the arrows container
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
