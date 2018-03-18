#pragma once

#include "primitives.h"
#include "drawable.h"

class DrawableVertex : public Drawable
{
private:
    Vertex *v;      // The currently selected vertex
public:
    DrawableVertex(GLWidget277 *context);
    void setNewVertex(Vertex *newV);        // Set the new vertex
    void create();                          // Set up the vbo
    GLenum drawMode();                      // Overriden to return GL_POINTS

    Vertex* getVertex();                    // Return the current vertex pointer
};

class DrawableFace : public Drawable
{
private:
    Face *f;        // The currently selected face
public:
    DrawableFace(GLWidget277 *context);
    void setNewFace(Face *newF);            // Set the new face
    void create();                          // Set up the vbo
    GLenum drawMode();                      // Overriden to return GL_TRIANGLES

    Face* getFace();                        // Return the current face pointer
};

class DrawableHalfEdge : public Drawable
{
private:
    HalfEdge *he;   // The currently selected half edge
public:
    DrawableHalfEdge(GLWidget277 *context);
    void setNewHalfEdge(HalfEdge *newHE);   // Set the new half edge
    void create();                          // Set up the vbo
    GLenum drawMode();                      // Overriden to return GL_LINES

    HalfEdge* getHalfEdge();                // Return the current face pointer
};
