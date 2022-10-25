#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QWidget>

namespace Ui {
class Visualize;
}

class Visualize : public QWidget
{
    Q_OBJECT

public:
    explicit Visualize(QWidget *parent = nullptr);
    ~Visualize();

private:
    Ui::Visualize *ui;
};

#endif // VISUALIZE_H
