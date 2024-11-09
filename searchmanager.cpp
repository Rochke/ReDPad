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
        findLabel = new QLabel("Find: ", this);
        searchBar = new QLineEdit(this);
        upButton = new QPushButton("▲", this);
        downButton = new QPushButton("▼", this);
        searchBar->setMinimumSize(60, 30);
        searchBar->setMaximumSize(180, 30);
        upButton->setFixedSize(20, 20);
        downButton->setFixedSize(20, 20);

        upButton->setStyleSheet("QPushButton { font-size: 10px }");
        downButton->setStyleSheet("QPushButton { font-size: 10px }");
        setupLayouts();

        findSetup = true;
        findShown = true;

        searchBar->setFocus();

        return;
    }

    if (findShown) {
        // Hide widgets and set spacer size to zero
        findLabel->hide();
        searchBar->hide();
        upButton->hide();
        downButton->hide();

        searchSpacerOne->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        searchSpacerTwo->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

        searchLayout->invalidate(); // Refresh the layout
        findShown = false;
    } else {
        // Show widgets and restore original spacer sizes
        searchSpacerOne->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        searchSpacerTwo->changeSize(10, 5, QSizePolicy::Minimum, QSizePolicy::Minimum);
        findLabel->show();
        searchBar->show();
        upButton->show();
        downButton->show();

        searchLayout->invalidate(); // Refresh the layout
        findShown = true;
    }

    searchBar->setFocus();
}
