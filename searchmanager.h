#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>

class MainWindow;

class SearchManager : public QWidget
{
public:
    SearchManager(QWidget*, MainWindow*);

    void openFindWindow();
private:
    MainWindow *passedMainWindow;
protected:
    void paintEvent(QPaintEvent*) override;

    void reposition();

    QWidget* findWindow = nullptr;
};

#endif // SEARCHMANAGER_H
