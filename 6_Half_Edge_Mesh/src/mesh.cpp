#include "mesh.h"
#include "primitives.h"
#include <iostream>
#include <algorithm>
#include <QTextStream>
#include <QFile>
#include <random>

#define EPSILON 0.00001

Mesh::Mesh(GLWidget277* context) : Drawable(context),
                                   vertices(),
                                   faces(),
                                   halfedges() {}

Mesh::~Mesh()
{
    for (Vertex *v : vertices)
    {
        delete v;
    }

    for (Face *f : faces)
    {
        delete f;
    }

    for (HalfEdge *he : halfedges)
    {
        delete he;
    }
}

void Mesh::setupCube()
{
    std::vector<glm::vec4> pos {glm::vec4 (0.5, 0.5, 0.5, 1),
                                glm::vec4 (-0.5, 0.5, 0.5, 1),
                                glm::vec4 (-0.5, -0.5, 0.5, 1),
                                glm::vec4 (0.5, -0.5, 0.5, 1),

                                glm::vec4 (0.5, 0.5, -0.5, 1),
                                glm::vec4 (0.5, -0.5, -0.5, 1),
                                glm::vec4 (-0.5, -0.5, -0.5, 1),
                                glm::vec4 (-0.5, 0.5, -0.5, 1)};

    std::vector<glm::vec4> col {glm::vec4 (1, 0, 0, 1),
                                glm::vec4 (0, 1, 0, 1),
                                glm::vec4 (0, 0, 1, 1),
                                glm::vec4 (1, 1, 0, 1),
                                glm::vec4 (1, 0, 1, 1),
                                glm::vec4 (0, 1, 1, 1)};

    // Create 8 vertices with positions
    for (int i = 0; i < 8; ++i)
    {
        Vertex *v = new Vertex(pos[i]);
        vertices.push_back(v);
    }

    // Create 6 faces
    for (int i = 0; i < 6; ++i)
    {
        Face *f = new Face(col[i]);
        faces.push_back(f);
    }

    // Create 24 halfedges and set up their faces
    for (int i = 0; i < 24; ++i)
    {
        HalfEdge *he = new HalfEdge();
        halfedges.push_back(he);
        halfedges[i]->setFace(faces[i / 4]);
    }

    // Set up the next half edges. After that the half edges are like 6 separate
    // square wire frames.
    for (int i = 0; i < 24; ++i)
    {
        if (i % 4 == 3)
        {
            halfedges[i]->setNextHalfEdge(halfedges[i - 3]);
        }
        else
        {
            halfedges[i]->setNextHalfEdge(halfedges[i + 1]);
        }
    }

    // Set up the vertices and syms for half edges
    halfedges[0]->setVertex(vertices[1]);
    halfedges[0]->setSymHalfEdge(halfedges[11]);

    halfedges[1]->setVertex(vertices[2]);
    halfedges[1]->setSymHalfEdge(halfedges[15]);

    halfedges[2]->setVertex(vertices[3]);
    halfedges[2]->setSymHalfEdge(halfedges[19]);

    halfedges[3]->setVertex(vertices[0]);
    halfedges[3]->setSymHalfEdge(halfedges[23]);

    halfedges[4]->setVertex(vertices[5]);
    halfedges[4]->setSymHalfEdge(halfedges[21]);

    halfedges[5]->setVertex(vertices[6]);
    halfedges[5]->setSymHalfEdge(halfedges[17]);

    halfedges[6]->setVertex(vertices[7]);
    halfedges[6]->setSymHalfEdge(halfedges[13]);

    halfedges[7]->setVertex(vertices[4]);
    halfedges[7]->setSymHalfEdge(halfedges[9]);

    halfedges[8]->setVertex(vertices[4]);
    halfedges[8]->setSymHalfEdge(halfedges[22]);

    halfedges[9]->setVertex(vertices[7]);
    halfedges[9]->setSymHalfEdge(halfedges[7]);

    halfedges[10]->setVertex(vertices[1]);
    halfedges[10]->setSymHalfEdge(halfedges[12]);

    halfedges[11]->setVertex(vertices[0]);
    halfedges[11]->setSymHalfEdge(halfedges[0]);

    halfedges[12]->setVertex(vertices[7]);
    halfedges[12]->setSymHalfEdge(halfedges[10]);

    halfedges[13]->setVertex(vertices[6]);
    halfedges[13]->setSymHalfEdge(halfedges[6]);

    halfedges[14]->setVertex(vertices[2]);
    halfedges[14]->setSymHalfEdge(halfedges[16]);

    halfedges[15]->setVertex(vertices[1]);
    halfedges[15]->setSymHalfEdge(halfedges[1]);

    halfedges[16]->setVertex(vertices[6]);
    halfedges[16]->setSymHalfEdge(halfedges[14]);

    halfedges[17]->setVertex(vertices[5]);
    halfedges[17]->setSymHalfEdge(halfedges[5]);

    halfedges[18]->setVertex(vertices[3]);
    halfedges[18]->setSymHalfEdge(halfedges[20]);

    halfedges[19]->setVertex(vertices[2]);
    halfedges[19]->setSymHalfEdge(halfedges[2]);

    halfedges[20]->setVertex(vertices[5]);
    halfedges[20]->setSymHalfEdge(halfedges[18]);

    halfedges[21]->setVertex(vertices[4]);
    halfedges[21]->setSymHalfEdge(halfedges[4]);

    halfedges[22]->setVertex(vertices[0]);
    halfedges[22]->setSymHalfEdge(halfedges[8]);

    halfedges[23]->setVertex(vertices[3]);
    halfedges[23]->setSymHalfEdge(halfedges[3]);

    for (int i = 0; i < 4; ++i)
    {
        vertices[i]->setHalfEdge(halfedges[(i + 3) % 4]);
        vertices[i + 4]->setHalfEdge(halfedges[(i + 3) % 4 + 4]);

    }

    for (int i = 0; i < 6; ++i)
    {
        faces[i]->setOneHalfEdge(halfedges[i * 4]);
    }

}

void Mesh::create()
{
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;

    std::vector<glm::ivec2> IDs;        // New feature in hw8
    std::vector<glm::vec2> weights;     // New feature in hw8

    int numVertexOfAll = 0;

    for (Face *f : faces)
    {
        // For this face, we try to find a valid norm. The two vectors taking cross product
        // to get this norm should not be parallel. This is to avoid the situation in
        // which we add vertices successively to the same half edge and the starting point
        // may have an invalid norm
        glm::vec4 oneValidNorm;

        // Traverse all the half edges to compute the norms until we find a valid one. We
        // use this same valid norm for each vertex belonging to this face
        for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
        {
            // Prepare the vertices for computing the normal, and use this for every vertex
            Vertex *lastVertex = (thisEdge->getSymHalfEdge())->getVertex();
            Vertex *thisVertex = thisEdge->getVertex();
            Vertex *nextVertex = (thisEdge->getNextHalfEdge())->getVertex();

            // Get the vectors for computing cross product
            glm::vec3 vectorA = glm::normalize(glm::vec3(thisVertex->getPosition() - lastVertex->getPosition()));
            glm::vec3 vectorB = glm::normalize(glm::vec3(nextVertex->getPosition() - thisVertex->getPosition()));

            // If the dot product are close enough to 1, the norm is treated as invalid.
            // Once a valid one is found, we end the search and break
            if (fabsf(glm::dot(vectorA, vectorB) - 1) > EPSILON)
            {
                oneValidNorm = glm::vec4(glm::cross(vectorA, vectorB), 0);

                break;
            }

            // March to the next edge. The loop is guaranteed to be a finite loop since any
            // closed face should have at least three vertices with valid norm
            thisEdge = thisEdge->getNextHalfEdge();
        }

        // Initialize the number of vertices of this face
        int numVertexOfThisFace = 0;

        // Start from an edge of this face and traverse all the vertices
        for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
        {

            // Populate the data
            pos.push_back((thisEdge->getVertex())->getPosition());
            col.push_back(f->getColor());
            nor.push_back(oneValidNorm);
            IDs.push_back((thisEdge->getVertex())->getJointIDs());          // New feature in hw8
            weights.push_back((thisEdge->getVertex())->getJointWeights());  // New feature in hw8

            // March to the next edge
            thisEdge = thisEdge->getNextHalfEdge();

            numVertexOfThisFace += 1;

            // Find if the next edge is again the starter. If so then the traversal is done
            if (thisEdge == f->getOneHalfEdge())
            {
                break;
            }
        }

        // Populate the indices in a way of fan triangulation
        for (int i = numVertexOfAll; i < numVertexOfAll + numVertexOfThisFace - 2; ++i)
        {
            idx.push_back(GLuint(numVertexOfAll));
            idx.push_back(GLuint(i + 1));
            idx.push_back(GLuint(i + 2));
        }

        numVertexOfAll += numVertexOfThisFace;
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

Vertex* Mesh::getAVertex(int idx)
{
    return vertices[idx];
}

Face* Mesh::getAFace(int idx)
{
    return faces[idx];
}

HalfEdge* Mesh::getAHalfEdge(int idx)
{
    return halfedges[idx];
}

std::vector<Vertex*> Mesh::getVertices()
{
    return vertices;
}

std::vector<Face*> Mesh::getFaces()
{
    return faces;
}

std::vector<HalfEdge*> Mesh::getHalfEdges()
{
    return halfedges;
}

Vertex* Mesh::addANewVertex(HalfEdge *selectedHE)
{
    // Get the end points of the selected half edge
    Vertex *start = selectedHE->getVertex();
    Vertex *end = (selectedHE->getSymHalfEdge())->getVertex();

    // Compute the position for the middle point
    glm::vec4 middlePosition = 0.5f * (start->getPosition() + end->getPosition());

    // Construct a new vertex and add it to the mesh
    Vertex *v = new Vertex(middlePosition);
    vertices.push_back(v);

    // Construct a new half edge h1 and set up its properties
    HalfEdge *h1 = new HalfEdge();
    halfedges.push_back(h1);
    h1->setNextHalfEdge(selectedHE->getNextHalfEdge());
    h1->setFace(selectedHE->getFace());
    h1->setVertex(start);

    // Construct a new half edge h2 and set up its properties
    HalfEdge *h2 = new HalfEdge();
    halfedges.push_back(h2);
    h2->setNextHalfEdge((selectedHE->getSymHalfEdge())->getNextHalfEdge());
    h2->setFace((selectedHE->getSymHalfEdge())->getFace());
    h2->setVertex(end);

    // Change the properties of old elements
    start->setHalfEdge(h1);
    end->setHalfEdge(h2);

    selectedHE->setNextHalfEdge(h1);
    (selectedHE->getSymHalfEdge())->setNextHalfEdge(h2);

    selectedHE->setVertex(v);
    (selectedHE->getSymHalfEdge())->setVertex(v);
    v->setHalfEdge(selectedHE);

    // Set up the sym half edges
    h1->setSymHalfEdge(selectedHE->getSymHalfEdge());
    (selectedHE->getSymHalfEdge())->setSymHalfEdge(h1);

    h2->setSymHalfEdge(selectedHE);
    selectedHE->setSymHalfEdge(h2);

    // return the pointer to the new added vertex
    return v;
}

void Mesh::triangulateAFace(Face *f)
{
    // Record the origin for the fan triangulation
    Vertex *origin = (f->getOneHalfEdge())->getVertex();

    // Construct a container of pointers for convenience
    std::vector<HalfEdge*> hes;

    // Populate this container
    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        hes.push_back(thisEdge);

        thisEdge = thisEdge->getNextHalfEdge();

        if (thisEdge == f->getOneHalfEdge())
        {
            break;
        }
    }

    // Record the number of sides of this face
    int numSides = hes.size();

    HalfEdge *nextTemp = hes[1];
    HalfEdge *thisEdge = nullptr;

    for (int i = 2; i <= numSides - 2; ++i)
    {
        thisEdge = hes[i];

        // Generate a new face
        Face *newF = new Face();
        faces.push_back(newF);

        // Setups for the new face
        newF->setColor(f->getColor());
        newF->setOneHalfEdge(thisEdge);

        // Construct two new halfedges
        HalfEdge *h1 = new HalfEdge();
        HalfEdge *h2 = new HalfEdge();
        halfedges.push_back(h1);
        halfedges.push_back(h2);

        // Setups for new halfedges
        h1->setVertex(origin);
        h1->setNextHalfEdge(nextTemp);
        h1->setFace(newF);

        h2->setVertex(thisEdge->getVertex());
        h2->setNextHalfEdge(thisEdge->getNextHalfEdge());

        h1->setSymHalfEdge(h2);
        h2->setSymHalfEdge(h1);

        // Changes made to the previous edges
        nextTemp->setFace(newF);
        thisEdge->setFace(newF);
        thisEdge->setNextHalfEdge(h1);

        // Update "nextTemp" for the next triangulation
        nextTemp = h2;
    }

    // Do some clean up for the last triangle, which is the old face at the beginning
    halfedges[halfedges.size() - 1]->setFace(f);
    (f->getOneHalfEdge())->setNextHalfEdge(halfedges[halfedges.size() - 1]);

}

void Mesh::catmullclarkSubdivision()
{
    // Create a container of old vertices for use in step 4
    std::vector<Vertex*> oldVertices = vertices;

    /// Step 1: create centoids for each face
    // Create a map to store centroids for each face

    std::vector<Vertex*> faceToCentroid;

    for (Face *f : faces)
    {
        // Count the number of vertices of this face
        int vertexCount = 0;

        // Cumulate the position data to compute the average
        glm::vec4 centroidPosition(0, 0, 0, 0);

        // Traverse all the vertices and cumulate data
        for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
        {
            centroidPosition = centroidPosition + (thisEdge->getVertex())->getPosition();

            vertexCount += 1;

            thisEdge = thisEdge->getNextHalfEdge();

            if (thisEdge == f->getOneHalfEdge())
            {
                break;
            }
        }

        // Take average
        centroidPosition = centroidPosition / (float)vertexCount;

        // Generate a new vertex as the centroid
        Vertex *centroid = new Vertex(centroidPosition);

        vertices.push_back(centroid);

        faceToCentroid.push_back(centroid);
    }

    /// Step 2: Add midpoints
    // Make a copy of the whole half edge list
    std::vector<HalfEdge*> originalHEs = halfedges;

    // Create a 1/2 halfedge list to achieve splitting full edges
    std::vector<HalfEdge*> oneOfTwoHEs;

    for (HalfEdge *he : originalHEs)
    {
        // Check if this half edge‘s sym is present in the 1/2 list
        if (std::find(oneOfTwoHEs.begin(), oneOfTwoHEs.end(), he->getSymHalfEdge()) != oneOfTwoHEs.end())
        {
            // If so, then skip
            continue;
        }

        // If not, then add this half edge to the 1/2 list
        oneOfTwoHEs.push_back(he);
    }

    // Now that we have a 1/2 halfedge list, perform edge splitting on them
    for (HalfEdge *he : oneOfTwoHEs)
    {
        Vertex *newV = addANewVertex(he);

        Vertex *v1 = (he->getNextHalfEdge())->getVertex();
        Vertex *v2 = (he->getSymHalfEdge())->getVertex();
        Vertex *f1 = faceToCentroid[(he->getFace())->getID()];
        Vertex *f2 = faceToCentroid[((he->getSymHalfEdge())->getFace())->getID()];

        glm::vec4 midPosition = (v1->getPosition() + v2->getPosition() +
                                 f1->getPosition() + f2->getPosition()) / 4.0f;

        newV->setPosition(midPosition);
    }

    /// Step 3: Quadrangulate each face to set up new half edges and data structure
    std::vector<Face*> beforeQuad = faces;

    for (Face *f : beforeQuad)
    {
        // Construct a temporary container to hold the orignal half edges for this face.
        std::vector<HalfEdge*> oldHEsOfThisFace;
        for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
        {
            oldHEsOfThisFace.push_back(thisEdge);

            thisEdge = (thisEdge->getNextHalfEdge())->getNextHalfEdge();

            if (thisEdge == f->getOneHalfEdge())
            {
                break;
            }
        }

        quadrangulateAFace(f, faceToCentroid[f->getID()]);
    }

    /// Step 4: Smooth the original vertices
    smoothVertices(oldVertices);

}

void Mesh::quadrangulateAFace(Face *f, Vertex *centroid)
{
    // Create temporary containers for new half edges and new faces
    std::vector<HalfEdge*> fromCentroid;
    std::vector<HalfEdge*> toCentroid;
    std::vector<Face*> newFaces;

    // Create a temporary container for storing the old half edges in a ordered way
    std::vector<HalfEdge*> originalOrdered;

    // Retrieve the color of this face
    glm::vec4 thisColor = f->getColor();

    // Traverse the face's all old half edges
    // In our way of edge splitting, the face's edge is guaranteed to be an old half edge
    for (HalfEdge *he = f->getOneHalfEdge(); ;)
    {
        // Push this old half edge in the container
        originalOrdered.push_back(he);

        // If this is an original half edge, then it must point to the middle point in
        // our way of edge splitting
        HalfEdge *fromC = new HalfEdge();
        HalfEdge *toC = new HalfEdge();

        halfedges.push_back(fromC);
        halfedges.push_back(toC);

        fromCentroid.push_back(fromC);
        toCentroid.push_back(toC);

        fromC->setSymHalfEdge(toC);
        fromC->setVertex(he->getVertex());
        fromC->setNextHalfEdge(he->getNextHalfEdge());

        toC->setSymHalfEdge(fromC);
        toC->setVertex(centroid);

        // Create a new face and add it to the mesh structure
        Face *newF = new Face(thisColor);
        faces.push_back(newF);
        newFaces.push_back(newF);

        newF->setOneHalfEdge(fromC);

        fromC->setFace(newF);
        (fromC->getNextHalfEdge())->setFace(newF);
        ((fromC->getNextHalfEdge())->getNextHalfEdge())->setFace(newF);

        he = (he->getNextHalfEdge())->getNextHalfEdge();

        if ((he->getNextHalfEdge())->getNextHalfEdge() == f->getOneHalfEdge())
        {
            originalOrdered.push_back(he);
            break;
        }
    }

    // Treat the old face seperately
    HalfEdge *lastOldEdge = originalOrdered[originalOrdered.size() - 1];

    HalfEdge *fromC = new HalfEdge();
    HalfEdge *toC = new HalfEdge();

    halfedges.push_back(fromC);
    halfedges.push_back(toC);

    fromCentroid.push_back(fromC);
    toCentroid.push_back(toC);

    fromC->setSymHalfEdge(toC);
    fromC->setVertex(lastOldEdge->getVertex());
    fromC->setNextHalfEdge(lastOldEdge->getNextHalfEdge());

    toC->setSymHalfEdge(fromC);
    toC->setVertex(centroid);

    newFaces.push_back(f);

    f->setOneHalfEdge(fromC);

    fromC->setFace(f);
    (fromC->getNextHalfEdge())->setFace(f);
    ((fromC->getNextHalfEdge())->getNextHalfEdge())->setFace(f);

    // Record the number of new faces
    int num = newFaces.size();

    // Set up & update the remaining relationships for new & old elements
    for (int i = 0; i < num; ++i)
    {
        toCentroid[i]->setFace(newFaces[(i - 1 + num) % num]);
        toCentroid[i]->setNextHalfEdge(fromCentroid[(i - 1 + num) % num]);

        originalOrdered[i]->setNextHalfEdge(toCentroid[i]);
    }

    // The first created half edge pointing to the centroid
    centroid->setHalfEdge(toCentroid[0]);
}

void Mesh::smoothVertices(std::vector<Vertex*> oldVertices)
{
    // Traverse all the old vertices
    for (Vertex *oldV : oldVertices)
    {

        int numOfAdjacentEdges = 0;

        glm::vec4 sumE(0, 0, 0, 0);
        glm::vec4 sumF(0, 0, 0, 0);
        glm::vec4 oldPosition = oldV->getPosition();

        for (HalfEdge *adjacentHEs = oldV->getOneHalfEdge(); ;)
        {
            sumE = sumE + ((adjacentHEs->getSymHalfEdge())->getVertex())->getPosition();
            sumF = sumF + (((adjacentHEs->getNextHalfEdge())->getNextHalfEdge())->getVertex())->getPosition();

            numOfAdjacentEdges += 1;

            adjacentHEs = (adjacentHEs->getNextHalfEdge())->getSymHalfEdge();

            if (adjacentHEs == oldV->getOneHalfEdge())
            {
                break;
            }
        }

        float N = (float)numOfAdjacentEdges;

        // Compute the new position according to the smoothing rule
        glm::vec4 newPosition = (N - 2) / N * oldPosition +
                                sumE / (N * N) +
                                sumF / (N * N);

        oldV->setPosition(newPosition);

    }

}

void Mesh::extrudeAFace(Face *f, float distance)
{
    /// Step 1: find a valid norm of this face
    // Same as in create(), first try to find a valid norm of this face
    glm::vec4 oneValidNorm;

    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        // Prepare the vertices for computing the normal, and use this for every vertex
        Vertex *lastVertex = (thisEdge->getSymHalfEdge())->getVertex();
        Vertex *thisVertex = thisEdge->getVertex();
        Vertex *nextVertex = (thisEdge->getNextHalfEdge())->getVertex();

        // Get the vectors for computing cross product
        glm::vec3 vectorA = glm::normalize(glm::vec3(thisVertex->getPosition() - lastVertex->getPosition()));
        glm::vec3 vectorB = glm::normalize(glm::vec3(nextVertex->getPosition() - thisVertex->getPosition()));

        // If the dot product are close enough to 1, the norm is treated as invalid.
        // Once a valid one is found, we end the search and break
        if (fabsf(glm::dot(vectorA, vectorB) - 1) > EPSILON)
        {
            oneValidNorm = glm::vec4(glm::cross(vectorA, vectorB), 0);

            break;
        }

        // March to the next edge. The loop is guaranteed to be a finite loop since any
        // closed face should have at least three vertices with valid norm
        thisEdge = thisEdge->getNextHalfEdge();
    }

    /// Step 2: compute necessary information & create temporary containers
    // The displacement for new vertices
    glm::vec4 extrudePosition(glm::vec3(distance * oneValidNorm), 0);

    // By default the new side faces will be the same color as the extruded face
    glm::vec4 color = f->getColor();

    // Create temporary containers for new elements
    std::vector<Face*> newFaces;
    std::vector<HalfEdge*> newInnerSideHalfEdges;
    std::vector<HalfEdge*> newOuterSideHalfEdges;
    std::vector<HalfEdge*> newFrontSplitHalfEdges;
    std::vector<HalfEdge*> newBackSplitHalfEdges;
    std::vector<Vertex*> newVertices;

    /// Step 3: create new elements and set up & update relations
    int edgeCount = 0;

    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        Vertex *thisVertex = thisEdge->getVertex();

        glm::vec4 newPosition = thisVertex->getPosition() + extrudePosition;

        // Create a new vertex
        Vertex *v = new Vertex(newPosition);

        vertices.push_back(v);
        newVertices.push_back(v);

        // Create two new half edges from splitting, two new half edges forming a new side edge
        // and a new face
        HalfEdge *h1 = new HalfEdge();
        HalfEdge *h2 = new HalfEdge();
        HalfEdge *h3 = new HalfEdge();
        HalfEdge *h4 = new HalfEdge();

        Face *newF = new Face(color);

        halfedges.push_back(h1);
        halfedges.push_back(h2);
        halfedges.push_back(h3);
        halfedges.push_back(h4);
        faces.push_back(newF);

        newInnerSideHalfEdges.push_back(h1);
        newOuterSideHalfEdges.push_back(h2);
        newFrontSplitHalfEdges.push_back(h3);
        newBackSplitHalfEdges.push_back(h4);
        newFaces.push_back(newF);

        // Set up some relations for new elements
        v->setHalfEdge(thisEdge);

        h1->setVertex(v);
        h1->setNextHalfEdge(h3);
        h1->setSymHalfEdge(h2);
        h1->setFace(newF);

        h2->setVertex(thisVertex);
        h2->setSymHalfEdge(h1);

        h3->setSymHalfEdge(thisEdge);
        h3->setFace(newF);

        h4->setVertex(thisVertex);
        h4->setNextHalfEdge(h1);
        h4->setSymHalfEdge(thisEdge->getSymHalfEdge());
        h4->setFace(newF);

        newF->setOneHalfEdge(h1);

        // Update some relations for old elements
        thisEdge->setVertex(v);

        thisVertex->setHalfEdge(h4);

        (thisEdge->getSymHalfEdge())->setSymHalfEdge(h4);

        // Count up the edge by 1
        edgeCount += 1;

        thisEdge = thisEdge->getNextHalfEdge();

        if (thisEdge == f->getOneHalfEdge())
        {
            break;
        }
    }

    // Fix the rest of the relations
    for (int i = 0; i < edgeCount; ++i)
    {
        newOuterSideHalfEdges[i]->setFace(newFaces[(i + 1) % edgeCount]);
        newOuterSideHalfEdges[i]->setNextHalfEdge(newBackSplitHalfEdges[(i + 1) % edgeCount]);

        newFrontSplitHalfEdges[i]->setVertex(newVertices[(i - 1 + edgeCount) % edgeCount]);
        newFrontSplitHalfEdges[i]->setNextHalfEdge(newOuterSideHalfEdges[(i - 1 + edgeCount) % edgeCount]);

        (newFrontSplitHalfEdges[i]->getSymHalfEdge())->setSymHalfEdge(newFrontSplitHalfEdges[i]);
    }

}

void Mesh::centralDivideAFace(Face *f, float distance)
{
    /// Step 1: find a valid norm of this face
    // Same as in create(), first try to find a valid norm of this face
    glm::vec4 oneValidNorm;

    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        // Prepare the vertices for computing the normal, and use this for every vertex
        Vertex *lastVertex = (thisEdge->getSymHalfEdge())->getVertex();
        Vertex *thisVertex = thisEdge->getVertex();
        Vertex *nextVertex = (thisEdge->getNextHalfEdge())->getVertex();

        // Get the vectors for computing cross product
        glm::vec3 vectorA = glm::normalize(glm::vec3(thisVertex->getPosition() - lastVertex->getPosition()));
        glm::vec3 vectorB = glm::normalize(glm::vec3(nextVertex->getPosition() - thisVertex->getPosition()));

        // If the dot product are close enough to 1, the norm is treated as invalid.
        // Once a valid one is found, we end the search and break
        if (fabsf(glm::dot(vectorA, vectorB) - 1) > EPSILON)
        {
            oneValidNorm = glm::vec4(glm::cross(vectorA, vectorB), 0);

            break;
        }

        // March to the next edge. The loop is guaranteed to be a finite loop since any
        // closed face should have at least three vertices with valid norm
        thisEdge = thisEdge->getNextHalfEdge();
    }

    /// Step 2: compute the central point of this face by taking average of all vertices
    glm::vec4 centralPoint(0, 0, 0, 0);

    int vertexCount = 0;

    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        centralPoint = centralPoint + (thisEdge->getVertex())->getPosition();

        vertexCount += 1;

        thisEdge = thisEdge->getNextHalfEdge();

        if (thisEdge == f->getOneHalfEdge())
        {
            break;
        }
    }

    centralPoint = centralPoint / (float)vertexCount;

    glm::vec4 newPosition = centralPoint + distance * oneValidNorm;

    /// Step 3: create a new vertex
    Vertex *newV = new Vertex(newPosition);

    vertices.push_back(newV);

    /// Step 4: create some temporary containers for the new elements
    std::vector<HalfEdge*> newInnerHalfEdges;
    std::vector<HalfEdge*> newOuterHalfEdges;
    std::vector<Face*> newFaces;
    std::vector<HalfEdge*> oldHalfEdges;

    /// Step 5: traverse the half edges (except for the last one) of this face and create new half
    /// edges & faces
    // Use the same color as the current face to create new faces
    glm::vec4 color = f->getColor();

    for (HalfEdge *thisEdge = f->getOneHalfEdge(); ;)
    {
        // Create two new half edges, one new face
        HalfEdge *h1 = new HalfEdge();
        HalfEdge *h2 = new HalfEdge();

        Face *newF = new Face(color);

        halfedges.push_back(h1);
        halfedges.push_back(h2);
        faces.push_back(newF);

        newInnerHalfEdges.push_back(h1);
        newOuterHalfEdges.push_back(h2);
        newFaces.push_back(newF);
        oldHalfEdges.push_back(thisEdge);

        // Set up some relations for the new elements
        h1->setVertex(newV);
        h1->setSymHalfEdge(h2);
        h1->setFace(newF);

        h2->setVertex(thisEdge->getVertex());
        h2->setNextHalfEdge(thisEdge->getNextHalfEdge());
        h2->setSymHalfEdge(h1);

        newF->setOneHalfEdge(h1);

        // Update some relations for the old elements
        thisEdge->setFace(newF);

        thisEdge = thisEdge->getNextHalfEdge();

        // Stop at one half edge before the starter
        if (thisEdge->getNextHalfEdge() == f->getOneHalfEdge())
        {
            oldHalfEdges.push_back(thisEdge);
            break;
        }
    }

    // Treat the old face separately. We need only two new half edges
    HalfEdge *h1 = new HalfEdge();
    HalfEdge *h2 = new HalfEdge();
    HalfEdge *last = oldHalfEdges[oldHalfEdges.size() - 1];

    halfedges.push_back(h1);
    halfedges.push_back(h2);

    newInnerHalfEdges.push_back(h1);
    newOuterHalfEdges.push_back(h2);
    newFaces.push_back(f);

    h1->setVertex(newV);
    h1->setSymHalfEdge(h2);
    h1->setFace(f);

    h2->setVertex(last->getVertex());
    h2->setNextHalfEdge(last->getNextHalfEdge());
    h2->setSymHalfEdge(h1);

    f->setOneHalfEdge(h1);

    newV->setHalfEdge(h1);

    // Fix the rest of the relation
    for (int i = 0; i < vertexCount; ++i)
    {
        newInnerHalfEdges[i]->setNextHalfEdge(newOuterHalfEdges[(i - 1 + vertexCount) % vertexCount]);

        newOuterHalfEdges[i]->setFace(newFaces[(i + 1) % vertexCount]);

        oldHalfEdges[i]->setNextHalfEdge(newInnerHalfEdges[i]);
    }

}

void Mesh::clearPrimitives()
{
    for (Vertex *v : vertices)
    {
        delete v;
    }

    for (Face *f : faces)
    {
        delete f;
    }

    for (HalfEdge *he : halfedges)
    {
        delete he;
    }

    vertices.clear();
    faces.clear();
    halfedges.clear();
}

void Mesh::setupOBJ(QString filename)
{
    // Clear all the present primitives
    clearPrimitives();

    // Some set up for generating random numbers between 0.1 and 0.99, which is used
    // to set the face colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 0.99);

    // Store the indices of an edge's two end points; used for set up the sym half edges
    QMap<HalfEdge*, glm::ivec2> twoEnds;

    // Open and read a file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Could not open the OBJ file.");
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        // Read in one line of the file
        QString line = in.readLine();

        // Value holders for reading vertex positions
        char name[2];
        float x, y, z;

        // Value holder for reading face
        QStringList attributes;
        QStringList perVertex;

        if (line.indexOf(QString("v ")) == 0)
        {
            QTextStream stream(&line);
            stream >> name[0] >> x >> y >> z;
            glm::vec4 position(x, y, z, 1);

            // Create a new vertex
            Vertex *v = new Vertex(position);
            vertices.push_back(v);
        }
        if (line.indexOf(QString("f ")) == 0)
        {
            // Create a new face
            glm::vec4 randomColor((float)dis(gen), (float)dis(gen), (float)dis(gen), 1);

            Face *f = new Face(randomColor);
            faces.push_back(f);

            // Create some temporary containers for dealing with the relations
            std::vector<int> indices;
            std::vector<HalfEdge*> newHalfEdges;
            std::vector<Vertex*> newVertices;

            attributes = line.split(" ");

            // If the list has n components, then the face has n - 1 vertices
            int vertexCount = attributes.length() - 1;
            for (int i = 1; i <= vertexCount; ++i)
            {
                QString s = attributes.at(i);
                perVertex = s.split("/");

                QString indexString = perVertex.at(0);
                int index = indexString.toInt() - 1;
                indices.push_back(index);

                newVertices.push_back(vertices[index]);

                HalfEdge *h = new HalfEdge();

                halfedges.push_back(h);
                newHalfEdges.push_back(h);

                h->setFace(f);

            }

            f->setOneHalfEdge(newHalfEdges[0]);

            // Use the temporary containers to set up the remaining relations.
            for (int i = 0; i < vertexCount; ++i)
            {
                newHalfEdges[i]->setNextHalfEdge(newHalfEdges[(i + 1) % vertexCount]);
                newHalfEdges[i]->setVertex(newVertices[(i + 1) % vertexCount]);

                newVertices[(i + 1) % vertexCount]->setHalfEdge(newHalfEdges[i]);

                twoEnds[newHalfEdges[i]] = glm::ivec2(indices[i], indices[(i + 1) % vertexCount]);
            }
        }
    }

    // Now the final work is to set up the syms for the new half edges
    QMap<HalfEdge*, glm::ivec2>::iterator i;
    QMap<HalfEdge*, glm::ivec2>::iterator j;

    // Iterate over the QMap. Compare the two ends
    for (i = twoEnds.begin(); i != twoEnds.end(); ++i)
    {
        glm::ivec2 ends_1 = i.value();
        for (j = i + 1; j != twoEnds.end(); ++j)
        {
            glm::ivec2 ends_2 = j.value();

            // If the two ends match, set the sym relations
            if (ends_1[0] == ends_2[1] && ends_1[1] == ends_2[0])
            {
                i.key()->setSymHalfEdge(j.key());
                j.key()->setSymHalfEdge(i.key());

                // Erase the element for speed up
                twoEnds.erase(j);

                // If the erased element is the next one we should decrease i by 1
                // since the next element in consideration is move forward automatically
                // due to deletion. This can speed up the search drastically
                if (j == (i + 1))
                {
                    i -= 1;
                }
                // Terminate this round of search. This can speed up the search
                // drastically
                break;
            }
        }
    }

}

void Mesh::skinMesh(Skeleton &sklt)
{
    // Get all current joints
    std::vector<Joint*> joints = sklt.getJointSpheres();

    // Iterate over all vertices
    for (Vertex *v : vertices)
    {
        // Construct a map that stores the distance between each joint to this vertex
        QMap<float, int> distAndID;

        glm::vec4 vPos = v->getPosition();

        for (int i = 0; i < joints.size(); ++i)
        {
            glm::mat4 jTransform = joints[i]->getOverallTransformation();
            glm::vec4 jPos = jTransform[3];

            // Compute distance
            float distance = glm::l2Norm(glm::vec3(vPos - jPos));

            // Store the distance-index pair in the map
            distAndID[distance] = i;
        }

        // The first key is the smallest distance, its value being the corresponding index
        float distance_0 = distAndID.firstKey();
        int index_0 = distAndID.first();

        // Construct an iterator starting at the first element
        QMap<float, int>::iterator iter = distAndID.find(distAndID.firstKey());

        // Erase the first element
        distAndID.erase(iter);

        // Now the first key is the second smallest distance, its value being the corresponding index
        float distance_1 = distAndID.firstKey();
        int index_1 = distAndID.first();

        // Construct IDs and weights
        glm::ivec2 IDs(index_0, index_1);
        glm::vec2 weights(distance_1 / (distance_0 + distance_1),
                          distance_0 / (distance_0 + distance_1));

        // Set IDs and weights of this vertex
        v->setJointIDs(IDs);
        v->setJointWeights(weights);
    }
}
