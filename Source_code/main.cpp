#include "lida.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LIDA w;
    w.show();

    return a.exec();
}
