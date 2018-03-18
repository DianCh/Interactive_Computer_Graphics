#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    ui->changeX->setMinimum(-100.0);
    ui->changeY->setMinimum(-100.0);
    ui->changeZ->setMinimum(-100.0);

    ui->changeX->setSingleStep(0.1);
    ui->changeY->setSingleStep(0.1);
    ui->changeZ->setSingleStep(0.1);

    ui->changeR->setMaximum(1.0);
    ui->changeG->setMaximum(1.0);
    ui->changeB->setMaximum(1.0);

    ui->changeR->setSingleStep(0.05);
    ui->changeG->setSingleStep(0.05);
    ui->changeB->setSingleStep(0.05);

    ui->distance->setMinimum(0.05);
    ui->distance->setSingleStep(0.05);

    ui->translateDistance->setMinimum(-100.0);
    ui->translateDistance->setSingleStep(0.05);

    ui->rotateAngle->setMinimum(-100.0);
    ui->rotateAngle->setSingleStep(1.0);

    connect(ui->mygl, SIGNAL(sendVertices(std::vector<Vertex*>)), this, SLOT(slot_setUpVerticesList(std::vector<Vertex*>)));
    connect(ui->mygl, SIGNAL(sendFaces(std::vector<Face*>)), this, SLOT(slot_setUpFacesList(std::vector<Face*>)));
    connect(ui->mygl, SIGNAL(sendHalfEdges(std::vector<HalfEdge*>)), SLOT(slot_setUpHalfEdgesList(std::vector<HalfEdge*>)));

    connect(ui->vertexList, SIGNAL(currentRowChanged(int)), ui->mygl, SLOT(slot_setCurrentVertex(int)));
    connect(ui->faceList, SIGNAL(currentRowChanged(int)), ui->mygl, SLOT(slot_setCurrentFace(int)));
    connect(ui->halfedgeList, SIGNAL(currentRowChanged(int)), ui->mygl, SLOT(slot_setCurrentHalfEdge(int)));

    connect(ui->addVertex, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_addANewVertex(bool)));
    connect(ui->triangulate, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_triangulateAFace(bool)));

    connect(ui->mygl, SIGNAL(sendAVertex(Vertex*)), this, SLOT(slot_changeXSpinBox(Vertex*)));
    connect(ui->mygl, SIGNAL(sendAVertex(Vertex*)), this, SLOT(slot_changeYSpinBox(Vertex*)));
    connect(ui->mygl, SIGNAL(sendAVertex(Vertex*)), this, SLOT(slot_changeZSpinBox(Vertex*)));

    connect(ui->changeX, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setVertexPositionX(double)));
    connect(ui->changeY, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setVertexPositionY(double)));
    connect(ui->changeZ, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setVertexPositionZ(double)));

    connect(ui->mygl, SIGNAL(sendAFace(Face*)), this, SLOT(slot_changeRSpinBox(Face*)));
    connect(ui->mygl, SIGNAL(sendAFace(Face*)), this, SLOT(slot_changeGSpinBox(Face*)));
    connect(ui->mygl, SIGNAL(sendAFace(Face*)), this, SLOT(slot_changeBSpinBox(Face*)));

    connect(ui->changeR, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setFaceColorR(double)));
    connect(ui->changeG, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setFaceColorG(double)));
    connect(ui->changeB, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_setFaceColorB(double)));

    connect(ui->divideMesh, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_subdivideCurrentMesh(bool)));

    connect(ui->distance, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_receiveDistance(double)));

    connect(ui->extrude, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_extrudeAFace(bool)));

    connect(ui->centralDivide, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_centralDivideAFace(bool)));

    connect(ui->loadOBJ, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadOBJ(bool)));

    connect(ui->loadJSON, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_loadJSON(bool)));
    connect(ui->mygl, SIGNAL(sendRootJoint(Joint*)), this, SLOT(slot_setUpJointsList(Joint*)));

    connect(ui->translateDistance, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_receiveTranslateDistance(double)));
    connect(ui->rotateAngle, SIGNAL(valueChanged(double)), ui->mygl, SLOT(slot_receiveRotateAngle(double)));

    connect(ui->transX, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_translateX(bool)));
    connect(ui->transY, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_translateY(bool)));
    connect(ui->transZ, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_translateZ(bool)));
    connect(ui->rotX, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_rotateX(bool)));
    connect(ui->rotY, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_rotateY(bool)));
    connect(ui->rotZ, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_rotateZ(bool)));

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), ui->mygl, SLOT(slot_setCurrentJoint(QTreeWidgetItem*,int)));

    connect(ui->bindSkeleton, SIGNAL(clicked(bool)), ui->mygl, SLOT(slot_bindAndSkin(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}


void MainWindow::slot_setUpVerticesList(std::vector<Vertex *> vertices)
{
    for (int i = 0; i < vertices.size(); ++i)
    {
        ui->vertexList->addItem(vertices[i]);
    }
}

void MainWindow::slot_setUpFacesList(std::vector<Face *> faces)
{
    for (int i = 0; i < faces.size(); ++i)
    {
        ui->faceList->addItem(faces[i]);
    }
}

void MainWindow::slot_setUpHalfEdgesList(std::vector<HalfEdge *> halfedges)
{
    for (int i = 0; i < halfedges.size(); ++i)
    {
        ui->halfedgeList->addItem(halfedges[i]);
    }
}

void MainWindow::slot_changeXSpinBox(Vertex *v)
{
    glm::vec4 position = v->getPosition();
    ui->changeX->setValue(position.x);
}

void MainWindow::slot_changeYSpinBox(Vertex *v)
{
    glm::vec4 position = v->getPosition();
    ui->changeY->setValue(position.y);
}

void MainWindow::slot_changeZSpinBox(Vertex *v)
{
    glm::vec4 position = v->getPosition();
    ui->changeZ->setValue(position.z);
}

void MainWindow::slot_changeRSpinBox(Face *f)
{
    glm::vec4 color = f->getColor();
    ui->changeR->setValue(color.r);
}

void MainWindow::slot_changeGSpinBox(Face *f)
{
    glm::vec4 color = f->getColor();
    ui->changeG->setValue(color.g);
}

void MainWindow::slot_changeBSpinBox(Face *f)
{
    glm::vec4 color = f->getColor();
    ui->changeB->setValue(color.b);
}

void MainWindow::slot_setUpJointsList(Joint *root)
{
    ui->treeWidget->addTopLevelItem(root);
}
