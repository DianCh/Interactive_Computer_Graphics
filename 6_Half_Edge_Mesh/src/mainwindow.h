#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mesh.h"
#include "skeleton.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

public slots:
    void slot_setUpVerticesList(std::vector<Vertex*> vertices);
    void slot_setUpFacesList(std::vector<Face*> faces);
    void slot_setUpHalfEdgesList(std::vector<HalfEdge*> halfedges);

    void slot_changeXSpinBox(Vertex *v);
    void slot_changeYSpinBox(Vertex *v);
    void slot_changeZSpinBox(Vertex *v);

    void slot_changeRSpinBox(Face *f);
    void slot_changeGSpinBox(Face *f);
    void slot_changeBSpinBox(Face *f);

    void slot_setUpJointsList(Joint *root);        // New feature in hw8; set up the tree widget list

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
