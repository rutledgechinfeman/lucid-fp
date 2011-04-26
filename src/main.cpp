#include <QtGui/QApplication>
#include "mainwindow.h"
#include "referee.h"
#include "mesh.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Referee ref("../data/grammar/first.gmr");
    ref.blowWhistle();
    Mesh m = Mesh("../data/test.obj");

    return a.exec();
}
