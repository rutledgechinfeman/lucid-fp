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
    Feature* root = ref.blowWhistle();

    w.setRoot(root);
    root->draw();

    return a.exec();
}

