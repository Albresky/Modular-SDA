#include "diagramscene.h"
#include "arrow.h"


DiagramScene::DiagramScene(QMenu* itemMenu, QObject* parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
//    myItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void DiagramScene::setLineColor(const QColor& color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type))
    {
        Arrow* item = qgraphicsitem_cast<Arrow*>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void DiagramScene::setTextColor(const QColor& color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type))
    {
        DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void DiagramScene::setItemColor(const QColor& color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type))
    {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void DiagramScene::setFont(const QFont& font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type))
    {
        QGraphicsTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
        {
            item->setFont(myFont);
        }
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::ModuleType type)
{
    myItemType = type;
}


void DiagramScene::editorLostFocus(DiagramTextItem* item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())
    {
        removeItem(item);
        item->deleteLater();
    }
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    qDebug() << "mousePressEvent()";
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }

    DiagramItem* item;
    QList<QGraphicsItem*> nowItems;
    switch (myMode)
    {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            nowItems  = this->items();
            sceneItemsMap.insert(nowItems[0], item);
            designerPage->addAttributesBox(item->Widget());
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                                mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, &DiagramTextItem::lostFocus,
                    this, &DiagramScene::editorLostFocus);
            connect(textItem, &DiagramTextItem::selectedChange,
                    this, &DiagramScene::itemSelected);
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            break;
        case MoveItem:
//            designerPage->updateAttributesBox(item->Widget());
            break;
        default:
            break;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
    qDebug() << "selected count:" << this->selectedItems().size();
    if(this->selectedItems().size() > 0)
    {
        designerPage->updateAttributesBox(sceneItemsMap.value(selectedItems()[0])->Widget());
    }
    else
    {
        designerPage->resetAttributesBox();
    }
    qDebug() << "mousePressEvent() ended ";
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    qDebug() << "mouseMoveEvent() started";
    if (myMode == InsertLine && line != nullptr)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    qDebug() << "mouseMoveEvent() ended";
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    qDebug() << "mouseReleaseEvent() started()";
    if (line != nullptr && myMode == InsertLine)
    {
        QList<QGraphicsItem*> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem*> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
        {
            endItems.removeFirst();
        }

        removeItem(line);
        delete line;


        if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == DiagramItem::Type &&
                endItems.first()->type() == DiagramItem::Type &&
                startItems.first() != endItems.first())
        {
            DiagramItem* startItem = qgraphicsitem_cast<DiagramItem*>(startItems.first());
            DiagramItem* endItem = qgraphicsitem_cast<DiagramItem*>(endItems.first());
            Arrow* arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }

    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    qDebug() << "mouseReleaseEvent() ended()";
}


bool DiagramScene::isItemChange(int type) const
{
    const QList<QGraphicsItem*> items = selectedItems();
    const auto cb = [type](const QGraphicsItem * item)
    {
        return item->type() == type;
    };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

void DiagramScene::setDesignerpage(DesignerPage* designerPage)
{
    this->designerPage = designerPage;
}
