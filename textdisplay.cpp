#include "textdisplay.h"
#include "mainwindow.h"
#include "undostackmanager.h"

void TextDisplay::setupTextDisplay() {
    TextDisplay* textDisplay = passedMainWindow->getTextDisplay();
    textDisplay->setStyleSheet("QTextEdit { background-color: #1e1e1e; color: white; border: none; }"
                               "QScrollBar::handle:vertical { background: #6d6d6d; }"
                               "QScrollBar:vertical { background: #1e1e1e; }"
                               "QScrollBar::add-line:vertical { background: none; }"
                               "QScrollBar::sub-line:vertical { background: none; }"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { background: none; }"
                               "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
}

void TextDisplay::keyPressEvent(QKeyEvent *event) {
    TextDisplay* textDisplay = passedMainWindow->getTextDisplay();
    QUndoStack* undoStack = passedMainWindow->getUndoStack();
    QTextCursor cursor = textDisplay->textCursor();
    int position = textDisplay->textCursor().position();
    UndoStackManager::OperationType opType;
    UndoStackManager* command;
    bool opPerformed = false;
    //Ctrl+X (Cut)
    if (event->key() == Qt::Key_X && (event->modifiers() & Qt::ControlModifier)) {
        QString text = cursor.selectedText();
        QApplication::clipboard()->setText(text);
        cursor.removeSelectedText();
        opPerformed = true;
        opType = UndoStackManager::OperationType::Cut;
        UndoStackManager *command = new UndoStackManager(textDisplay, position, text, opType, opPerformed, passedMainWindow);
        undoStack->push(command);
    //Ctrl+V (Paste)
    } else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier)) {
        if (cursor.hasSelection()) {
            cursor.removeSelectedText();
        }
        QString text = QApplication::clipboard()->text();
        cursor.insertText(text);
        opPerformed = true;
        opType = UndoStackManager::OperationType::Paste;
        command = new UndoStackManager(textDisplay, position, text, opType, opPerformed, passedMainWindow);
        undoStack->push(command);
    //Ctrl+Z (Undo)
    } else if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)) {
        if (undoStack->canUndo()) {
            undoStack->undo();
        }
    //Ctrl+Y (Redo)
    } else if (event->key() == Qt::Key_Y && (event->modifiers() & Qt::ControlModifier)) {
        if (undoStack->canRedo()) {
            undoStack->redo();
        }
    //Ctrl++ (Zoom In)
    } else if (event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier)) {
        passedMainWindow->statusBarManager->zoomIn();
    //Ctrl+- (Zoom Out)
    } else if (event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier)) {
        passedMainWindow->statusBarManager->zoomOut();
    //Delete/Backspace
    } else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        QString text;
        if(cursor.hasSelection()) {
            text = cursor.selectedText();
            position = cursor.selectionStart();
            cursor.removeSelectedText();
        } else if (event->key() == Qt::Key_Delete) {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            text = cursor.selectedText();
            cursor.removeSelectedText();
        } else if (event->key() == Qt::Key_Backspace) {
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            text = cursor.selectedText();
            cursor.removeSelectedText();
        }
        opPerformed = true;
        opType = UndoStackManager::OperationType::Delete;
        command = new UndoStackManager(textDisplay, position, text, opType, opPerformed, passedMainWindow);
        undoStack->push(command);

        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    //New File
    } else if (event->key() == Qt::Key_N && (event->modifiers() == Qt::ControlModifier)) {
        passedMainWindow->fileManager->newClicked(passedMainWindow->fileName, textDisplay, this);
    //Open File
    } else if (event->key() == Qt::Key_O && (event->modifiers() == Qt::ControlModifier)) {
        passedMainWindow->fileManager->openClicked(passedMainWindow->fileName, textDisplay, this);
    //Save
    } else if (event->key() == Qt::Key_S && (event->modifiers() == Qt::ControlModifier)) {
        passedMainWindow->fileManager->saveClicked(passedMainWindow->fileName, textDisplay, this);
    //Save As
    } else if (event->key() == Qt::Key_S && (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))) {
        passedMainWindow->fileManager->saveAsClicked(passedMainWindow->fileName, textDisplay, this);
    //Find
    } else if (event->key() == Qt::Key_F && (event->modifiers() == Qt::ControlModifier)) {
        passedMainWindow->searchManager->openFind();
    //Tab
    } else if (event->key() == Qt::Key_Tab) {
        cursor.insertText(QString("    "));
    } else if (event->key() == Qt::Key_Up) {
        if(passedMainWindow->searchManager->findShown) {
            passedMainWindow->searchManager->upMatches();
        }
    } else if (event->key() == Qt::Key_Down) {
        if(passedMainWindow->searchManager->findShown) {
            passedMainWindow->searchManager->downMatches();
        }
    }
    //All Other Text
    else {
        QString typedText = event->text();
        if (!typedText.isEmpty()) {
            QTextEdit::keyPressEvent(event);
            int position = cursor.position() - typedText.length();
            opPerformed = true;
            opType = UndoStackManager::OperationType::Type;
            UndoStackManager* command = new UndoStackManager(textDisplay, position, typedText, opType, opPerformed, passedMainWindow);
            undoStack->push(command);

            passedMainWindow->statusBarManager->updateCount();
            passedMainWindow->statusBarManager->updateTotalCount();
        }
        else {
            QTextEdit::keyPressEvent(event);
        }
    }
    passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, textDisplay);
    event->accept();
}

void TextDisplay::wheelEvent(QWheelEvent* event) {
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->angleDelta().y() > 0) {
            passedMainWindow->statusBarManager->StatusBarManager::zoomIn();
        } else if (event->angleDelta().y() < 0) {
            passedMainWindow->statusBarManager->StatusBarManager::zoomOut();
        }
    }
    else {
        QTextEdit::wheelEvent(event);
    }
}

void TextDisplay::pressCtrlC() {
    QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier);
    QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_C, Qt::ControlModifier);
    QApplication::postEvent(QApplication::focusWidget(), pressEvent);
    QApplication::postEvent(QApplication::focusWidget(), releaseEvent);
}

void TextDisplay::pressCtrlA() {
    QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::ControlModifier);
    QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, Qt::Key_A, Qt::ControlModifier);
    QApplication::postEvent(QApplication::focusWidget(), pressEvent);
    QApplication::postEvent(QApplication::focusWidget(), releaseEvent);
}
