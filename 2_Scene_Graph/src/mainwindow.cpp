#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "node.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentNode(nullptr)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl, SIGNAL(sendRootNode(Node*)), this, SLOT(slot_initTreeWidget(Node*)));

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slot_setCurrentNode(QTreeWidgetItem*)));

    connect(ui->transXEntry, SIGNAL(valueChanged(int)), this, SLOT(slot_setTransX(int)));
    connect(ui->transYEntry, SIGNAL(valueChanged(int)), this, SLOT(slot_setTransY(int)));
    connect(ui->angleEntry, SIGNAL(valueChanged(int)), this, SLOT(slot_setAngle(int)));
    connect(ui->scaleXEntry, SIGNAL(valueChanged(int)), this, SLOT(slot_setScaleX(int)));
    connect(ui->scaleYEntry, SIGNAL(valueChanged(int)), this, SLOT(slot_setScaleY(int)));

    connect(ui->transXEntry, SIGNAL(valueChanged(int)), this->ui->mygl, SLOT(slot_storeTransX(int)));
    connect(ui->transYEntry, SIGNAL(valueChanged(int)), this->ui->mygl, SLOT(slot_storeTransY(int)));
    connect(ui->angleEntry, SIGNAL(valueChanged(int)), this->ui->mygl, SLOT(slot_storeAngle(int)));
    connect(ui->scaleXEntry, SIGNAL(valueChanged(int)), this->ui->mygl, SLOT(slot_storeScaleX(int)));
    connect(ui->scaleYEntry, SIGNAL(valueChanged(int)), this->ui->mygl, SLOT(slot_storeScaleY(int)));

    connect(ui->setGeometry, SIGNAL(clicked(bool)), this->ui->mygl, SLOT(slot_setGeometry(QTreeWidgetItem*)));
    connect(ui->addTransNode, SIGNAL(clicked(bool)), this->ui->mygl, SLOT(slot_addTransNode(QTreeWidgetItem*)));
    connect(ui->addRotateNode, SIGNAL(clicked(bool)), this->ui->mygl, SLOT(slot_addRotateNode(QTreeWidgetItem*)));
    connect(ui->addScaleNode, SIGNAL(clicked(bool)), this->ui->mygl, SLOT(slot_addScaleNode(QTreeWidgetItem*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

///----------------------------------------------------------------------
/// vvvvv Implementing signal slots for MainWindow
///----------------------------------------------------------------------

void MainWindow::slot_initTreeWidget(Node *root)
{
    ui->treeWidget->addTopLevelItem(root);
}

void MainWindow::slot_setCurrentNode(QTreeWidgetItem *current)
{
    currentNode = dynamic_cast<Node*>(current);
}

void MainWindow::slot_setTransX(int newTransX)
{
    dynamic_cast<TranslateNode*>(currentNode)->setTranslateX((float)newTransX);
}

void MainWindow::slot_setTransY(int newTransY)
{
    dynamic_cast<TranslateNode*>(currentNode)->setTranslateY((float)newTransY);
}

void MainWindow::slot_setAngle(int newTheta)
{
    dynamic_cast<RotateNode*>(currentNode)->setRotate((float)newTheta);
}

void MainWindow::slot_setScaleX(int newScaleX)
{
    dynamic_cast<ScaleNode*>(currentNode)->setScaleX((float)newScaleX);
}

void MainWindow::slot_setScaleY(int newScaleY)
{
    dynamic_cast<ScaleNode*>(currentNode)->setScaleY((float)newScaleY);
}



///----------------------------------------------------------------------
/// ^^^^^ Implementing signal slots for MainWindow
///----------------------------------------------------------------------
