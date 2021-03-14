#include "Qt64vcpkgr.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt64vcpkgr w;
    w.show();
    return a.exec();
}
