#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include <QTextEdit>
#include <QFile>
#include <QLabel>
#include <QObject>
#include <QStatusBar>
#include "error.h"

class MainWindow;

class StatusBarManager : public QObject
{
    Q_OBJECT
public:
    StatusBarManager(MainWindow*, QTextEdit*, QWidget*);
    void setupCount();
    void setupZoom();
    void setupStatusBar();
    void zoomIn();
    void zoomOut();
    void resetZoom();

    int zoomDisplayPercent = 100;
    QLabel *zoomDisplay;
public slots:
    void updateCount();
    void updateTotalCount();
private:
    MainWindow *passedMainWindow;
    QTextEdit *textDisplay;
    Error error;

    QLabel *countLabel;
};

#endif // STATUSBARMANAGER_H
