#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHBoxLayout"
#include "glwidget.h"
#include <iostream>

using std::cout;
using std::endl;

MainWindow::MainWindow(Referee* ref, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    system("clear");
    ui->setupUi(this);

    m_ref = ref;
    m_root = NULL;
    m_planner = new FloorPlanner();

    this->show();

    this->setRoot(ref->restart());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRoot(Feature* root){

    ui->glWidget->setRoot(root, m_ref->getString());
    delete m_root;
    m_root = root;
}


void MainWindow::keyPressEvent(QKeyEvent* event){

    if (event->key() == Qt::Key_F1) {
        this->setRoot(m_ref->refresh());
    }


    if (event->key() == Qt::Key_F2) {
        this->setRoot(m_ref->restart());
    }


    if (event->key() == Qt::Key_F3) {
        if (m_root != NULL) {
            m_planner->plan(m_root);
            ui->glWidget->setPlanner(m_planner);
        }
    }


    if (event->key() == Qt::Key_F4) {
        ui->glWidget->setPlanner(NULL);
    }

    if (event->key() == Qt::Key_1){
        this->setRoot(m_ref->restart("../data/grammar/demo1.gmr"));
    }

    if (event->key() == Qt::Key_2){
        this->setRoot(m_ref->restart("../data/grammar/demo2.gmr"));
    }

    if (event->key() == Qt::Key_3){
        this->setRoot(m_ref->restart("../data/grammar/demo3.gmr"));
    }

    if (event->key() == Qt::Key_4){
        this->setRoot(m_ref->restart("../data/grammar/demo4.gmr"));
    }

    if (event->key() == Qt::Key_5){
        this->setRoot(m_ref->restart("../data/grammar/demo5.gmr"));
    }

    if (event->key() == Qt::Key_F12){
        this->setRoot(m_ref->restart("../data/grammar/nd.gmr"));
    }

    if (event->key() == Qt::Key_F11){
        this->setRoot(m_ref->restart("../data/grammar/second.gmr"));
    }

    if (event->key() == Qt::Key_F10){
        this->setRoot(m_ref->restart("../data/grammar/first.gmr"));
    }
    if (event->key() == Qt::Key_F9){
        this->setRoot(m_ref->restart("../data/grammar/house.gmr"));
    }

}
