// modified by Dian Chen for CIS 560 homework 02

#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/grid.h>
#include <scene/polygon.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "node.h"


class MyGL
    : public GLWidget277
{
    Q_OBJECT

private:
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    Grid m_geomGrid; // The instance of the object used to render the 5x5 grid
    Polygon2D m_geomSquare; // The singular instance of our square object that can be re-drawn with different colors
                         // to create the appearance of there being multiple squares

    bool m_showGrid; // Read in paintGL to determine whether or not to draw the grid.

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    Node *root;

    QString storeName;
    float storeTransX;
    float storeTransY;
    float storeAngle;
    float storeScaleX;
    float storeScaleY;
    glm::vec3 storeColor;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    Node* constructSceneGraph();
    void traverseSceneGraph(Node *node, glm::mat3 T);

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slot_storeTransX(int x);
    void slot_storeTransY(int y);
    void slot_storeAngle(int theta);
    void slot_storeScaleX(int x);
    void slot_storeScaleY(int y);

    void slot_setGeometry(QTreeWidgetItem *current);

    void slot_addTransNode();
    void slot_addRotateNode();
    void slot_addScaleNode();

signals:
    void sendRootNode(Node *root);

};


#endif // MYGL_H
