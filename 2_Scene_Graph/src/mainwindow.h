#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "node.h"

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

private:
    Ui::MainWindow *ui;

    Node *currentNode;



public slots:


    void slot_initTreeWidget(Node *root);

    void slot_setCurrentNode(QTreeWidgetItem *current);

    void slot_setTransX(int newTransX);
    void slot_setTransY(int newTransY);
    void slot_setAngle(int newTheta);
    void slot_setScaleX(int newScaleX);
    void slot_setScaleY(int newScaleY);


};


#endif // MAINWINDOW_H
