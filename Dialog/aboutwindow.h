#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QLabel>
#include <QWidget>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QVBoxLayout>

class AboutWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWindow(QWidget* parent = nullptr);

private slots:
    void blog_linkActivated();
    void github_linkActivated();

private:
    QLabel* authorLink;
    QLabel* githubLink;
};

#endif // ABOUTWINDOW_H
