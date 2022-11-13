#ifndef DESIGNERPAGE_H
#define DESIGNERPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsView>
#include <QToolBox>
#include <QButtonGroup>
#include <QSplitter>
#include <QThread>
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QStackedWidget>
#include "custom/commonHeaders.h"
#include "Diagram/diagramscene.h"
#include "Diagram/diagramitem.h"

namespace Ui
{
    class DesignerPage;
}

class DesignerPage : public QWidget
{
    Q_OBJECT

public:
    explicit DesignerPage(QGraphicsView* view, DiagramScene* scene, QMenu* itemMenu, QWidget* parent = nullptr);
    ~DesignerPage();

    QWidget* getInstance();
    void unCheckButtonGroupItem(int index);
    void unCheckButtonGroupTextItem();
    void addAttributesBox(QWidget* widget);
    void updateAttributesBox(QWidget* widget);
    void resetAttributesBox();

private slots:

    void buttonGroupClicked(QAbstractButton* button);


private:
    Ui::DesignerPage* ui;

    // variables
    QGraphicsView* view = nullptr;
    DiagramScene* scene = nullptr;
    QMenu* itemMenu;
    QFrame* toolBoxes;
    QStackedWidget* attributesBox;
    QWidget* designBoard;
    QWidget* noModuleSelected;
    QButtonGroup* buttonGroup_1;
    QLabel* toolBoxName_1;
    QToolBox* toolBox_1;
    QSpinBox* qSpinBox_pointCnt;
    QSpinBox* qSpinBox_SampleFreq;


    // functions
    void init();
    void createToolbars();
    void initToolBox();
    void initAttributesBox();

    // QWidget* createCellWidget(QButtonGroup*, const QString&, DiagramItem::DiagramType);
    QWidget* createCellWidget(QButtonGroup*, const QString&, DiagramItem::ModuleType);
};

#endif // DESIGNERPAGE_H
