#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QUndoStack>
#include <QMainWindow>

class MainWindow;

class MenuBar : public QObject {
    Q_OBJECT
public:
    MenuBar(QUndoStack*, MainWindow*);

    void setupMenuBar();
    void connectMenuBar();

    QMenuBar *menubar;
private:
    QUndoStack *undoStack;
    MainWindow *passedMainWindow;

    QAction *newFile;
    QAction *openFile;
    QAction *save;
    QAction *saveAs;
    QAction *exit;

    QAction *undo;
    QAction *redo;
    QAction *cut;
    QAction *copy;
    QAction *paste;
    QAction *find;
    QAction *goTo;
    QAction *markAll;
};

#endif // MENUBAR_H
