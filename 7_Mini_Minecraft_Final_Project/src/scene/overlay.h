#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Overlay : public Drawable
{
private:
    float depth;

public:
    Overlay(OpenGLContext* context) : Drawable(context), depth(0.01) { m_color = glm::vec3(1, 1, 1); }
    virtual ~Overlay(){}
    void create() override;
    GLenum drawMode() override;

    glm::vec3 m_color;

    void setDepth(float d);
    void createWithDeathTexture();
    void createWithFullScreenTexture();
};
