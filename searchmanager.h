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
#include <QTextBlock>
#include <QDialog>

class MainWindow;
class GoToLineEdit;

class SearchManager : public QWidget
{
public:
    SearchManager(QWidget*, MainWindow*);

    void openGoToWindow();
    void openFind();
    void search(const QString = "");
    void goTo();
    void upMatches();
    void downMatches();

    bool findShown = false;

    QHBoxLayout *searchLayout;
    QVBoxLayout *searchButtonLayout;

    QDialog *goToWindow = nullptr;
private:
    MainWindow *passedMainWindow;
    QLineEdit *searchBar;
    QPushButton *upButton;
    QPushButton *downButton;
    QLabel *findLabel;
    QLabel *resultLabel;

    int currentMatch = 1;
    int matchCount = 0;
    int queryLength = 0;
    std::vector<int> matchPositions;

    QSpacerItem* searchSpacerOne;
    QSpacerItem* searchSpacerTwo;

    void createFind();
    void setupLayouts();
    void updateMatches(int, int);

    std::vector<int> buildLPS(const QString);
    std::vector<int> KMPSearch(const QString content, const QString query);

    bool eventFilter(QObject *watched, QEvent *event) override;

    GoToLineEdit *goToLineEdit;
};

class GoToLineEdit : public QLineEdit {
public:
    GoToLineEdit(SearchManager *manager, QWidget *parent = nullptr)
        : QLineEdit(parent), searchManager(manager) {}
protected:
    void keyPressEvent(QKeyEvent *event) override {
        if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) ||
            event->key() == Qt::Key_Backspace ||
            event->key() == Qt::Key_Plus ||
            event->key() == Qt::Key_Minus) {
            QLineEdit::keyPressEvent(event);
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            if (searchManager) {
                searchManager->goTo();  // Call the SearchManager's goTo function
            }
        } else if (event->key() == Qt::Key_Escape) {
            searchManager->goToWindow->close();
        }
        else {
            event->accept();
        }
    }

private:
    SearchManager *searchManager;  // Pointer to the SearchManager instance
};

#endif // SEARCHMANAGER_H
