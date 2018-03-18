#include "itemframe.h"

ItemFrame::ItemFrame(OpenGLContext* context) : Drawable(context), m_selected(0), m_blockType(EMPTY){

}

void ItemFrame::create()
{

    std::vector<glm::vec4> frameUV;
    if(m_blockType == EMPTY) {
        frameUV = Chunk::uve[BlockWrapper(STONE)];
    } else {
        frameUV = Chunk::uve[BlockWrapper(m_blockType)];
    }

    GLuint idx[6] = {0, 1, 2, 0, 2, 3};

    std::vector<glm::vec4> pnt = {glm::vec4(-0.5, 0.5, 0.01, 1), glm::vec4(0, 0, 0, 0), frameUV[1],
                                  glm::vec4(-0.5, -0.5,0.01, 1), glm::vec4(0, 0, 0, 0), frameUV[2],
                                  glm::vec4(0.5, -0.5, 0.01, 1), glm::vec4(0, 0, 0, 0), frameUV[3],
                                  glm::vec4(0.5, 0.5, 0.01, 1), glm::vec4(0, 0, 0, 0), frameUV[0]};

    count = 6;

    std::vector<glm::ivec2> flags = {glm::ivec2(m_selected, m_selected),
                                     glm::ivec2(m_selected, m_selected),
                                     glm::ivec2(m_selected, m_selected),
                                     glm::ivec2(m_selected, m_selected),
                                     glm::ivec2(m_selected, m_selected),
                                     glm::ivec2(m_selected, m_selected)};

    generateFlag();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufFlag);
    context->glBufferData(GL_ARRAY_BUFFER, flags.size() * sizeof(glm::ivec2), flags.data(), GL_STATIC_DRAW);

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnt.size() * sizeof(glm::vec4), pnt.data(), GL_STATIC_DRAW);


}

GLenum ItemFrame::drawMode()
{
    return GL_TRIANGLES;
}
