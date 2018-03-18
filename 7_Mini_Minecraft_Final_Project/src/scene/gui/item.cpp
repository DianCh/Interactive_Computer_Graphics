#include "item.h"

Item::Item(OpenGLContext* context, BlockType blockType) : Drawable(context), m_blockType(blockType), m_num(0){

}

void Item::create()
{


//    GLuint idx[6] = {0, 1, 2, 0, 2, 3};
//    std::vector<glm::vec4> pnc = {glm::vec4(-1, 1, 0.01, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.3),
//                                  glm::vec4(-1, -1,0.01, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.3),
//                                  glm::vec4(1, -1, 0.01, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.3),
//                                  glm::vec4(1, 1, 0.01, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.3)};

//    count = 6;

//    generateIdx();
//    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
//    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

//    generateAttr();
//    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
//    context->glBufferData(GL_ARRAY_BUFFER, pnc.size() * sizeof(glm::vec4), pnc.data(), GL_STATIC_DRAW);

}

GLenum Item::drawMode()
{
    return GL_TRIANGLES;
}

