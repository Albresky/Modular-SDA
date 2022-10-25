#ifndef DESIGNERPAGE_H
#define DESIGNERPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


namespace Ui
{
    class DesignerPage;
}

class DesignerPage : public QWidget
{
    Q_OBJECT

public:
    explicit DesignerPage(QWidget* parent = nullptr);
    ~DesignerPage();

private:
    Ui::DesignerPage* ui;

    // variables
    QListWidget* toolBox_0;
    QListWidget* toolBox_1;
    QListWidget* toolBox_2;
    QLabel* toolBoxName_0;
    QLabel* toolBoxName_1;
    QLabel* toolBoxName_2;

    // functions
    void init();

};

#endif // DESIGNERPAGE_H
