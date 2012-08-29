#include <QtGui/QApplication>
#include "depthprobe.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    depthProbe w;
    w.show();
    
    return a.exec();
}
