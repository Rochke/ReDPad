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

    void openFind();

    QHBoxLayout *searchLayout;
    QVBoxLayout *searchButtonLayout;
private:
    MainWindow *passedMainWindow;
    QLineEdit *searchBar;
    QPushButton *upButton;
    QPushButton *downButton;
    QLabel *findLabel;

    bool findSetup = false;
    bool findShown = false;

    QSpacerItem* searchSpacerOne;
    QSpacerItem* searchSpacerTwo;

    void setupLayouts();
};

#endif // SEARCHMANAGER_H
