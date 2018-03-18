#include "listholder.h"

void ListHolder::create()
{

    GLuint idx[6] = {0, 1, 2, 0, 2, 3};

    glm::vec4 hold_color(1, 1, 1, 0.5);
    float holder_w = 1.5;
    float holder_h = 0.3;
    glm::vec2 left_upper(-0.7, -0.55);
    std::vector<glm::vec4> pnc = {glm::vec4(left_upper.x, left_upper.y, 0.01, 1), glm::vec4(0, 0, 0, 0), hold_color,
                                  glm::vec4(left_upper.x, left_upper.y - holder_h, 0.01, 1), glm::vec4(0, 0, 0, 0), hold_color,
                                  glm::vec4(left_upper.x + holder_w, left_upper.y - holder_h, 0.01, 1), glm::vec4(0, 0, 0, 0), hold_color,
                                  glm::vec4(left_upper.x + holder_w, left_upper.y, 0.01, 1), glm::vec4(0, 0, 0, 0), hold_color};

    count = 6;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnc.size() * sizeof(glm::vec4), pnc.data(), GL_STATIC_DRAW);

}

GLenum ListHolder::drawMode()
{
    return GL_TRIANGLES;
}

