#ifndef UNDOSTACKMANAGER_H
#define UNDOSTACKMANAGER_H

#include <QUndoCommand>
#include <QTextEdit>

class MainWindow;

class UndoStackManager : public QUndoCommand {
public:
    enum class OperationType {
        Paste,
        Cut,
        Type,
        Delete
    };

    UndoStackManager(QTextEdit* textDisplay, int position, const QString& text, OperationType opType, bool opPerformed, MainWindow* passedMainWindow, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;
private:
    MainWindow* passedMainWindow;
    QTextEdit* textDisplay;
    int position;
    QString text;
    OperationType opType;
    bool opPerformed;
};

#endif // UNDOSTACKMANAGER_H
