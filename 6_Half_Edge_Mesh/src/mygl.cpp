#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      m_geomCylinder(this), m_geomSphere(this),
      m_progLambert(this), m_progFlat(this), m_progSkinWithFlat(this),
      m_glCamera(), m_mesh(this), m_face(this),
      m_halfedge(this), m_vertex(this), m_skeleton(this), m_joint(nullptr),
      isBound(false)
{
    setFocusPolicy(Qt::StrongFocus);
    Skeleton::initWireframes();

    // I hard coded spaces for 50 matrices
    for (int i = 0; i < 50; ++i)
    {
        // Initialize bind & trans matrices to identity matrices, which make sense
        // if the skeleton is not yet bound to the mesh
        bindMatrices[i] = glm::mat4(1.0f);
        transMatrices[i] = glm::mat4(1.0f);
    }
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomCylinder.destroy();
    m_geomSphere.destroy();
    m_mesh.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
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

    //Create the instances of Cylinder and Sphere.
    m_geomCylinder.create();

    m_geomSphere.create();

    m_mesh.setupCube();
    m_mesh.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendFaces(m_mesh.getFaces());
    emit sendHalfEdges(m_mesh.getHalfEdges());

    m_face.setNewFace(m_mesh.getAFace(0));
    m_face.create();

    m_vertex.setNewVertex(m_mesh.getAVertex(0));
    m_vertex.create();

    m_halfedge.setNewHalfEdge(m_mesh.getAHalfEdge(0));
    m_halfedge.create();

    emit sendAVertex(m_vertex.getVertex());
    emit sendAFace(m_face.getFace());

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    // Create and set up the skeleton-enabled shader; new feature in hw8
    m_progSkinWithFlat.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    m_progLambert.setGeometryColor(glm::vec4(0,1,0,1));

    m_progSkinWithFlat.setBindMatrices(bindMatrices);
    m_progSkinWithFlat.setTransMatrices(transMatrices);

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSkinWithFlat.setViewProjMatrix(viewproj);

    m_progSkinWithFlat.setBindMatrices(bindMatrices);
    m_progSkinWithFlat.setTransMatrices(transMatrices);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSkinWithFlat.setViewProjMatrix(m_glCamera.getViewProj());

//#define NOPE
#ifndef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    //m_progLambert.draw(m_geomSphere);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    m_progLambert.setModelMatrix(model);
    //m_progLambert.draw(m_geomCylinder);

    model = glm::mat4(1.0f);
    m_progLambert.setModelMatrix(model);
    m_progFlat.setModelMatrix(model);
    m_progSkinWithFlat.setModelMatrix(model);

    glEnable(GL_DEPTH_TEST);

    m_progSkinWithFlat.draw(m_mesh);

    glDisable(GL_DEPTH_TEST);
    m_progSkinWithFlat.draw(m_vertex);
    m_progSkinWithFlat.draw(m_face);
    m_progSkinWithFlat.draw(m_halfedge);
    m_progFlat.draw(m_skeleton);
#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutY(-amount);
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutY(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutX(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutX(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        m_halfedge.setNewHalfEdge((m_halfedge.getHalfEdge())->getNextHalfEdge());
    } else if (e->key() == Qt::Key_M) {
        m_halfedge.setNewHalfEdge((m_halfedge.getHalfEdge())->getSymHalfEdge());
    } else if (e->key() == Qt::Key_F) {
        m_face.setNewFace((m_halfedge.getHalfEdge())->getFace());
    } else if (e->key() == Qt::Key_V) {
        m_vertex.setNewVertex((m_halfedge.getHalfEdge())->getVertex());
    } else if (e->key() == Qt::Key_H) {
        m_halfedge.setNewHalfEdge((m_vertex.getVertex())->getOneHalfEdge());
    }
    if (e->key() == Qt::KeyboardModifier::ShiftModifier && e->key() == Qt::Key_H) {
        m_halfedge.setNewHalfEdge((m_face.getFace())->getOneHalfEdge());
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::slot_setCurrentVertex(int index)
{
    m_vertex.setNewVertex(m_mesh.getAVertex(index));
    update();
    emit sendAVertex(m_vertex.getVertex());
}

void MyGL::slot_setCurrentHalfEdge(int index)
{
    m_halfedge.setNewHalfEdge(m_mesh.getAHalfEdge(index));
    update();
}

void MyGL::slot_setCurrentFace(int index)
{
    m_face.setNewFace(m_mesh.getAFace(index));
    update();
    emit sendAFace(m_face.getFace());
}

void MyGL::slot_addANewVertex(bool)
{
    m_mesh.addANewVertex(m_halfedge.getHalfEdge());
    m_mesh.destroy();
    m_mesh.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());

    update();
}

void MyGL::slot_triangulateAFace(bool)
{
    m_mesh.triangulateAFace(m_face.getFace());
    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_setVertexPositionX(double x)
{
    glm::vec4 currentPosition = (m_vertex.getVertex())->getPosition();
    glm::vec4 newPosition((float)x, currentPosition.y, currentPosition.z, 1);

    (m_vertex.getVertex())->setPosition(newPosition);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_setVertexPositionY(double y)
{
    glm::vec4 currentPosition = (m_vertex.getVertex())->getPosition();
    glm::vec4 newPosition(currentPosition.x, (float)y, currentPosition.z, 1);

    (m_vertex.getVertex())->setPosition(newPosition);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_setVertexPositionZ(double z)
{
    glm::vec4 currentPosition = (m_vertex.getVertex())->getPosition();
    glm::vec4 newPosition(currentPosition.x, currentPosition.y, (float)z, 1);

    (m_vertex.getVertex())->setPosition(newPosition);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_setFaceColorR(double r)
{
    glm::vec4 currentColor = (m_face.getFace())->getColor();

    glm::vec4 newColor = currentColor;
    newColor.r = r;

    (m_face.getFace())->setColor(newColor);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    update();
}

void MyGL::slot_setFaceColorG(double g)
{
    glm::vec4 currentColor = (m_face.getFace())->getColor();

    glm::vec4 newColor = currentColor;
    newColor.g = g;

    (m_face.getFace())->setColor(newColor);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    update();
}

void MyGL::slot_setFaceColorB(double b)
{
    glm::vec4 currentColor = (m_face.getFace())->getColor();

    glm::vec4 newColor = currentColor;
    newColor.b = b;

    (m_face.getFace())->setColor(newColor);

    m_mesh.destroy();
    m_mesh.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    m_face.destroy();
    m_face.create();

    update();
}

void MyGL::slot_subdivideCurrentMesh(bool)
{
    m_mesh.catmullclarkSubdivision();

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_extrudeAFace(bool)
{
    m_mesh.extrudeAFace(m_face.getFace(), (float)distance);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_centralDivideAFace(bool)
{
    m_mesh.centralDivideAFace(m_face.getFace(), (float)distance);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendHalfEdges(m_mesh.getHalfEdges());
    emit sendFaces(m_mesh.getFaces());

    update();
}

void MyGL::slot_loadOBJ(bool)
{
    // To support the new features in hw8, a newly loaded mesh should not be controlled
    // by the current skeleton, so the bind & trans matrices should be restored to identity
    // matrices
    for (int i = 0; i < 50; ++i)
    {
        // Reset bind & trans matrices to identity matrices, which make sense
        // if the skeleton is not yet bound to the mesh
        bindMatrices[i] = glm::mat4(1.0f);
        transMatrices[i] = glm::mat4(1.0f);
    }

    isBound = false;

    m_mesh.destroy();
    m_face.destroy();
    m_vertex.destroy();
    m_halfedge.destroy();

    QString filename = QFileDialog::getOpenFileName(0, QString("load OBJ"),
                                                    QString(":/"),
                                                    QString("*.obj"));
    m_mesh.setupOBJ(filename);  // This clears all the primitives on the heap at first

    m_mesh.create();

    emit sendVertices(m_mesh.getVertices());
    emit sendFaces(m_mesh.getFaces());
    emit sendHalfEdges(m_mesh.getHalfEdges());

    m_face.setNewFace(m_mesh.getAFace(0));
    m_face.create();

    m_vertex.setNewVertex(m_mesh.getAVertex(0));
    m_vertex.create();

    m_halfedge.setNewHalfEdge(m_mesh.getAHalfEdge(0));
    m_halfedge.create();

    emit sendAVertex(m_vertex.getVertex());
    emit sendAFace(m_face.getFace());

    update();
}

void MyGL::slot_receiveDistance(double d)
{
    distance = d;
}

void MyGL::slot_loadJSON(bool)
{
    // Since the current skeleton is discarded, all the bind & trans matrices should be reset
    // to identity matrices
    for (int i = 0; i < 50; ++i)
    {
        // Reset bind & trans matrices to identity matrices, which make sense
        // if the skeleton is not yet bound to the mesh
        bindMatrices[i] = glm::mat4(1.0f);
        transMatrices[i] = glm::mat4(1.0f);
    }

    // The new skeleton isn't bound to the mesh by default
    isBound = false;

    // Resume the original look of the mesh along with highlights
    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    // Destroy the VBOs of the current skeleton
    m_skeleton.destroy();

    // Read file and set up a new skeleton
    QString filename = QFileDialog::getOpenFileName(0, QString("load JSON"),
                                                    QString(":/"),
                                                    QString("*.json"));
    m_skeleton.setupJSON(filename);

    // Set the root joint as the default selected joint and modify its display color (bright gold)
    m_joint = m_skeleton.getRootJoint();
    glm::vec4 displayGold(0.92, 0.89, 0.41, 1);
    m_joint->setColor(displayGold);

    m_skeleton.create();

    emit sendRootJoint(m_skeleton.getRootJoint());

    update();
}

void MyGL::slot_translateX(bool)
{
    // Update and recreate skeleton display
    glm::vec4 newPosition = m_joint->getPosition() + glm::vec4((float)translate, 0, 0, 0);
    m_joint->setPosition(newPosition);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_translateY(bool)
{
    // Update and recreate skeleton display
    glm::vec4 newPosition = m_joint->getPosition() + glm::vec4(0, (float)translate, 0, 0);
    m_joint->setPosition(newPosition);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_translateZ(bool)
{
    // Update and recreate skeleton display
    glm::vec4 newPosition = m_joint->getPosition() + glm::vec4(0, 0, (float)translate, 0);
    m_joint->setPosition(newPosition);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_rotateX(bool)
{
    // Update and recreate skeleton display
    glm::vec3 X(1, 0, 0);
    glm::quat newRotation = glm::angleAxis((float)angle, X) * m_joint->getRotation();
    m_joint->setRotation(newRotation);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_rotateY(bool)
{
    // Update and recreate skeleton display
    glm::vec3 Y(0, 1, 0);
    glm::quat newRotation = glm::angleAxis((float)angle, Y) * m_joint->getRotation();
    m_joint->setRotation(newRotation);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_rotateZ(bool)
{
    // Update and recreate skeleton display
    glm::vec3 Z(0, 0, 1);
    glm::quat newRotation = glm::angleAxis((float)angle, Z) * m_joint->getRotation();
    m_joint->setRotation(newRotation);

    m_skeleton.destroy();
    m_skeleton.create();

    // Update and recreate mesh & highlights display
    std::vector<Joint*> joints = m_skeleton.getJointSpheres();
    updateTransMatrices(joints);

    m_mesh.destroy();
    m_mesh.create();

    m_face.destroy();
    m_face.create();

    m_vertex.destroy();
    m_vertex.create();

    m_halfedge.destroy();
    m_halfedge.create();

    update();
}

void MyGL::slot_receiveTranslateDistance(double d)
{
    translate = d;
}

void MyGL::slot_receiveRotateAngle(double angle)
{
    this->angle = angle * M_PI / 180;
}

void MyGL::slot_setCurrentJoint(QTreeWidgetItem *item, int column)
{
    Joint *currentJoint = dynamic_cast<Joint*>(item);
    glm::vec4 originalCyan(0, 1, 1, 1);
    m_joint->setColor(originalCyan);

    m_joint = currentJoint;
    glm::vec4 displayGold(0.92, 0.89, 0.41, 1);
    m_joint->setColor(displayGold);

    m_skeleton.destroy();
    m_skeleton.create();
    update();
}

void MyGL::slot_bindAndSkin(bool)
{
    // Only perform binding if there exists a valid skeleton
    if (m_skeleton.getRootJoint() != nullptr)
    {
        isBound = true;

        m_skeleton.setupBindMatrices();

        std::vector<Joint*> joints = m_skeleton.getJointSpheres();

        // Populate the container with non-trivial bind & trans matrices
        updateBindMatrices(joints);
        updateTransMatrices(joints);

        m_mesh.skinMesh(m_skeleton);
    }
    else
    {
        std::cout << "No skeleton applicable. Please load your skeleton." << std::endl;
    }
}

void MyGL::updateBindMatrices(std::vector<Joint *> &joints)
{
    // Only update if a skeleton is bound to the mesh; otherwise keep using identity
    // matrices for the mesh
    if (isBound)
    {
        for (int i = 0; i < joints.size(); ++i)
        {
            bindMatrices[i] = joints[i]->getBindMatrix();
        }
        m_progSkinWithFlat.setBindMatrices(bindMatrices);
    }
}

void MyGL::updateTransMatrices(std::vector<Joint *> &joints)
{
    // Only update if a skeleton is bound to the mesh; otherwise keep using identity
    // matrices for the mesh
    if (isBound)
    {
        for (int i = 0; i < joints.size(); ++i)
        {
            transMatrices[i] = joints[i]->getOverallTransformation();
        }
        m_progSkinWithFlat.setTransMatrices(transMatrices);
    }
}
