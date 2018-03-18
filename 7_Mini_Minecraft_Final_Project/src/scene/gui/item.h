#ifndef ITEM_H
#define ITEM_H

#include "drawable.h"
#include <la.h>
#include "scene/terrain.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Item : public Drawable
{
public:
    Item(OpenGLContext* context, BlockType blockType);
    virtual ~Item(){}
    void create() override;
    GLenum drawMode() override;

    BlockType m_blockType;
    int m_num;

};

#endif // ITEM_H
