#include "primitives.h"
#define EPSILON 0.000001

int Vertex::lastAssigned = -1;
int Face::lastAssigned = -1;
int HalfEdge::lastAssigned = -1;


Vertex::Vertex() : position(glm::vec4(0, 0, 0, 1)),
                   halfEdge(nullptr),
                   jointIDs(glm::ivec2(0, 0)), jointWeights(glm::vec2(1, 0)),
                   id(Vertex::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Vertex::Vertex(const glm::vec4 &p) : position(p),
                                     halfEdge(nullptr),
                                     jointIDs(glm::ivec2(0, 0)), jointWeights(glm::vec2(1, 0)),
                                     id(Vertex::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Vertex::Vertex(const glm::vec4 &p, HalfEdge *e) : position(p),
                                                 halfEdge(e),
                                                 jointIDs(glm::ivec2(0, 0)), jointWeights(glm::vec2(1, 0)),
                                                 id(Vertex::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Vertex::~Vertex()
{
    lastAssigned -= 1;
}

void Vertex::setPosition(const glm::vec4 &p)
{
    position = p;
}

void Vertex::setHalfEdge(HalfEdge *e)
{
    halfEdge = e;
}

void Vertex::setJointIDs(glm::ivec2 &IDs)
{
    jointIDs = IDs;
}

void Vertex::setJointWeights(glm::vec2 &weights)
{
    jointWeights = weights;
}

glm::vec4 Vertex::getPosition()
{
    return position;
}

HalfEdge* Vertex::getOneHalfEdge()
{
    return halfEdge;
}

glm::ivec2 Vertex::getJointIDs() const
{
    return jointIDs;
}

glm::vec2 Vertex::getJointWeights() const
{
    return jointWeights;
}

Face::Face() : oneHalfEdge(nullptr),
               color(glm::vec4(1, 1, 1, 1)),
               id(Face::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Face::Face(const glm::vec4 &c) : oneHalfEdge(nullptr),
                                 color(c),
                                 id(Face::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Face::Face(HalfEdge *e, const glm::vec4 &c) : oneHalfEdge(e),
                                              color(c),
                                              id(Face::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

Face::~Face()
{
    lastAssigned -= 1;
}

void Face::setOneHalfEdge(HalfEdge *oe)
{
    oneHalfEdge = oe;
}

void Face::setColor(const glm::vec4 &c)
{
    color = c;
}

HalfEdge* Face::getOneHalfEdge()
{
    return oneHalfEdge;
}

glm::vec4 Face::getColor()
{
    return color;
}

int Face::getID()
{
    return id;
}

bool Face::needTriangulation()
{
    // Default answer is no
    bool needed = false;

    // Find the first three points from the known half edge
    HalfEdge *he0 = oneHalfEdge;
    glm::vec4 point0 = he0->getVertex()->getPosition();

    HalfEdge *he1 = he0->getNextHalfEdge();
    glm::vec4 point1 = he1->getVertex()->getPosition();

    HalfEdge *he2 = he1->getNextHalfEdge();
    glm::vec4 point2 = he2->getVertex()->getPosition();

    // From highschool math we know that any three points can form a plane,
    // whose equation can have the form: x + Ay + Bz + C = 0. Now solve for
    // A, B and C
    glm::vec3 x(point0.x, point1.x, point2.x);
    glm::vec3 y(point0.y, point1.y, point2.y);
    glm::vec3 z(point0.z, point1.z, point2.z);
    glm::vec3 ones(1, 1, 1);

    glm::mat3 matrix(y, z, ones);

    glm::vec3 valueABC = glm::inverse(matrix) * (-x);

    glm::vec4 coefficients = glm::vec4(1, valueABC);

    // Now traverse the remaining vertices to see if they conform this equation
    for (HalfEdge *thisEdge = he2->getNextHalfEdge(); ;)
    {
        // Extract the coordinates of this vertex
        glm::vec4 thisPoint = (thisEdge->getVertex())->getPosition();

        // Plug in the coordinates to the equation. If the value differs from 0
        // then this face should be triangulated
        if (fabsf(glm::dot(coefficients, thisPoint)) > EPSILON)
        {
            needed = true;
            break;
        }

        // March to the next edge
        thisEdge = thisEdge->getNextHalfEdge();

        // If this edge is the same as the starter edge then the traversal is done
        if (thisEdge == oneHalfEdge)
        {
            break;
        }

    }

    return needed;
}

HalfEdge::HalfEdge() : nextHalfEdge(nullptr),
                       sym(nullptr),
                       face(nullptr),
                       vertex(nullptr),
                       id(HalfEdge::lastAssigned + 1)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

HalfEdge::HalfEdge(HalfEdge *e, HalfEdge *s, Face *f, Vertex *v) :
    nextHalfEdge(e), sym(s), face(f), vertex(v)
{
    QString name = QString::number(id);
    setText(name);
    lastAssigned += 1;
}

HalfEdge::~HalfEdge()
{
    lastAssigned -= 1;
}

void HalfEdge::setNextHalfEdge(HalfEdge *ne)
{
    nextHalfEdge = ne;
}

void HalfEdge::setSymHalfEdge(HalfEdge *s)
{
    sym = s;
}

void HalfEdge::setFace(Face *f)
{
    face = f;
}

void HalfEdge::setVertex(Vertex *v)
{
    vertex = v;
}

HalfEdge* HalfEdge::getNextHalfEdge()
{
    return nextHalfEdge;
}

HalfEdge* HalfEdge::getSymHalfEdge()
{
    return sym;
}

Face* HalfEdge::getFace()
{
    return face;
}

Vertex* HalfEdge::getVertex()
{
    return vertex;
}

int HalfEdge::getID()
{
    return id;
}
