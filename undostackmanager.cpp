#include "undostackmanager.h"
#include "mainwindow.h"

UndoStackManager::UndoStackManager(QTextEdit* textDisplay, int position, const QString& text, OperationType opType, bool opPerformed, MainWindow* passedMainWindow, QUndoCommand* parent)
    : QUndoCommand(parent), passedMainWindow(passedMainWindow), textDisplay(textDisplay), position(position), text(text), opType(opType), opPerformed(opPerformed)  {
    switch(opType) {
        case OperationType::Paste :
            setText("Pasted");
            break;
        case OperationType::Cut :
            setText("Cut");
            break;
        case OperationType::Type :
            setText("Typed");
            break;
        case OperationType::Delete :
            setText("Deleted");
            break;
    }
}

void UndoStackManager::undo() {
    QTextCursor cursor = textDisplay->textCursor();
    cursor.setPosition(position);
    switch (opType) {
    case OperationType::Delete:
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
        cursor.insertText(text);
        break;
    case OperationType::Paste:
        qDebug() << "Paste (undo)";
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
        cursor.removeSelectedText();
        break;
    case OperationType::Cut:
        qDebug() << "Cut (undo)";
        cursor.insertText(text);
        break;
    case OperationType::Type:
        qDebug() << "Type (undo)";
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
        cursor.removeSelectedText();
        break;
    }
    passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, textDisplay);
}

void UndoStackManager::redo() {
    QTextCursor cursor = textDisplay->textCursor();
    cursor.setPosition(position);
    if(!opPerformed) {
        switch (opType) {
        case OperationType::Delete:
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
            cursor.removeSelectedText();
            break;
        case OperationType::Paste:
            qDebug() << "Pasted (redo) : " << text;
            cursor.insertText(text);
            break;
        case OperationType::Cut:
            qDebug() << "Cut (redo)";
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
            cursor.removeSelectedText();
            break;
        case OperationType::Type:
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
            cursor.insertText(text);
            break;
        }
    }
    passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, textDisplay);
    opPerformed = false;
}
