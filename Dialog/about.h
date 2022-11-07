#ifndef ABOUT_H
#define ABOUT_H

#include <QLabel>
#include <QWidget>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QVBoxLayout>

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget* parent = nullptr);

private slots:
    void blog_linkActivated();
    void github_linkActivated();

private:
    QLabel* authorLink;
    QLabel* githubLink;
};

#endif // ABOUT_H
