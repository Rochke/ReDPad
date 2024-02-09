#include "statusbarmanager.h"
#include "mainwindow.h"

StatusBarManager::StatusBarManager(MainWindow *mainWindow, QTextEdit *textDisplay, QWidget *parent = nullptr) : passedMainWindow(mainWindow), textDisplay(textDisplay){
    Q_UNUSED(parent)
    QObject::connect(textDisplay, &TextDisplay::cursorPositionChanged, this, &StatusBarManager::updateCount);
}

void StatusBarManager::setupCount() {
    passedMainWindow->lineCount = 1;
    passedMainWindow->charCount = 0;
    countLabel = new QLabel(QString("Line %1, Column %2").arg(passedMainWindow->lineCount).arg(passedMainWindow->charCount));
    countLabel->setAlignment(Qt::AlignLeft);
    countLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passedMainWindow->mainStatusBar->addPermanentWidget(countLabel);
}

void StatusBarManager::setupZoom() {
    zoomDisplay = new QLabel(QString("Zoom %1%").arg(zoomDisplayPercent));
    zoomDisplay->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    zoomDisplay->setAlignment(Qt::AlignRight);
    passedMainWindow->mainStatusBar->addPermanentWidget(zoomDisplay, 1);
}

void StatusBarManager::setupStatusBar() {
    passedMainWindow->mainStatusBar->setContentsMargins(3, 3, 3, 3);
    passedMainWindow->mainStatusBar->setSizeGripEnabled(false);
    passedMainWindow->mainStatusBar->setStyleSheet(
        "QLabel { color: lightgrey; }"
        "QStatusBar::item { border: none; }"
        "QStatusBar { spacing: 5px; }"
        );

    setupCount();
    setupZoom();
}

void StatusBarManager::updateCount() {
    passedMainWindow->lineCount = 1;
    passedMainWindow->charCount = 0;
    QString text = textDisplay->toPlainText();
    int position = textDisplay->textCursor().position();
    for(int i = 0; i < position; i++) {
        if(text[i] == '\n') {
            passedMainWindow->lineCount++;
            passedMainWindow->charCount = 0;
        } else {
            passedMainWindow->charCount++;
        }
    }
    countLabel->setText(QString("Line %1, Column %2").arg(passedMainWindow->lineCount).arg(passedMainWindow->charCount));
}

void StatusBarManager::zoomIn() {
    TextDisplay* textDisplay = passedMainWindow->getTextDisplay();
    QFont font = textDisplay->font();
    if(font.pointSize() < 72) {
        font.setPointSize(font.pointSize() + 1);
        textDisplay->setFont(font);
        zoomDisplayPercent += 10;
        zoomDisplay->setText(QString("Zoom %1%").arg(zoomDisplayPercent));
    }
}

void StatusBarManager::zoomOut() {
    TextDisplay* textDisplay = passedMainWindow->getTextDisplay();
    QFont font = textDisplay->font();
    if(font.pointSize() > 8) {
        font.setPointSize(font.pointSize() - 1);
        textDisplay->setFont(font);
        zoomDisplayPercent -= 10;
        zoomDisplay->setText(QString("Zoom %1%").arg(zoomDisplayPercent));
    }
}

void StatusBarManager::resetZoom() {
    TextDisplay* textDisplay = passedMainWindow->getTextDisplay();
    QFont font = textDisplay->font();
    font.setPointSize(12);
    textDisplay->setFont(font);
    zoomDisplayPercent = 100;
    zoomDisplay->setText(QString("Zoom %1%").arg(zoomDisplayPercent));
}
