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
    moduleStatusMap.insert(DiagramItem::ModuleType::Input, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::Output, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::FFT, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::IFFT, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::DFT, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::HanningWin, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::BlackmanWin, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::HT, false);
    moduleStatusMap.insert(DiagramItem::ModuleType::Filter, false);
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
        // At this point the selection can change so the first selected item might not be a DiagramTextItem
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
//    QList<QGraphicsItem*> nowItems;
    switch (myMode)
    {
        case InsertItem:
            if (moduleStatusMap[myItemType])
            {
                QMessageBox::warning(nullptr, "Warning", tr("该模块已经存在！"));
                myMode = DiagramScene::MoveItem;
                return;
            }
            if(myItemType == DiagramItem::ModuleType::HT && (moduleStatusMap[DiagramItem::ModuleType::FFT] || moduleStatusMap[DiagramItem::ModuleType::IFFT]))
            {
                QMessageBox::warning(nullptr, "Warning", tr("该模块不能与FFT或IFFT共存！"));
                myMode = DiagramScene::MoveItem;
                return;
            }
            if(moduleStatusMap[DiagramItem::ModuleType::HT] && (myItemType == DiagramItem::ModuleType::FFT || myItemType == DiagramItem::ModuleType::IFFT))
            {
                QMessageBox::warning(nullptr, "Warning", tr("该模块不能与HT共存！"));
                myMode = DiagramScene::MoveItem;
                return;
            }

            moduleStatusMap[myItemType] = true;

            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);

            // store Diagram info, mapping diagram to ModuleType and *pointer
            sceneItemsMap.insert(item, item);
            designerPage->addAttributesBox(item->Widget());
            moduleMap.insert(myItemType, item);

            item->setPos(mouseEvent->scenePos());

            if(myItemType == DiagramItem::ModuleType::DFT)
            {
                analyzerPage->setDftVisibility(true);
            }
            else if(myItemType == DiagramItem::ModuleType::FFT || myItemType == DiagramItem::ModuleType::IFFT || myItemType == DiagramItem::ModuleType::HT)
            {
                analyzerPage->set_FFT_IFFT_HT_Visibility(true);
            }
            emit itemInserted(item);
            showNowItemsCnt();
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
    if (isItemChange(DiagramItem::Type) && this->selectedItems().size() > 0)
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
    if (myMode == InsertLine && line != nullptr)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
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

void DiagramScene::setAnalyzerPage(AnalyzerPage* analyzerPage)
{
    this->analyzerPage = analyzerPage;
}

void DiagramScene::removeDiagramItemInMap(DiagramItem::ModuleType module_type)
{
    qDebug() << "DiagramItem deleted in Map, by DiagramItem::ModuleType";
    moduleStatusMap[module_type] = false;
    moduleMap.remove(module_type);
    if(module_type == DiagramItem::ModuleType::DFT)
    {
        analyzerPage->setDftVisibility(false);
    }
    else if(module_type == DiagramItem::ModuleType::FFT || myItemType == DiagramItem::ModuleType::IFFT || myItemType == DiagramItem::ModuleType::HT)
    {
        analyzerPage->set_FFT_IFFT_HT_Visibility(false);
    }
    showNowItemsCnt();
}

void DiagramScene::removeDiagramItemInMap(DiagramItem* diagramItem)
{
    qDebug() << "DiagramItem deleted in Map, by DiagramItem*";
    DiagramItem::ModuleType _type = diagramItem->diagramType();
    moduleStatusMap[_type] = false;
    moduleMap.remove(_type);
    if(_type == DiagramItem::ModuleType::DFT)
    {
        analyzerPage->setDftVisibility(false);
    }
    else if(_type == DiagramItem::ModuleType::FFT || myItemType == DiagramItem::ModuleType::IFFT || myItemType == DiagramItem::ModuleType::HT)
    {
        analyzerPage->set_FFT_IFFT_HT_Visibility(false);
    }
    showNowItemsCnt();
}

DiagramItem* DiagramScene::getModule(DiagramItem::ModuleType module_type)
{
    /* TODO
    *   null Key!!
    */
    return moduleMap.value(module_type);
}

void DiagramScene::setMainWindow(MainWindow* mainwindow)
{
    this->mainwindow = mainwindow;
}

void DiagramScene::showNowItemsCnt()
{
    QString _msg = QString("当前控件数量: ") + QString::number(moduleMap.size());
    mainwindow->showStatusBarMessage(_msg);
}


bool DiagramScene::isModuleAdded(DiagramItem::ModuleType module_type)
{
    return moduleStatusMap[module_type];
}
