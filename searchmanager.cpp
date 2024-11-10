#include "searchmanager.h"
#include "mainwindow.h"

SearchManager::SearchManager(QWidget *parent, MainWindow *passedMainWindow) : passedMainWindow(passedMainWindow) {
    Q_UNUSED(parent);
    searchLayout = new QHBoxLayout();
    searchButtonLayout = new QVBoxLayout();
    passedMainWindow->mainLayout->addLayout(searchLayout);
    createFind();
}

void SearchManager::goTo() {
    if(goToLineEdit->displayText().toInt() > passedMainWindow->totalLineCount) {
        QMessageBox::warning(this, "Error", "No lines past " + QString::number(passedMainWindow->totalLineCount));
        return;
    }
    goToWindow->close();

    QTextCursor cursor = passedMainWindow->getTextDisplay()->textCursor();
    cursor.setPosition(passedMainWindow->getTextDisplay()->document()->findBlockByNumber(goToLineEdit->displayText().toInt() - 1).position());
    passedMainWindow->getTextDisplay()->setTextCursor(cursor);
}

void SearchManager::openGoToWindow() {
    goToWindow = new QDialog(this);
    goToWindow->setWindowTitle("Go To");
    goToWindow->setStyleSheet("background-color: #3a3a3a");

    QLabel *label = new QLabel("Line :", goToWindow);
    goToLineEdit = new GoToLineEdit(this, goToWindow);
    QPushButton *okButton = new QPushButton("Ok", goToWindow);

    QVBoxLayout *outerLayout = new QVBoxLayout;
    QHBoxLayout *firstInnerLayout = new QHBoxLayout;
    QHBoxLayout *secondInnerLayout = new QHBoxLayout;

    firstInnerLayout->addWidget(label);
    firstInnerLayout->addWidget(goToLineEdit);

    secondInnerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    secondInnerLayout->addWidget(okButton);

    label->setStyleSheet("QLabel { color: white; }");
    goToLineEdit->setStyleSheet("QLineEdit:focus { border: none white; outline: none; }");
    goToLineEdit->setStyleSheet("QLineEdit { color: white; border: 1px solid white; outline: none; }");
    okButton->setStyleSheet("QPushButton { color: white; }");

    outerLayout->addLayout(firstInnerLayout);
    outerLayout->addLayout(secondInnerLayout);

    goToWindow->setLayout(outerLayout);

    goToLineEdit->setMinimumWidth(175);
    goToWindow->setFixedHeight(83);

    goToWindow->setModal(true);

    goToLineEdit->setText(QString::number(passedMainWindow->totalLineCount));
    goToLineEdit->selectAll();

    connect(okButton, &QPushButton::clicked, this, &SearchManager::goTo);

    goToWindow->show();

    connect(goToWindow, &QDialog::destroyed, this, [this]() {
        goToWindow = nullptr;
    });
}

void SearchManager::setupLayouts() {
    searchSpacerOne = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    searchSpacerTwo = new QSpacerItem(10, 5, QSizePolicy::Minimum, QSizePolicy::Minimum);
    searchLayout->addWidget(resultLabel);
    searchLayout->addSpacerItem(searchSpacerOne);
    searchLayout->addWidget(findLabel);
    searchLayout->addWidget(searchBar);
    searchLayout->addSpacerItem(searchSpacerTwo);

    searchButtonLayout->addWidget(upButton);
    searchButtonLayout->addWidget(downButton);
    searchLayout->addLayout(searchButtonLayout);
}

void SearchManager::createFind() {
    resultLabel = new QLabel("%1 of %2", this);
    findLabel = new QLabel("Find:   ", this);
    searchBar = new QLineEdit(this);
    upButton = new QPushButton("▲", this);
    downButton = new QPushButton("▼", this);
    searchBar->setMinimumSize(60, 30);
    searchBar->setMaximumSize(180, 30);
    searchBar->installEventFilter(this);
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

    connect(upButton, &QPushButton::clicked, this, &SearchManager::upMatches);
    connect(downButton, &QPushButton::clicked, this, &SearchManager::downMatches);

    findShown = true;
    openFind(); //Closes Find (I know, confusing)

    searchBar->setFocus();
}

void SearchManager::openFind() {
    if (findShown) {
        resultLabel->hide();
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
        resultLabel->show();
        findLabel->show();
        searchBar->show();
        upButton->show();
        downButton->show();

        searchLayout->invalidate(); // Force repaint
        findShown = true;
    }

    search(searchBar->displayText());
    searchBar->setFocus();
}

void SearchManager::search(const QString query) {
    if(query.isEmpty()) {
        updateMatches(0, 0);
        return;
    }

    //Get And Lower Text
    TextDisplay *textDisplay = passedMainWindow->getTextDisplay();
    QString text = textDisplay->toPlainText();
    QString loweredText = text.toLower();

    //Search & Get Match Count
    matchPositions = KMPSearch(loweredText, query);
    matchCount = matchPositions.size();
    queryLength = query.length();

    //Update Match Count Display
    if (matchCount > 0) {
        QTextCursor cursor = passedMainWindow->getTextDisplay()->textCursor();
        cursor.setPosition(matchPositions[0]);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, query.length());

        passedMainWindow->getTextDisplay()->setTextCursor(cursor);

        updateMatches(currentMatch, matchCount);

        passedMainWindow->getTextDisplay()->setFocus();
    } else {
        updateMatches(0, 0);
    }
}

std::vector<int> SearchManager::KMPSearch(const QString content, const QString query) {
    std::vector<int> matchPositions;
    if (query.isEmpty()) return matchPositions;

    std::vector<int> lps = buildLPS(query);

    int n = content.size();
    int m = query.size();

    int i = 0;
    int j = 0;

    while (i < n) {
        if (query[j] == content[i]) {
            i++;
            j++;
        }

        if (j == m) {
            matchPositions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && query[j] != content[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return matchPositions;
}

std::vector<int> SearchManager::buildLPS(const QString query) {
    int size = query.size();
    std::vector<int> array(size, 0);
    int length = 0;
    int i = 1;

    while(i < size) {
        if(query[i] == query[length]) {
            length++;
            array[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = array[length - 1];
            } else {
                array[i] = 0;
                i++;
            }
        }
    }

    return array;
}

void SearchManager::updateMatches(const int currentMatch, const int matchCount) {
    resultLabel->setText(QString(" %1 of %2").arg(currentMatch).arg(matchCount));
}

void SearchManager::upMatches() {
    if (matchCount > 0) {
        QTextCursor cursor = passedMainWindow->getTextDisplay()->textCursor();
        currentMatch = (currentMatch == 1) ? matchCount : currentMatch - 1;
        cursor.setPosition(matchPositions[currentMatch - 1]);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, queryLength);
        passedMainWindow->getTextDisplay()->setTextCursor(cursor);
    }
    updateMatches(currentMatch, matchCount);
    passedMainWindow->getTextDisplay()->setFocus();
}

void SearchManager::downMatches() {
    if (matchCount > 0) {
        QTextCursor cursor = passedMainWindow->getTextDisplay()->textCursor();
        currentMatch = (currentMatch == matchCount) ? 1 : currentMatch + 1;
        cursor.setPosition(matchPositions[currentMatch - 1]);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, queryLength);
        passedMainWindow->getTextDisplay()->setTextCursor(cursor);
    }
    updateMatches(currentMatch, matchCount);
    passedMainWindow->getTextDisplay()->setFocus();
}

bool SearchManager::eventFilter(QObject *watched, QEvent *event) {
    if (watched == searchBar) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_F && keyEvent->modifiers() == Qt::ControlModifier) {
                openFind();
                return true;
            }

            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                QString query = searchBar->text();
                QString loweredQuery = query.toLower();
                search(loweredQuery);
                currentMatch = 1;
                return true;
            }

            if(keyEvent->key() == Qt::Key_Escape) {
                openFind();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
