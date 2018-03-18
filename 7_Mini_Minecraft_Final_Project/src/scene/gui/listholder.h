#ifndef LISTHOLDER_H
#define LISTHOLDER_H

#include "drawable.h"
#include <la.h>
#include "scene/terrain.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class ListHolder : public Drawable
{
public:
    ListHolder(OpenGLContext* context) : Drawable(context){}
    virtual ~ListHolder(){}
    void create() override;
    GLenum drawMode() override;
};

#endif // LISTHOLDER_H
