#include "itemcube.h"

ItemCube::ItemCube(OpenGLContext* context) : Drawable(context), m_blockType(EMPTY), m_selected(0){

}

void ItemCube::create()
{
    //std::vector<glm::vec4> itemUV = Chunk::uve[m_blockType];
    std::vector<glm::vec4> thisUV = Chunk::uve[GRASS];

    std::vector<glm::vec4> pnt;
    std::vector<glm::ivec2> flags;

    glm::vec4 eastNorm = glm::vec4(1, 0, 0, 0);
    glm::vec4 southNorm = glm::vec4(0, 0, 1, 0);
    glm::vec4 westNorm = glm::vec4(-1, 0, 0, 0);
    glm::vec4 northNorm = glm::vec4(0, 0, -1, 0);
    glm::vec4 aboveNorm = glm::vec4(0, 1, 0, 0);
    glm::vec4 belowNorm = glm::vec4(0, -1, 0, 0);

    //right-side face

    pnt.push_back(glm::vec4(1, 1, -1, 1));
    pnt.push_back(eastNorm);
    pnt.push_back(thisUV[0]);

    pnt.push_back(glm::vec4(1, 1, 1, 1));
    pnt.push_back(eastNorm);
    pnt.push_back(thisUV[1]);

    pnt.push_back(glm::vec4(1, -1, 1, 1));
    pnt.push_back(eastNorm);
    pnt.push_back(thisUV[2]);

    pnt.push_back(glm::vec4(1, -1, -1, 1));
    pnt.push_back(eastNorm);
    pnt.push_back(thisUV[3]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));

    //front face

    pnt.push_back(glm::vec4(1, 1, 1, 1));
    pnt.push_back(southNorm);
    pnt.push_back(thisUV[4]);

    pnt.push_back(glm::vec4(-1, 1, 1, 1));
    pnt.push_back(southNorm);
    pnt.push_back(thisUV[5]);

    pnt.push_back(glm::vec4(-1, -1, 1, 1));
    pnt.push_back(southNorm);
    pnt.push_back(thisUV[6]);

    pnt.push_back(glm::vec4(1, -1, 1, 1));
    pnt.push_back(southNorm);
    pnt.push_back(thisUV[7]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));

    //left-side face

    pnt.push_back(glm::vec4(-1, 1, 1, 1));
    pnt.push_back(westNorm);
    pnt.push_back(thisUV[8]);

    pnt.push_back(glm::vec4(-1, 1, -1, 1));
    pnt.push_back(westNorm);
    pnt.push_back(thisUV[9]);

    pnt.push_back(glm::vec4(-1, -1, -1, 1));
    pnt.push_back(westNorm);
    pnt.push_back(thisUV[10]);

    pnt.push_back(glm::vec4(-1, -1, 1, 1));
    pnt.push_back(westNorm);
    pnt.push_back(thisUV[11]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));


    //back face

    pnt.push_back(glm::vec4(-1, 1, -1, 1));
    pnt.push_back(northNorm);
    pnt.push_back(thisUV[12]);

    pnt.push_back(glm::vec4(1, 1, -1, 1));
    pnt.push_back(northNorm);
    pnt.push_back(thisUV[13]);

    pnt.push_back(glm::vec4(1, -1, -1, 1));
    pnt.push_back(northNorm);
    pnt.push_back(thisUV[14]);

    pnt.push_back(glm::vec4(-1, -1, -1, 1));
    pnt.push_back(northNorm);
    pnt.push_back(thisUV[15]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));

    //top face

    pnt.push_back(glm::vec4(1, 1, -1, 1));
    pnt.push_back(aboveNorm);
    pnt.push_back(thisUV[16]);

    pnt.push_back(glm::vec4(-1, 1, -1, 1));
    pnt.push_back(aboveNorm);
    pnt.push_back(thisUV[17]);

    pnt.push_back(glm::vec4(-1, 1, 1, 1));
    pnt.push_back(aboveNorm);
    pnt.push_back(thisUV[18]);

    pnt.push_back(glm::vec4(1, 1, 1, 1));
    pnt.push_back(aboveNorm);
    pnt.push_back(thisUV[19]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));

    //bottom face
    pnt.push_back(glm::vec4(1, -1, 1, 1));
    pnt.push_back(belowNorm);
    pnt.push_back(thisUV[20]);

    pnt.push_back(glm::vec4(-1, -1, 1, 1));
    pnt.push_back(belowNorm);
    pnt.push_back(thisUV[21]);

    pnt.push_back(glm::vec4(-1, -1, -1, 1));
    pnt.push_back(belowNorm);
    pnt.push_back(thisUV[22]);

    pnt.push_back(glm::vec4(1, -1, -1, 1));
    pnt.push_back(belowNorm);
    pnt.push_back(thisUV[23]);

    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));
    flags.push_back(glm::ivec2(m_selected, m_selected));


    std::vector<GLuint> idx {0, 1, 2, 0, 2, 3,
                             4, 5, 6, 4, 6, 7,
                             8, 9, 10, 8, 10, 11,
                             12, 13, 14, 12, 14, 15,
                             16, 17, 18, 16, 18, 19,
                             20, 21, 22, 20, 22, 23};

    count = 36;

    generateFlag();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufFlag);
    context->glBufferData(GL_ARRAY_BUFFER, flags.size() * sizeof(glm::ivec2), flags.data(), GL_STATIC_DRAW);

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnt.size() * sizeof(glm::vec4), pnt.data(), GL_STATIC_DRAW);

}

GLenum ItemCube::drawMode()
{
    return GL_TRIANGLES;
}


