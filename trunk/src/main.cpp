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

    Referee ref("../data/grammar/first.gmr");
    ref.blowWhistle();

    Feature* root = new Feature("cube", "mesh", true, Scope(Vector4(0.0, 0.0, 1.0, 0.0)));//ref.blowWhistle();
    Mesh* m = new Mesh("../data/test.obj");
    root->setMesh(m);
    w.setRoot(root);
    return a.exec();
}
