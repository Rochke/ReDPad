/*
 *  Welcome To The ReDPad Source Code
 *  V1.1.1
 *  Made By : Rochke (GitHub)
 *  Date: 12/11/2024 (EU)
 *
 * TO-DO :
 * Line Counter On The Left
 * Settings Menu
 * Saving Settings (Including Of Specific Files, Zoom, Window Size, etc.)
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setFont(QFont("Segoe UI"));
    MainWindow w;
    w.setWindowIcon(QIcon(":/ReDPadLogo.ico"));
    w.show();
    QStringList args = QCoreApplication::arguments();
    if(args.length() > 1) {
        QString filePath = args.at(1);
        w.fileManager->openFile(filePath, w.getTextDisplay());
    }
    return a.exec();
}
