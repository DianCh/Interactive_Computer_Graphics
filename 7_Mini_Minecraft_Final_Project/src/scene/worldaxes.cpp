#include "worldaxes.h"

void WorldAxes::create()
{

    GLuint idx[6] = {0, 1, 2, 3, 4, 5};
    std::vector<glm::vec4> pnc = {glm::vec4(32,128,32,1), glm::vec4(0, 0, 0, 0), glm::vec4(1,0,0,1),
                                  glm::vec4(40,128,32,1), glm::vec4(0, 0, 0, 0), glm::vec4(1,0,0,1),
                                  glm::vec4(32,128,32,1), glm::vec4(0, 0, 0, 0), glm::vec4(0,1,0,1),
                                  glm::vec4(32,136,32,1), glm::vec4(0, 0, 0, 0), glm::vec4(0,1,0,1),
                                  glm::vec4(32,128,32,1), glm::vec4(0, 0, 0, 0), glm::vec4(0,0,1,1),
                                  glm::vec4(32,128,40,1), glm::vec4(0, 0, 0, 0), glm::vec4(0,0,1,1)};

    count = 6;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnc.size() * sizeof(glm::vec4), pnc.data(), GL_STATIC_DRAW);
}

GLenum WorldAxes::drawMode()
{
    return GL_LINES;
}
