#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "error.h"
#include <QString>
#include <QFile>
#include <QTextEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

class MainWindow;

class FileManager {
public:
    FileManager(MainWindow *mainWindow, QWidget* parent = nullptr);
    void updateWindowName(QString&, QTextEdit*);
    void updateCount(QTextEdit*);
public slots:
    void newClicked(QString&, QTextEdit*, QWidget*);
    void openClicked(QString&, QTextEdit*, QWidget*);
    void openFile(QString&, QTextEdit*);
    void saveClicked(QString&, QTextEdit*, QWidget*);
    void saveAsClicked(QString&, QTextEdit*, QWidget*);
    void checkIfSaved(QString&, QTextEdit*, QWidget*);

private:
    void saveFile(const QString&, const QTextEdit*);
    Error error;
    MainWindow *passedMainWindow;
};

#endif // FILEMANAGER_H
