#include "searchmanager.h"
#include "mainwindow.h"

SearchManager::SearchManager(QWidget *parent, MainWindow *passedMainWindow) : passedMainWindow(passedMainWindow) {
    Q_UNUSED(parent);
}

void SearchManager::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
}

void SearchManager::openFindWindow() {
    if(findWindow) {
        delete findWindow;
        qDebug("if");
    }

    findWindow = new QWidget;
    findWindow->setWindowTitle("Find");
    findWindow->setStyleSheet("background-color: #3a3a3a");

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
    label->setStyleSheet("QLabel { color: white; }");
    lineEdit->setStyleSheet("QLineEdit:focus { border: none white; outline: none; }");
    lineEdit->setStyleSheet("QLineEdit { color: white; border: 1px solid white; outline: none; }");
    okButton->setStyleSheet("QPushButton { color: white; }");

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
