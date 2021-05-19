#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w(2323);
    w.show();
    return a.exec();
}
