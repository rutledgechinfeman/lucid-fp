#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHBoxLayout"
#include "glwidget.h"

MainWindow::MainWindow(Referee* ref, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    system("clear");
    ui->setupUi(this);

    m_ref = ref;
    Feature* root = m_ref->blowWhistle();

    this->setRoot(root);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRoot(Feature* root){
    ui->glWidget->setRoot(root);
}
