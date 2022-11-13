#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include "diagramscene.h"

#include <QGraphicsView>
#include <QWheelEvent>

class DiagramView : public QGraphicsView
{
    Q_OBJECT
public:

    DiagramView(DiagramScene* scene);


protected:
    void wheelEvent(QWheelEvent* event) override;
};
#endif // DIAGRAMVIEW_H
