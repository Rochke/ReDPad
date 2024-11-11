#include "mainwindow.h"
#include "ui_mainwindow.h"

// TO DO :
// Change Color Of Unavailable Menu

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(250, 200);
    fileManager = new FileManager(this, this);
    undoStack = new QUndoStack(this);
    mainStatusBar = this->statusBar();
    menubar = new MenuBar(undoStack, this);
    textDisplay = new TextDisplay(this);
    textDisplay->setupTextDisplay();
    menubar->setupMenuBar();
    statusBarManager = new StatusBarManager(this, textDisplay, this);
    statusBarManager->StatusBarManager::setupStatusBar();
    statusBarManager->updateTotalCount();
    setupLayouts();
    searchManager = new SearchManager(this, this);
    searchManager->search();
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
    qDebug() << width() << ' ' << height() << '\n';
    qDebug() << (width() - 200) / 2 << ' ' << (height() - 100) / 2 << '\n';
}
