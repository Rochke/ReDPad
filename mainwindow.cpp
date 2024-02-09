#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(250, 100);
    fileManager = new FileManager(this, this);
    undoStack = new QUndoStack(this);
    mainStatusBar = this->statusBar();
    menubar = new MenuBar(undoStack, this);
    textDisplay = new TextDisplay(this);
    textDisplay->setupTextDisplay();
    menubar->setupMenuBar();
    setupLayouts();
    fileManager->updateWindowName(fileName, textDisplay);
    statusBarManager = new StatusBarManager(this, textDisplay, this);
    statusBarManager->StatusBarManager::setupStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLayouts() {
    this->setStyleSheet("background-color: #3a3a3a;");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(textDisplay);
/*
    lineLayout = new QHBoxLayout(centralWidget);
    mainLayout->addLayout(lineLayout);
    lineLayout->setSpacing(0);
    lineLayout->setContentsMargins(0, 0, 0, 0);
*/
}
