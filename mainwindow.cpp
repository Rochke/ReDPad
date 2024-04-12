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
    searchManager = new SearchManager();
    mainStatusBar = this->statusBar();
    menubar = new MenuBar(undoStack, this);
    textDisplay = new TextDisplay(this);
    textDisplay->setupTextDisplay();
    menubar->setupMenuBar();
    statusBarManager = new StatusBarManager(this, textDisplay, this);
    statusBarManager->StatusBarManager::setupStatusBar();
    setupLayouts();
    fileManager->updateWindowName(fileName, textDisplay);
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

    searchLayout = new QHBoxLayout();
    mainLayout->addLayout(searchLayout);
    searchLayout->setSpacing(0);
    searchLayout->setContentsMargins(0, 0, 0, 0);

    searchManager->setStyleSheet("background-color: red");
    searchLayout->addWidget(searchManager);
}
