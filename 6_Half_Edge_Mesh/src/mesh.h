#pragma once

#include "drawable.h"
#include "primitives.h"
#include "skeleton.h"

class Mesh : public Drawable
{
private:
    std::vector<Vertex*> vertices;          // A vector of vertex pointers
    std::vector<Face*> faces;               // A vector of face pointers
    std::vector<HalfEdge*> halfedges;       // A vector of half edge pointers

public:
    Mesh(GLWidget277*);
    ~Mesh();

    void setupCube();       // Setup the mesh data of the required cube
    void create();          // Construct and send data to the vbo

    Vertex* getAVertex(int idx);        // Return a vertex pointer at specified index; used for selection from GUI
    Face* getAFace(int idx);            // Return a face pointer at specifed index; used for selection from GUI
    HalfEdge* getAHalfEdge(int idx);    // Return a half edge pointer at specified index; used for selection from GUI

    std::vector<Vertex*> getVertices();     // Return the vector of vertex pointers; used for setup QList
    std::vector<Face*> getFaces();          // Return the vector of face pointers; used for setup QList
    std::vector<HalfEdge*> getHalfEdges();  // Return the vector of half edge pointers; used for setup QList

    Vertex* addANewVertex(HalfEdge *selectedHE);   // Add a new vertex at the middle of selected half edge
                                                   // Its return type has been modified for hw7 to return the new vertex
    void triangulateAFace(Face *f);                // Triangulate a given face

    void catmullclarkSubdivision();         // New feature added in hw7; perform one iteration of CC subdivision on
                                            // current mesh
    void quadrangulateAFace(Face *f, Vertex *centroid);
                                            // New feature added in hw7; perform quadrangulation on a given face
    void smoothVertices(std::vector<Vertex*> oldVertices);
                                            // New feature added in hw7; perform smoothing on the old vertices of a face

    void extrudeAFace(Face *f, float distance);     // New feature added in hw7; perform extruding on a face by some distance

    void centralDivideAFace(Face *f, float distance);   // New feature added in hw7 (EC); perform central divide on a face

    void clearPrimitives();                 // New feature added in hw7; delete all primitives of current mesh in preparation
                                            // for new ones
    void setupOBJ(QString filename);        // New feature added in hw7; set up primitives and all the data structure given an
                                            // OBJ file
    void skinMesh(Skeleton &sklt);          // New feature added in hw8; given a set of joints (a skeleton), determine the influences
                                            // for each vertex

};
