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

    this->show();

    this->setRoot(ref->restart());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRoot(Feature* root){

    ui->glWidget->setRoot(root);
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

}
