#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "feature.h"
#include "referee.h"
#include <QKeyEvent>
#include "floorplanner.h"

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

    void keyPressEvent(QKeyEvent *event);


private:
    Ui::MainWindow *ui;

    Referee* m_ref;
    Feature* m_root;
    FloorPlanner* m_planner;
};

#endif // MAINWINDOW_H
