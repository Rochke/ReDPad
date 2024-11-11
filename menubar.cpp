#include "menubar.h"
#include "undostackmanager.h"
#include "mainwindow.h"

MenuBar::MenuBar(QUndoStack *undoStack, MainWindow *passedMainWindow) : undoStack(undoStack), passedMainWindow(passedMainWindow) {}

void MenuBar::setupMenuBar() {
    menubar = new QMenuBar(passedMainWindow);
    menubar->setStyleSheet("QMenuBar {"
                           "background-color: #3a3a3a; "
                           "color: lightgrey; }"
                           "QMenuBar::item:selected { background-color: #2a2a2a } "
                           "QMenu { color: white; }"
                           "QMenu::item:selected { background-color: #2a2a2a; } "
                           "QMenu::item:disabled { color: grey; } ");
    QMenu *file = menubar->addMenu("&File");
    QMenu *edit = menubar->addMenu("&Edit");
    //File
    newFile = file->addAction("&New");
    newFile->setShortcut(QKeySequence::New);
    openFile = file->addAction("&Open");
    openFile->setShortcut(QKeySequence::Open);
    file->addSeparator();
    save = file->addAction("&Save");
    save->setShortcut(QKeySequence::Save);
    saveAs = file->addAction("Save As");
    saveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    file->addSeparator();
    exit = file->addAction("Exit");
    //Edit
    undo = edit->addAction("&Undo");
    undo->setEnabled(undoStack->canUndo());
    undo->setShortcut(QKeySequence::Undo);
    redo = edit->addAction("&Redo");
    redo->setEnabled(undoStack->canRedo());
    redo->setShortcut(QKeySequence::Redo);
    edit->addSeparator();
    cut = edit->addAction("Cut");
    cut->setShortcut(QKeySequence::Cut);
    cut->setEnabled(false);
    copy = edit->addAction("&Copy");
    copy->setShortcut(QKeySequence::Copy);
    copy->setEnabled(false);
    paste = edit->addAction("&Paste");
    paste->setShortcut(QKeySequence::Paste);
    paste->setEnabled(false);
    edit->addSeparator();
    //To-Do :
    find = edit->addAction("&Find");
    find->setShortcut(QKeySequence("Ctrl+F"));
    goTo = edit->addAction("&Go To");
    goTo->setShortcut(QKeySequence("Ctrl+G"));
    edit->addSeparator();
    markAll = edit->addAction("&Mark All");
    markAll->setShortcut(QKeySequence::SelectAll);

    connectMenuBar();

    passedMainWindow->setMenuBar(menubar);
}

void MenuBar::connectMenuBar() {
    //File
    connect(newFile, &QAction::triggered, this, [this]() {
        passedMainWindow->fileManager->newClicked(passedMainWindow->fileName, passedMainWindow->getTextDisplay(), passedMainWindow);
    });
    connect(openFile, &QAction::triggered, this, [this]() {
        passedMainWindow->fileManager->openClicked(passedMainWindow->fileName, passedMainWindow->getTextDisplay(), passedMainWindow);
    });
    connect(save, &QAction::triggered, this, [this]() {
        passedMainWindow->fileManager->saveClicked(passedMainWindow->fileName, passedMainWindow->getTextDisplay(), passedMainWindow);
    });
    connect(saveAs, &QAction::triggered, this, [this]() {
        passedMainWindow->fileManager->saveAsClicked(passedMainWindow->fileName, passedMainWindow->getTextDisplay(), passedMainWindow);
    });
    connect(exit, &QAction::triggered, this, [this]() {
        passedMainWindow->fileManager->checkIfSaved(passedMainWindow->fileName, passedMainWindow->getTextDisplay(), passedMainWindow);
        ::exit(0);
    });
    //Edit
    connect(undo, &QAction::triggered, this, [this]() {
        if (undoStack->canUndo()) {
            undoStack->undo();
        }
        passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, passedMainWindow->getTextDisplay());
        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    });
    connect(redo, &QAction::triggered, this, [this]() {
        if (undoStack->canRedo()) {
            undoStack->redo();
        }
        passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, passedMainWindow->getTextDisplay());
        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    });
    connect(cut, &QAction::triggered, this, [this]() {
        QTextCursor cursor = passedMainWindow->getTextDisplay()->textCursor();
        QString text = cursor.selectedText();
        QApplication::clipboard()->setText(text);
        int position = passedMainWindow->getTextDisplay()->textCursor().position();
        cursor.removeSelectedText();
        UndoStackManager::OperationType opType = UndoStackManager::OperationType::Cut;
        UndoStackManager* command = new UndoStackManager(passedMainWindow->getTextDisplay(), position, text, opType, true, passedMainWindow);
        undoStack->push(command);
        passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, passedMainWindow->getTextDisplay());
        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    });
    connect(copy, &QAction::triggered, this, [this]() {
        passedMainWindow->getTextDisplay()->pressCtrlC();
    });
    connect(paste, &QAction::triggered, this, [this]() {
        const QClipboard *clipboard = QApplication::clipboard();
        QString text = clipboard->text();
        int position = passedMainWindow->getTextDisplay()->textCursor().position();
        UndoStackManager::OperationType opType = UndoStackManager::OperationType::Paste;
        UndoStackManager* command = new UndoStackManager(passedMainWindow->getTextDisplay(), position, text, opType, true, passedMainWindow);
        undoStack->push(command);
        passedMainWindow->fileManager->updateWindowName(passedMainWindow->fileName, passedMainWindow->getTextDisplay());
        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    });
    connect(find, &QAction::triggered, this, [this]() {
        passedMainWindow->searchManager->openFind();
    });
    connect(goTo, &QAction::triggered, this, [this]() {
        passedMainWindow->searchManager->openGoToWindow();
    });
    connect(markAll, &QAction::triggered, this, [this]() {
        passedMainWindow->getTextDisplay()->pressCtrlA();
        passedMainWindow->statusBarManager->updateCount();
        passedMainWindow->statusBarManager->updateTotalCount();
    });
    //Enable/Disable Actions
    connect(QApplication::clipboard(), &QClipboard::changed, this, [this]() {
        QClipboard *clipboard = QApplication::clipboard();
        QString clipboardText = clipboard->text();
        paste->setEnabled(!clipboardText.isEmpty());
    });
    connect(passedMainWindow->getTextDisplay(), &QTextEdit::selectionChanged, this, [this]() {
        if(passedMainWindow->getTextDisplay()->textCursor().selectedText().isEmpty()) {
            cut->setEnabled(false);
            copy->setEnabled(false);
        } else {
            cut->setEnabled(true);
            copy->setEnabled(true);
        }
    });
    connect(undoStack, &QUndoStack::canUndoChanged, this, [undo=undo, undoStack=undoStack]() {
        undo->setEnabled(undoStack->canUndo());
    });
    connect(undoStack, &QUndoStack::canRedoChanged, this, [redo=redo, undoStack=undoStack]() {
        redo->setEnabled(undoStack->canRedo());
    });
}
