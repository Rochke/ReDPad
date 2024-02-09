#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <QKeyEvent>
#include <QWidget>
#include <QApplication>
#include <QTextEdit>
#include <QWheelEvent>
#include <QMainWindow>
#include <QLabel>

class MainWindow;

class TextDisplay : public QTextEdit {
    Q_OBJECT
public:
    explicit TextDisplay(MainWindow *mainWindow = nullptr, QWidget *parent = nullptr) :
        passedMainWindow(mainWindow)
    {
        Q_UNUSED(parent); Q_UNUSED(mainWindow);
    }

    MainWindow* passedMainWindow;

    void keyPressEvent(QKeyEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    void setupTextDisplay();
    void pressCtrlZ();
    void pressCtrlY();
    void pressCtrlX();
    void pressCtrlC();
    void pressCtrlV();
    void pressCtrlA();
};

#endif // TEXTDISPLAY_H
