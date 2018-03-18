#ifndef ITEMCUBE_H
#define ITEMCUBE_H

#include "drawable.h"
#include <la.h>
#include "scene/terrain.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class ItemCube : public Drawable
{
public:
    ItemCube(OpenGLContext* context);
    virtual ~ItemCube(){}
    void create() override;
    GLenum drawMode() override;

    BlockType m_blockType;
    int m_selected;
};

#endif // ITEMCUBE_H
