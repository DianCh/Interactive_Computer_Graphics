#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include "camera.h"
#include "mesh.h"
#include "highlights.h"
#include "skeleton.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    Cylinder m_geomCylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere m_geomSphere;// The instance of a unit sphere we can use to render any sphere

    Mesh m_mesh;                    // The current mesh
    DrawableFace m_face;            // The drawable current face
    DrawableHalfEdge m_halfedge;    // The drawable current half edge
    DrawableVertex m_vertex;        // The drawable current vertex

    double distance;                // Container for the distance used for mesh's extruding and central dividing

    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkinWithFlat;    // New feature in hw8; a shader program that allows using skeletons and
                                         // applies flat shadow

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    Skeleton m_skeleton;            // New feature in hw8; a drawable skeleton
    Joint *m_joint;                 // New feature in hw8; the current selected joint
    double translate;               // New feature in hw8; container for the distance to translate
    double angle;                   // New feature in hw8; container for the angle to rotate
    glm::mat4 bindMatrices[50];     // New feature in hw8; containers for the bind matrices. Hard coded spaces for
                                             // 50 matrices
    glm::mat4 transMatrices[50];    // New feature in hw8; containers for the trans matrices. Hard coded spaces for
                                             // 50 matrices
    bool isBound;                   // New feature in hw8; a flag to record if a the skeleton is bound to the mesh

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void updateBindMatrices(std::vector<Joint*> &joints);   // New feature in hw8; update all the bind matrices at binding
    void updateTransMatrices(std::vector<Joint*> &joints);  // New feature in hw8; update all the trans matrices whenever
                                                            // the joints undergo a transformation

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sendVertices(std::vector<Vertex*> vertices);       // Used for setting up the QList
    void sendFaces(std::vector<Face*> faces);               // Used for setting up the QList
    void sendHalfEdges(std::vector<HalfEdge*> halfedges);   // Used for setting up the QList

    void sendAVertex(Vertex *v);                            // Used for setting up the spinboxes
    void sendAFace(Face *f);                                // Used for setting up the spinboxes

    void sendRootJoint(Joint *root);                        // New feature in hw8; send root joint to set up the tree widget

public slots:
    void slot_setCurrentVertex(int index);                  // Set the current vertex
    void slot_setCurrentHalfEdge(int index);                // Set the current half edge
    void slot_setCurrentFace(int index);                    // Set the current face

    void slot_addANewVertex(bool);                          // Receive the signal from button to add a new vertex
    void slot_triangulateAFace(bool);                       // Receive the signal from button to triangulate a face

    void slot_setVertexPositionX(double x);                 // Set the x coordinate of a vertex
    void slot_setVertexPositionY(double y);                 // Set the y coordinate of a vertex
    void slot_setVertexPositionZ(double z);                 // Set the z coordinate of a vertex

    void slot_setFaceColorR(double r);                      // Set the r component of a face color
    void slot_setFaceColorG(double g);                      // Set the g component of a face color
    void slot_setFaceColorB(double b);                      // Set the b component of a face color

    void slot_subdivideCurrentMesh(bool);                   // New feature in hw7; receive the signal from button to
                                                            // perform Catmull-Clark subdivision on current mesh
    void slot_extrudeAFace(bool);                           // New feature in hw7; receive the signal from button to
                                                            // perform extruding on a face
    void slot_centralDivideAFace(bool);                     // New feature in hw7 (EC); receive the signal from button
                                                            // perform central divide on a face
    void slot_loadOBJ(bool);                                // New feature in hw7; receive the signal from button to
                                                            // load an OBJ file to form the mesh
    void slot_receiveDistance(double d);                    // New feature in hw7; set the desired distance given from
                                                            // the GUI
    void slot_loadJSON(bool);                               // New feature in hw8; receive the signal from button to
                                                            // load an JSON file to set up the skeleton
    void slot_translateX(bool);                             // New feature in hw8; receive the signal to translate a joint
                                                            // along positive X direction by some amount
    void slot_translateY(bool);                             // New feature in hw8; receive the signal to translate a joint
                                                            // along positive Y direction by some amount
    void slot_translateZ(bool);                             // New feature in hw8; receive the signal to translate a joint
                                                            // along positive Z direction by some amount
    void slot_rotateX(bool);                                // New feature in hw8; receive the signal to rotate a joint
                                                            // around X axis by some angle
    void slot_rotateY(bool);                                // New feature in hw8; receive the signal to rotate a joint
                                                            // around Y axis by some angle
    void slot_rotateZ(bool);                                // New feature in hw8; receive the signal to rotate a joint
                                                            // around Z axis by some angle
    void slot_receiveTranslateDistance(double d);           // New feature in hw8; set the desired translate distance from
                                                            // the GUI
    void slot_receiveRotateAngle(double angle);             // New feature in hw8; set the desired rotate angle from the GUI
    void slot_setCurrentJoint(QTreeWidgetItem* item, int column);
                                                            // New feature in hw8; set the current selected joint
    void slot_bindAndSkin(bool);                            // New feature in hw8; receive the signal to bind the current
                                                            // skeleton to the mesh and perform skinning

};


#endif // MYGL_H
