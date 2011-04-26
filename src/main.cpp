#include <QtGui/QApplication>
#include "mainwindow.h"
#include "referee.h"

int main(int argc, char *argv[])
{
    cout << argv[0] << endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Referee ref("../data/grammar/first.gmr");
    ref.blowWhistle();

    return a.exec();
}
