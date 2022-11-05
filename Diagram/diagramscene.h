#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H


#include "Pages/analyzerpage.h"
#include "Pages/designerpage.h"
#include "diagramitem.h"
#include "diagramtextitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QMessageBox>
#include <QList>


QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE


class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    explicit DiagramScene(QMenu* itemMenu, QObject* parent = nullptr);
    QFont font() const
    {
        return myFont;
    }
    QColor textColor() const
    {
        return myTextColor;
    }
    QColor itemColor() const
    {
        return myItemColor;
    }
    QColor lineColor() const
    {
        return myLineColor;
    }
    void setLineColor(const QColor& color);
    void setTextColor(const QColor& color);
    void setItemColor(const QColor& color);
    void setFont(const QFont& font);
    void setDesignerpage(DesignerPage* designerPage);
    void setAnalyzerPage(AnalyzerPage* analyzerPage);
    void removeDiagramItemInMap(DiagramItem::ModuleType module_type);
    void removeDiagramItemInMap(DiagramItem* diagramItem);
    DiagramItem* getModule(DiagramItem::ModuleType module_type);
public slots:
    void setMode(DiagramScene::Mode mode);
    void setItemType(DiagramItem::ModuleType type);
    void editorLostFocus(DiagramTextItem* item);

signals:
    void itemInserted(DiagramItem* item);
    void textInserted(QGraphicsTextItem* item);
    void itemSelected(QGraphicsItem* item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

private:
    bool isItemChange(int type) const;
    DesignerPage* designerPage;
    AnalyzerPage* analyzerPage;
    DiagramItem::ModuleType myItemType;
    QMap<QGraphicsItem*, DiagramItem*> sceneItemsMap;
    QMap<DiagramItem::ModuleType, bool> moduleStatusMap;
    QMap<DiagramItem::ModuleType, DiagramItem*> moduleMap;
    QMenu* myItemMenu;
    DiagramScene::Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem* line;
    QFont myFont;
    DiagramTextItem* textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
};


#endif // DIAGRAMSCENE_H
