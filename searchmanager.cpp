#include "searchmanager.h"
#include "mainwindow.h"

SearchManager::SearchManager(QWidget *parent, MainWindow *passedMainWindow) : passedMainWindow(passedMainWindow) {
    Q_UNUSED(parent);
}

void SearchManager::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
}
