#ifndef DESIGNERPAGE_H
#define DESIGNERPAGE_H

#include "Diagram/diagramscene.h"
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsView>
#include <QToolBox>


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


private slots:

    void buttonGroupClicked(QAbstractButton* button);


private:
    Ui::DesignerPage* ui;

//    MainWindow* parent;

    // variables
    QGraphicsView* view = nullptr;
    DiagramScene* scene = nullptr;
    QMenu* itemMenu;
    QWidget* toolBoxes;
    QWidget* designBoard;
    QButtonGroup* buttonGroup_1;
    QButtonGroup* buttonGroup_2;
    QButtonGroup* buttonGroup_3;
    QLabel* toolBoxName_1;
    QLabel* toolBoxName_2;
    QLabel* toolBoxName_3;

    QToolBox* toolBox_1;


    // functions
    void init();
    void createToolbars();
    void loadToolBox();
    QWidget* createCellWidget(QButtonGroup*, const QString&, DiagramItem::DiagramType);
};

#endif // DESIGNERPAGE_H
