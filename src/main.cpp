#include <QApplication>   // ✅ not QApplication2
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Slicer Qt App");
    QApplication::setOrganizationName("YourOrg");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setWindowIcon(QIcon(":/icons/logo.png"));  //from qrc

    MainWindow w;
    w.show();
    return app.exec();
}
