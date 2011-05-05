#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "feature.h"
#include "referee.h"

class GLWidget;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Referee* ref = NULL, QWidget *parent = 0);
    ~MainWindow();
    void setRoot(Feature* root);


private:
    Ui::MainWindow *ui;

    Referee* m_ref;
};

#endif // MAINWINDOW_H
