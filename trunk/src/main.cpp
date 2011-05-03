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

    //Feature* root = new Feature("cube", "mesh", true, Scope());//ref.blowWhistle();
    //Mesh* m = new Mesh("../data/ring.obj");
    //root->setMesh(m);
    w.setRoot(root);
    root->draw();
    //root->setMedia(new QImage("../data/rainbow.png"));
    return a.exec();
}
