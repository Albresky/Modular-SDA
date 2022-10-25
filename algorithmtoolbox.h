#ifndef ALGORITHMTOOLBOX_H
#define ALGORITHMTOOLBOX_H

#include <QWidget>
#include <QPushButton>

class AlgorithmToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit AlgorithmToolBox(QWidget *parent = nullptr);

signals:

};


class Algorithm : public  QPushButton{
    Q_OBJECT
public :
    explicit Algorithm(QPushButton *parent = nullptr);

//    void setName(QString name);
//    void setIcon(QString )
private:
    QString name;
    QIcon *icon;
};

#endif // ALGORITHMTOOLBOX_H
