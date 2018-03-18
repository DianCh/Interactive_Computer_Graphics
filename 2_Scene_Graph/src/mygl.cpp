// modified by Dian Chen for CIS 560 homework 02

#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      prog_flat(this),
      m_geomGrid(this), m_geomSquare(this, {glm::vec3(0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, -0.5f, 1.f),
                                            glm::vec3(0.5f, -0.5f, 1.f)}),
      m_showGrid(true),
      root(nullptr)     // Initialize the root node pointer to nullptr
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_geomGrid.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the scene geometry
    m_geomGrid.create();
    m_geomSquare.create();

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    root = constructSceneGraph();

}

void MyGL::resizeGL(int w, int h)
{
    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.2, 0.2)); // Screen is -5 to 5

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_showGrid)
    {
        prog_flat.setModelMatrix(glm::mat3());
        prog_flat.draw(*this, m_geomGrid);
    }

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////

    // Shapes will be drawn on top of one another, so the last object
    // drawn will appear in front of everything else

//    prog_flat.setModelMatrix(glm::mat3());
//    m_geomSquare.setColor(glm::vec3(0,1,0));
//    prog_flat.draw(*this, m_geomSquare);

//    m_geomSquare.setColor(glm::vec3(1,0,0));
//    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(-1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(-30.f)));
//    prog_flat.draw(*this, m_geomSquare);

//    m_geomSquare.setColor(glm::vec3(0,0,1));
//    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
//    prog_flat.draw(*this, m_geomSquare);


    emit sendRootNode(root);

    traverseSceneGraph(root, glm::mat3());

    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    // Here is a good spot to call your scene graph traversal function.
    // Any time you want to draw an instance of geometry, call
    // prog_flat.draw(*this, yourNonPointerGeometry);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    switch(e->key())
    {
    case(Qt::Key_Escape):
        QApplication::quit();
        break;

    case(Qt::Key_G):
        m_showGrid = !m_showGrid;
        break;
    }
}

Node* MyGL::constructSceneGraph()
{
    /// Nodes for dog torso, also the root.
    TranslateNode *torsoT = new TranslateNode(nullptr, glm::vec3(1,0,0), "torso translate", 0, 0);
    RotateNode *torsoR = new RotateNode(nullptr, glm::vec3(1,0,0), "torso rotate", 0);
    ScaleNode *torsoS = new ScaleNode(&m_geomSquare, glm::vec3(1,0,0), "torso scale", 3, 1);

    /// Nodes for 4 legs of the dog. Each leg consists upper part and lower part.
    /// To shrink down the variable name while maintaining certain level of self-explanatory,
    /// we use f, b, l, r, u and d to denote front, back, left, right, upper and lower (down),
    /// respectively. There are in total 8 leg parts.

    /// Front right leg parts
    TranslateNode *moveJoint1 = new TranslateNode(nullptr, glm::vec3(0,1,0), "front right upper leg", 1.375, -0.25);
    TranslateNode *fruLegT = new TranslateNode(nullptr, glm::vec3(0,1,0), "fru leg translate", 0, -0.4);
    RotateNode *fruLegR = new RotateNode(nullptr, glm::vec3(0,1,0), "fru leg rotate", -30);
    ScaleNode *fruLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,1,0), "fru leg scale", 0.25, 0.8);

    TranslateNode *moveJoint2 = new TranslateNode(nullptr, glm::vec3(0,1,0), "frongt right lower leg", 0, -0.4);
    TranslateNode *frdLegT = new TranslateNode(nullptr, glm::vec3(0,0,1), "frd leg translate", 0, -0.4);
    RotateNode *frdLegR = new RotateNode(nullptr, glm::vec3(0,0,1), "frd leg rotate", 60);
    ScaleNode *frdLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,0,1), "frd leg scale", 0.25, 0.8);

    /// Front left leg parts
    TranslateNode *moveJoint3 = new TranslateNode(nullptr, glm::vec3(0,1,0), "front left upper leg", 1.375, -0.25);
    TranslateNode *fluLegT = new TranslateNode(nullptr, glm::vec3(0,1,0), "flu leg translate", 0, -0.4);
    RotateNode *fluLegR = new RotateNode(nullptr, glm::vec3(0,1,0), "flu leg rotate", -45);
    ScaleNode *fluLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,1,0), "flu leg scale", 0.25, 0.8);

    TranslateNode *moveJoint4 = new TranslateNode(nullptr, glm::vec3(0,1,0), "front left lower leg", 0, -0.4);
    TranslateNode *fldLegT = new TranslateNode(nullptr, glm::vec3(0,0,1), "fld leg translate", 0, -0.4);
    RotateNode *fldLegR = new RotateNode(nullptr, glm::vec3(0,0,1), "fld leg rotate", 60);
    ScaleNode *fldLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,0,1), "fld leg scale", 0.25, 0.8);

    /// Back right leg parts
    TranslateNode *moveJoint5 = new TranslateNode(nullptr, glm::vec3(0,1,0), "back right upper leg", -1.375, -0.25);
    TranslateNode *bruLegT = new TranslateNode(nullptr, glm::vec3(0,1,0), "bru leg translate", 0, -0.4);
    RotateNode *bruLegR = new RotateNode(nullptr, glm::vec3(0,1,0), "bru leg rotate", -30);
    ScaleNode *bruLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,1,0), "bru leg scale", 0.25, 0.8);

    TranslateNode *moveJoint6 = new TranslateNode(nullptr, glm::vec3(0,1,0), "back right lower leg", 0, -0.4);
    TranslateNode *brdLegT = new TranslateNode(nullptr, glm::vec3(0,0,1), "brd leg translate", 0, -0.4);
    RotateNode *brdLegR = new RotateNode(nullptr, glm::vec3(0,0,1), "brd leg rotate", 60);
    ScaleNode *brdLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,0,1), "brd leg scale", 0.25, 0.8);

    /// Back left leg parts
    TranslateNode *moveJoint7 = new TranslateNode(nullptr, glm::vec3(0,1,0), "back left upper leg", -1.375, -0.25);
    TranslateNode *bluLegT = new TranslateNode(nullptr, glm::vec3(0,1,0), "blu leg translate", 0, -0.4);
    RotateNode *bluLegR = new RotateNode(nullptr, glm::vec3(0,1,0), "blu leg rotate", 45);
    ScaleNode *bluLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,1,0), "blu leg scale", 0.25, 0.8);

    TranslateNode *moveJoint8 = new TranslateNode(nullptr, glm::vec3(0,1,0), "back left lower leg", 0, -0.4);
    TranslateNode *bldLegT = new TranslateNode(nullptr, glm::vec3(0,0,1), "bld leg translate", 0, -0.4);
    RotateNode *bldLegR = new RotateNode(nullptr, glm::vec3(0,0,1), "bld leg rotate", 60);
    ScaleNode *bldLegS = new ScaleNode(&m_geomSquare, glm::vec3(0,0,1), "bld leg scale", 0.25, 0.8);

    TranslateNode *moveJoint9 = new TranslateNode(nullptr, glm::vec3(0,1,0), "head", 1.5, 0.5);
    TranslateNode *headT = new TranslateNode(nullptr, glm::vec3(0,0,1), "head translate", 0, 0.3);
    RotateNode *headR = new RotateNode(nullptr, glm::vec3(0,0,1), "head rotate", 30);
    ScaleNode *headS = new ScaleNode(&m_geomSquare, glm::vec3(0,0,1), "head scale", 0.6, 0.6);

    torsoT->addAChild(moveJoint3);
    moveJoint3->addAChild(fluLegR);
    fluLegR->addAChild(fluLegT);
    fluLegT->addAChild(fluLegS);

    fluLegT->addAChild(moveJoint4);
    moveJoint4->addAChild(fldLegR);
    fldLegR->addAChild(fldLegT);
    fldLegT->addAChild(fldLegS);

    torsoT->addAChild(moveJoint7);
    moveJoint7->addAChild(bluLegR);
    bluLegR->addAChild(bluLegT);
    bluLegT->addAChild(bluLegS);

    bluLegT->addAChild(moveJoint8);
    moveJoint8->addAChild(bldLegR);
    bldLegR->addAChild(bldLegT);
    bldLegT->addAChild(bldLegS);

    torsoT->addAChild(torsoR);
    torsoR->addAChild(torsoS);

    torsoT->addAChild(moveJoint1);
    moveJoint1->addAChild(fruLegR);
    fruLegR->addAChild(fruLegT);
    fruLegT->addAChild(fruLegS);

    fruLegT->addAChild(moveJoint2);
    moveJoint2->addAChild(frdLegR);
    frdLegR->addAChild(frdLegT);
    frdLegT->addAChild(frdLegS);


    torsoT->addAChild(moveJoint5);
    moveJoint5->addAChild(bruLegR);
    bruLegR->addAChild(bruLegT);
    bruLegT->addAChild(bruLegS);

    bruLegT->addAChild(moveJoint6);
    moveJoint6->addAChild(brdLegR);
    brdLegR->addAChild(brdLegT);
    brdLegT->addAChild(brdLegS);

    torsoT->addAChild(moveJoint9);
    moveJoint9->addAChild(headR);
    headR->addAChild(headT);
    headT->addAChild(headS);

    return torsoT;
}

void MyGL::traverseSceneGraph(Node *node, glm::mat3 T)
{
    T = T * node->Transformation();
    for (Node *n : node->getChildren())
    {
        traverseSceneGraph(n, T);
    }
    if ((*node).getGeometry() != nullptr)
    {
        //m_geomSquare.setColor(glm::vec3(0,0,1));
        (*node).getGeometry()->setColor((*node).getColor());
        prog_flat.setModelMatrix(T);
        prog_flat.draw(*this, m_geomSquare);
    }
}
void MyGL::slot_storeTransX(int x)
{
    storeTransX = x;
}

void MyGL::slot_storeTransY(int y)
{
    storeTransY = y;
}

void MyGL::slot_storeAngle(int theta)
{
    storeAngle = theta;
}

void MyGL::slot_storeScaleX(int x)
{
    storeScaleX = x;
}

void MyGL::slot_storeScaleY(int y)
{
    storeScaleY = y;
}

void MyGL::slot_setGeometry(QTreeWidgetItem *current)
{
    dynamic_cast<Node*>(current)->setGeometry(&m_geomSquare);
}

void MyGL::slot_addTransNode()
{
    TranslateNode *newTrans = new TranslateNode(&(m_geomSquare), currentNode->getColor(), "new translation", storeTransX, storeTransY);
    currentNode->addAChild(newTrans);
    currentNode->addChild(newTrans);
}

void MyGL::slot_addRotateNode()
{

}

void MyGL::slot_addScaleNode()
{

}
