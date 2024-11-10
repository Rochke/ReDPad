#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <textdisplay.h>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QEvent>
#include <QKeyEvent>

class MainWindow;

class SearchManager : public QWidget
{
public:
    SearchManager(QWidget*, MainWindow*);

    void openFind();
    void search(const QString = "");

    QHBoxLayout *searchLayout;
    QVBoxLayout *searchButtonLayout;
private:
    MainWindow *passedMainWindow;
    QLineEdit *searchBar;
    QPushButton *upButton;
    QPushButton *downButton;
    QLabel *findLabel;
    QLabel *resultLabel;

    bool findShown = false;

    QSpacerItem* searchSpacerOne;
    QSpacerItem* searchSpacerTwo;

    void createFind();
    void setupLayouts();
    void updateMatches(int, int);

    std::vector<int> buildLPS(const QString);
    std::vector<int> KMPSearch(const QString content, const QString query);

    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // SEARCHMANAGER_H
