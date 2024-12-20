#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "error.h"
#include "filemanager.h"
#include "statusbarmanager.h"
#include "textdisplay.h"
#include "menubar.h"
#include "searchmanager.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QShortcut>
#include <QCloseEvent>
#include <QClipboard>
#include <QUndoStack>
#include <cstdint>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    FileManager *fileManager;
    StatusBarManager *statusBarManager;
    QUndoStack *undoStack;
    QStatusBar *mainStatusBar;
    MenuBar *menubar;
    SearchManager *searchManager;
    Error error;

    QString fileName;
    uint64_t lineCount;
    uint64_t charCount;
    uint64_t totalLineCount;

    QVBoxLayout *mainLayout;

    TextDisplay *getTextDisplay() {
        return textDisplay;
    }

    QUndoStack* getUndoStack() {
        return undoStack;
    }

private:
    Ui::MainWindow *ui;
    TextDisplay *textDisplay;

    void setupLayouts();
protected:
    void closeEvent(QCloseEvent *event) override {
        fileManager->checkIfSaved(fileName, textDisplay, this);
        event->accept();
    }
};
#endif // MAINWINDOW_H
