#include "filemanager.h"
#include "mainwindow.h"

FileManager::FileManager(MainWindow *mainWindow, QWidget* parent) : passedMainWindow(mainWindow) { Q_UNUSED(parent) }

void FileManager::saveFile(const QString &fileName, const QTextEdit *textDisplay) {
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        error("Unable To Open File (saveFile())", Error::ProgramStatus::OPEN);
        return;
    }
    QTextStream out(&file);
    out << textDisplay->toPlainText();
    file.close();
}

void FileManager::newClicked(QString &fileName, QTextEdit *textDisplay, QWidget *parent) {
    checkIfSaved(fileName, textDisplay, parent);
    textDisplay->clear();
    fileName.clear();
    passedMainWindow->undoStack->clear();
    updateWindowName(fileName, textDisplay);
    passedMainWindow->statusBarManager->resetZoom();

    passedMainWindow->searchManager->search();
}

void FileManager::openClicked(QString &fileName, QTextEdit *textDisplay, QWidget *parent) {
    checkIfSaved(fileName, textDisplay, parent);
    fileName = QFileDialog::getOpenFileName(parent,
                                            ("Open"),
                                            QDir::homePath(),
                                            ("Text Files (*.txt);;All Files(*)"));
    if(fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error("Unable To Open File (openClicked())", Error::ProgramStatus::OPEN);
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    textDisplay->setPlainText(text);
    file.close();
    updateWindowName(fileName, textDisplay);

    passedMainWindow->searchManager->search();
}

void FileManager::openFile(QString &filePath, QTextEdit *textDisplay) {
    if(filePath.isEmpty()) {
        return;
    }
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error("Unable To Open File (openClicked())", Error::ProgramStatus::OPEN);
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    textDisplay->setPlainText(text);
    file.close();
    updateWindowName(filePath, textDisplay);
    passedMainWindow->fileName = filePath;

    passedMainWindow->searchManager->search();
}

void FileManager::saveAsClicked(QString &fileName, QTextEdit *textDisplay, QWidget *parent) {
    QString fileNameBackup = fileName;
    fileName = QFileDialog::getSaveFileName(parent,
                                            ("Save As"),
                                            QDir::homePath(),
                                            ("Text Files (*.txt);;All Files(*)"));
    if(fileName.isEmpty()) {
        fileName = fileNameBackup;
    } else {
        saveFile(fileName, textDisplay);
    }
    updateWindowName(fileName, textDisplay);
}

void FileManager::saveClicked(QString &fileName, QTextEdit *textDisplay, QWidget *parent) {
    fileName.isEmpty() ? saveAsClicked(fileName, textDisplay, parent) : saveFile(fileName, textDisplay);
    updateWindowName(fileName, textDisplay);
}

void FileManager::checkIfSaved(QString &fileName, QTextEdit *textDisplay, QWidget *parent) {
    bool saved = false;
    if(!fileName.isEmpty()) {
        QFile filecheck(fileName);
        if(!filecheck.open(QIODevice::ReadOnly | QIODevice::Text)) {
            error("Unable To Open File (checkIfSaved())", Error::ProgramStatus::OPEN);
            return;
        }
        QTextStream fcout(&filecheck);
        QString filetext = fcout.readAll();
        QString displaytext = textDisplay->toPlainText();
        if(displaytext == filetext) {
            saved = true;
        }
        filecheck.close();
    }
    if(!textDisplay->toPlainText().isEmpty() && !saved) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(  parent, "Save", "Do you wish to save your unfinished changes?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            saveClicked(fileName, textDisplay, parent);
        }
    }
}

void FileManager::updateWindowName(QString &fileName, QTextEdit *textDisplay) {
    QString displaytext = textDisplay->toPlainText();
    if(fileName.isEmpty()) {
        if(displaytext.isEmpty())
            passedMainWindow->setWindowTitle(QString("No Title | ReDPad"));
        else
            passedMainWindow->setWindowTitle(QString("No Title* | ReDPad"));
        return;
    }
    QFileInfo fileInfo(fileName);
    QString fileDisplayName = fileInfo.fileName();
    bool saved = false;
    QFile filecheck(fileName);
    if(!filecheck.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error("Unable To Open File (updateWindowName())", Error::ProgramStatus::OPEN);
        return;
    }
    QTextStream fcout(&filecheck);
    QString filetext = fcout.readAll();
    if(displaytext == filetext) {
        saved = true;
    }
    filecheck.close();
    if(saved)
        passedMainWindow->setWindowTitle(QString("%1 | ReDPad").arg(fileDisplayName));
    else
        passedMainWindow->setWindowTitle(QString("%1* | ReDPad").arg(fileDisplayName));
}
