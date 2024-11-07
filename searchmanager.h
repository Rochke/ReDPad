#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>

class MainWindow;

class SearchManager : public QWidget
{
public:
    SearchManager(QWidget*, MainWindow*);

    void openFindWindow() {
        if(findWindow) {
            findWindow->raise();
            findWindow->activateWindow();
            qDebug("if");
            return;
        }

        findWindow = new QWidget;
        findWindow->setWindowTitle("Find");

        QLabel *label = new QLabel("Find :", findWindow);
        QLineEdit *lineEdit = new QLineEdit(findWindow);
        QPushButton *okButton = new QPushButton("Ok", findWindow);

        QVBoxLayout *outerLayout = new QVBoxLayout;
        QHBoxLayout *firstInnerLayout = new QHBoxLayout;
        QHBoxLayout *secondInnerLayout = new QHBoxLayout;
        firstInnerLayout->addWidget(label);
        firstInnerLayout->addWidget(lineEdit);
        secondInnerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        secondInnerLayout->addWidget(okButton);

        outerLayout->addLayout(firstInnerLayout);
        outerLayout->addLayout(secondInnerLayout);

        findWindow->setLayout(outerLayout);

        lineEdit->setMinimumWidth(175);
        findWindow->setFixedHeight(83);

        findWindow->show();

        connect(findWindow, &QWidget::destroyed, this, [this]() {
            findWindow = nullptr;
            qDebug("Reset findWindow()");
        });

        qDebug("openFindWindow()");
    }
private:
    MainWindow *passedMainWindow;
protected:
    void paintEvent(QPaintEvent*) override;

    void reposition();

    QWidget* findWindow = nullptr;
};

#endif // SEARCHMANAGER_H
