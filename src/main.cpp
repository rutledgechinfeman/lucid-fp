#include <QtGui/QApplication>
#include "mainwindow.h"
#include "referee.h"
#include "mesh.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Referee ref("../data/grammar/first.gmr");

    MainWindow w(ref);
    w.show();


    Feature* root = ref.blowWhistle();

    w.setRoot(root);
    root->draw();

    return a.exec();
}
