#include <QtGui/QApplication>
#include "mainwindow.h"
#include "referee.h"
#include "mesh.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Referee ref("../data/grammar/demo1.gmr");
    MainWindow w(&ref);
    w.show();

    Feature* root = ref.restart();

    //Feature* root = new Feature("window", "mesh", true, Scope(Vector4(10,10,0,0), Vector4(1, 1, 1, 1)));
    //Mesh* m = new Mesh("../data/entrance.obj");
    //root->setMesh(m);
    w.setRoot(root);

    return a.exec();
}

