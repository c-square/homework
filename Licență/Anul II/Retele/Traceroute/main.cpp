#include "trmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    trMain w;
    w.show();
    
    return a.exec();
}
