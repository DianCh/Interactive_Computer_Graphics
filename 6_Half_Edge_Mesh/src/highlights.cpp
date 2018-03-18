#include "highlights.h"
#include <iostream>

static const int VERTEX_COUNT = 1;
static const int HALFEDGE_COUNT = 2;

DrawableVertex::DrawableVertex(GLWidget277 *context) : Drawable(context), v(nullptr)
{}

void DrawableVertex::setNewVertex(Vertex *newV)
{
    destroy();
    v = newV;
    create();
}

GLenum DrawableVertex::drawMode()
{
    return GL_POINTS;
}

void DrawableVertex::create()
{
    // Prepare the buffer data
    std::vector<glm::vec4> pos {v->getPosition()};
    std::vector<GLuint> idx {0};
    std::vector<glm::vec4> col {glm::vec4(1, 1, 1, 1)};
    std::vector<glm::ivec2> IDs {v->getJointIDs()};         // New feature in hw8
    std::vector<glm::vec2> weights {v->getJointWeights()};  // New feature in hw8

    count = VERTEX_COUNT;

    // Bind and send data to buffer
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTEX_COUNT * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    generateIDs();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIDs);
    mp_context->glBufferData(GL_ARRAY_BUFFER, IDs.size() * sizeof(glm::ivec2), IDs.data(), GL_STATIC_DRAW);

    generateWeights();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    mp_context->glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec2), weights.data(), GL_STATIC_DRAW);

}

Vertex* DrawableVertex::getVertex()
{
    return v;
}

DrawableFace::DrawableFace(GLWidget277 *context) : Drawable(context), f(nullptr)
{}

void DrawableFace::setNewFace(Face *newF)
{
    destroy();
    f = newF;
    create();
}

GLenum DrawableFace::drawMode()
{
    return GL_TRIANGLES;
}

void DrawableFace::create()
{
    // The vector of data to send to the buffer
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;

    std::vector<glm::ivec2> IDs;        // New feature in hw8
    std::vector<glm::vec2> weights;     // New feature in hw8

    // Compute the opposite color of this face
    glm::vec4 oppositeCol = glm::vec4(1, 1, 1, 1) - f->getColor();
    oppositeCol[3] = 1;

    // Start from an edge of this face and traverse all the vertices
    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {

        // Prepare the vertices for computing the per-vertex normal
        Vertex *lastVertex = (thisEdge->getSymHalfEdge())->getVertex();
        Vertex *thisVertex = thisEdge->getVertex();
        Vertex *nextVertex = (thisEdge->getNextHalfEdge())->getVertex();

        // Compute the normal at this vertex using cross product
        glm::vec4 thisNormal = glm::vec4(glm::normalize(glm::cross(glm::vec3(thisVertex->getPosition() - lastVertex->getPosition()),
                                                                   glm::vec3(nextVertex->getPosition() - thisVertex->getPosition()))), 0);

        // Populate the data
        pos.push_back(thisVertex->getPosition());
        col.push_back(oppositeCol);
        nor.push_back(thisNormal);
        IDs.push_back(thisVertex->getJointIDs());          // New feature in hw8
        weights.push_back(thisVertex->getJointWeights());  // New feature in hw8

        // March to the next edge
        thisEdge = thisEdge->getNextHalfEdge();

        // Find if the next edge is again the starter. If so then the traversal is done
        if (thisEdge == f->getOneHalfEdge())
        {
            break;
        }
    }

    // Populate the indices in a way of fan triangulation
    for (int i = 0; i < pos.size() - 2; ++i)
    {
        idx.push_back(GLuint(0));
        idx.push_back(GLuint(i + 1));
        idx.push_back(GLuint(i + 2));
    }

    // Set up VBOs for indices, positions, normals and colors
    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    generateIDs();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIDs);
    mp_context->glBufferData(GL_ARRAY_BUFFER, IDs.size() * sizeof(glm::ivec2), IDs.data(), GL_STATIC_DRAW);

    generateWeights();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    mp_context->glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec2), weights.data(), GL_STATIC_DRAW);

}

Face* DrawableFace::getFace()
{
    return f;
}

DrawableHalfEdge::DrawableHalfEdge(GLWidget277 *context) : Drawable(context), he(nullptr)
{}

void DrawableHalfEdge::setNewHalfEdge(HalfEdge *newHE)
{
    destroy();
    he = newHE;
    create();
}

GLenum DrawableHalfEdge::drawMode()
{
    return GL_LINES;
}

void DrawableHalfEdge::create()
{
    // Prepare the buffer data
    Vertex *thisVertex = he->getVertex();
    Vertex *lastVertex = (he->getSymHalfEdge())->getVertex();

    std::vector<GLuint> idx {0, 1};
    std::vector<glm::vec4> pos {thisVertex->getPosition(), lastVertex->getPosition()};
    std::vector<glm::vec4> col {glm::vec4(0.92, 0.89, 0.41, 1), glm::vec4(1, 0, 0, 1)};
    std::vector<glm::ivec2> IDs {thisVertex->getJointIDs(), lastVertex->getJointIDs()};             // New feature in hw8
    std::vector<glm::vec2> weights {thisVertex->getJointWeights(), lastVertex->getJointWeights()};  // New feature in hw8

    count = HALFEDGE_COUNT;

    // Bind and send data
    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    generateIDs();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIDs);
    mp_context->glBufferData(GL_ARRAY_BUFFER, IDs.size() * sizeof(glm::ivec2), IDs.data(), GL_STATIC_DRAW);

    generateWeights();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    mp_context->glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec2), weights.data(), GL_STATIC_DRAW);

}

HalfEdge* DrawableHalfEdge::getHalfEdge()
{
    return he;
}
