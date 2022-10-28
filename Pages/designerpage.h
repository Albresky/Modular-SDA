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


namespace Ui
{
    class DesignerPage;
}

class DesignerPage : public QWidget
{
    Q_OBJECT

public:
    explicit DesignerPage(QGraphicsView* view, QWidget* parent = nullptr);
    ~DesignerPage();

    QWidget* getInstance();



private:
    Ui::DesignerPage* ui;

    // variables
    QListWidget* toolBox_0;
    QListWidget* toolBox_1;
    QListWidget* toolBox_2;
    QLabel* toolBoxName_0;
    QLabel* toolBoxName_1;
    QLabel* toolBoxName_2;
    QWidget* toolBoxes;
    QWidget* designBoard;

    QGraphicsView* view = nullptr;

    // functions
    void init();
    void createToolbars();

};

#endif // DESIGNERPAGE_H
