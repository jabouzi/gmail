#include <QApplication>
#include "gmail.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName( "FMAIL" );
    Gmail al;
    return a.exec();
}
