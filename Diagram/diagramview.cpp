#include "diagramview.h"

DiagramView::DiagramView(DiagramScene* scene): QGraphicsView(scene) {}


void DiagramView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        // zoom
        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = event->angleDelta().y();
        qreal factor;
        if (angle > 0)
        {
            factor = 1.1;
        }
        else
        {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(anchor);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}
