#pragma once
#include "la.h"

#include <QListWidgetItem>


class Face;
class HalfEdge;

class Vertex : public QListWidgetItem
{
private:
    glm::vec4 position;          // coordinates of this vertex
    HalfEdge *halfEdge;          // pointer to one of the half edges that points to this vertex
    int id;                      // identifier of this vertex

    glm::ivec2 jointIDs;         // New feature added in hw8; the IDs of the joints influencing this vertex
    glm::vec2 jointWeights;      // New feature added in hw8; the corresponding weights of the influence

    static int lastAssigned;     // keep track of the last id assigned to a Vertex

public:
    Vertex();
    Vertex(const glm::vec4 &p);
    Vertex(const glm::vec4 &p, HalfEdge *e);
    ~Vertex();

    void setPosition(const glm::vec4 &p);
    void setHalfEdge(HalfEdge *e);
    void setJointIDs(glm::ivec2 &IDs);          // New feature added in hw8; set the IDs of the two joints
                                                // influencing this vertex
    void setJointWeights(glm::vec2 &weights);   // New feature added in hw8; set the weights of the two
                                                // joints influencing this vertex

    glm::vec4 getPosition();
    HalfEdge* getOneHalfEdge();
    glm::ivec2 getJointIDs() const;             // New feature added in hw8
    glm::vec2 getJointWeights() const;          // New feature added in hw8

};

class Face : public QListWidgetItem
{
private:
    HalfEdge *oneHalfEdge;       // pointer to one of the half edge lying on this face
    glm::vec4 color;             // RGB color of this face
    int id;                      // identifier of this Face

    static int lastAssigned;     // keep track of the last id assigned to a Face

public:
    Face();
    Face(const glm::vec4 &c);
    Face(HalfEdge *e, const glm::vec4 &c);
    ~Face();

    void setOneHalfEdge(HalfEdge *oe);
    void setColor(const glm::vec4 &c);

    HalfEdge* getOneHalfEdge();
    glm::vec4 getColor();

    bool needTriangulation();
    int getID();

};

class HalfEdge : public QListWidgetItem
{
private:
    HalfEdge *nextHalfEdge;     // pointer to the next half edge
    HalfEdge *sym;              // pointer to the sym half edge
    Face *face;                 // pointer to the face to the half edge's left
    Vertex *vertex;             // pointer to the vertex between this and the next half edge
    int id;                     // identifier of this half edge

    static int lastAssigned;    // keep track of the last id assigned to a HalfEdge

public:
    HalfEdge();
    HalfEdge(HalfEdge *e, HalfEdge *s, Face *f, Vertex *v);
    ~HalfEdge();

    void setNextHalfEdge(HalfEdge *ne);
    void setSymHalfEdge(HalfEdge *s);
    void setFace(Face *f);
    void setVertex(Vertex *v);

    HalfEdge* getNextHalfEdge();
    HalfEdge* getSymHalfEdge();
    Face* getFace();
    Vertex* getVertex();

    int getID();
};
