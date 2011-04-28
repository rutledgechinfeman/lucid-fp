#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHBoxLayout"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    system("clear");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setRoot(Feature* root){
    ui->glWidget->setRoot(root);
}
