#include "drawable.h"
#include <la.h>

Drawable::Drawable(OpenGLContext* context)
    : bufIdx(), bufAttr(), bufFlag(),
      idxBound(false), attrBound(false), flagBound(false),
      context(context)
{}

Drawable::~Drawable()
{}


void Drawable::destroy()
{
    context->glDeleteBuffers(1, &bufIdx);
    context->glDeleteBuffers(1, &bufAttr);
    context->glDeleteBuffers(1, &bufFlag);
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    context->glGenBuffers(1, &bufIdx);
}

bool Drawable::bindIdx()
{
    if(idxBound) {
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

void Drawable::generateAttr()
{
    attrBound = true;
    // Create a VBO on our GPU and store its handle in bufAttr
    context->glGenBuffers(1, &bufAttr);
}

bool Drawable::bindAttr()
{
    if (attrBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    }
    return attrBound;
}

void Drawable::generateFlag()
{
    flagBound = true;
    // Create a VBO on our GPU and store its handle in bufAnimate
    context->glGenBuffers(1, &bufFlag);
}

bool Drawable::bindFlag()
{
    if (flagBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufFlag);
    }
    return flagBound;
}
