#ifndef ITEMFRAME_H
#define ITEMFRAME_H

#include "drawable.h"
#include <la.h>
#include "scene/terrain.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class ItemFrame : public Drawable
{
public:
    ItemFrame(OpenGLContext* context);
    virtual ~ItemFrame(){}
    void create() override;
    GLenum drawMode() override;

    int m_selected;
    BlockType m_blockType;
};

#endif // ITEMFRAME_H
