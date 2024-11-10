#include "searchmanager.h"
#include "mainwindow.h"

SearchManager::SearchManager(QWidget *parent, MainWindow *passedMainWindow) : passedMainWindow(passedMainWindow) {
    Q_UNUSED(parent);
    searchLayout = new QHBoxLayout();
    searchButtonLayout = new QVBoxLayout();
    passedMainWindow->mainLayout->addLayout(searchLayout);
}

void SearchManager::setupLayouts() {
    searchSpacerOne = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    searchSpacerTwo = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Minimum);
    searchLayout->addSpacerItem(searchSpacerOne);
    searchLayout->addWidget(findLabel);
    searchLayout->addWidget(searchBar);
    searchLayout->addSpacerItem(searchSpacerTwo);

    searchButtonLayout->addWidget(upButton);
    searchButtonLayout->addWidget(downButton);
    searchLayout->addLayout(searchButtonLayout);
}

void SearchManager::openFind() {
    if (!findSetup) {
        findLabel = new QLabel("Find:   ", this);
        searchBar = new QLineEdit(this);
        upButton = new QPushButton("▲", this);
        downButton = new QPushButton("▼", this);
        searchBar->setMinimumSize(60, 30);
        searchBar->setMaximumSize(180, 30);
        upButton->setFixedSize(20, 20);
        downButton->setFixedSize(20, 20);

        upButton->setStyleSheet("QPushButton { font-size: 10px }");
        downButton->setStyleSheet("QPushButton { font-size: 10px }");
        searchBar->setStyleSheet(
            "QLineEdit { "
            "  border: 1px solid white; "
            "  outline: none; "
            "  border-radius: 8px; "
            "  background-color: transparent; "
            "} "
            "QLineEdit:focus { "
            "  border: 1px solid white; "
            "  outline: none; "
            "}"
            );

        setupLayouts();

        findSetup = true;
        findShown = true;

        searchBar->setFocus();

        return;
    }

    if (findShown) {
        findLabel->hide();
        searchBar->hide();
        upButton->hide();
        downButton->hide();

        searchSpacerOne->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        searchSpacerTwo->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

        searchLayout->invalidate(); // Force repaint
        findShown = false;
    } else {
        searchSpacerOne->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        searchSpacerTwo->changeSize(10, 5, QSizePolicy::Minimum, QSizePolicy::Minimum);
        findLabel->show();
        searchBar->show();
        upButton->show();
        downButton->show();

        searchLayout->invalidate(); // Force repaint
        findShown = true;
    }

    searchBar->setFocus();
}

bool SearchManager::eventFilter(QObject *watched, QEvent *event) {
    if (watched == searchBar) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_F && keyEvent->modifiers() == Qt::ControlModifier) {
                openFind();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
