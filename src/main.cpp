#include <QtGui/QApplication>
#include "mainwindow.h"
#include "referee.h"
#include "mesh.h"

int main(int argc, char *argv[])
{
    cout << endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Referee ref("../data/grammar/first.gmr");
    //Feature* root = ref.blowWhistle();

    Feature* root = new Feature("cube", "plane", true, Scope(Vector4(0.0, 0.0, 1.0, 0.0)));//ref.blowWhistle();
    //Mesh* m = new Msh("../data/test.obj");
    //root->setMesh(m);
    w.setRoot(root);
    root->setMedia(new QImage("../data/rainbow.png"));
    return a.exec();
}
