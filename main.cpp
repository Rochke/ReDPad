/*TO-DO :
 * Line Counter On The Left
 * Settings Menu
 * Saving Settings (Including Of Specific Files, Zoom, Window Size, etc.)
 * Add Color To QMenuBar & QAction's
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
