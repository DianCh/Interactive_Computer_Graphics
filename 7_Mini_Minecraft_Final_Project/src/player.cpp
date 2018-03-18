#include "player.h"
#include <iostream>
#include <QApplication>

// constructor
Player::Player(): m_position(glm::vec3(5, 245, 5)),
                  m_velocity(glm::vec3(0, 0, 0)),
                  m_forwardV(glm::vec3(0, 0, 0)),
                  m_rightV(glm::vec3(0, 0, 0)),
                  m_upV(glm::vec3(0, 0, 0)),
                  mp_camera(new Camera()),
                  m_WHold(false),
                  m_AHold(false),
                  m_SHold(false),
                  m_DHold(false),
                  m_SpaceHold(false),
                  m_FHold(false),
                  m_QHold(false),
                  m_EHold(false),
                  m_cursorPos(glm::ivec2(309, 216)),
                  m_leftButtonDown(false),
                  m_rightButtonDown(false),
                  m_grounded(true),
                  m_fly(false),
                  m_inWater(false),
                  m_inLava(false),
                  m_ZHold(false),
                  m_destroy(false),
                  m_HP(10),
                  m_dirtInventory(0),
                  m_walking(false),
                  m_swimming(false)
{
    mp_camera->eye = glm::vec3(m_position.x + 0.5, m_position.y + 1, m_position.z + 0.5);
}

// A function that takes in a QKeyEvent from MyGL's key press and key release event functions,
// and updates the relevant member variables based on the event
void Player::handleKeyPressEvent(QKeyEvent* e) {
    if(e->key() == Qt::Key_W) {
        m_WHold = true;
    } else if (e->key() == Qt::Key_A) {
        m_AHold = true;
    } else if (e->key() == Qt::Key_S) {
        m_SHold = true;
    } else if (e->key() == Qt::Key_D) {
        m_DHold = true;
    } else if (e->key() == Qt::Key_Space) {
        m_SpaceHold = true;
    } else if (e->key() == Qt::Key_F) {
        m_FHold = !m_FHold;
    } else if (e->key() == Qt::Key_Q) {
        m_QHold = true;
    } else if (e->key() == Qt::Key_E) {
        m_EHold = true;
    } else if (e->key() == Qt::Key_Z) {
        m_ZHold = !m_ZHold;
    } else if (e->key() == Qt::Key_C) {
        eatDirt();
    }


}

void Player::handleKeyReleaseEvent(QKeyEvent* e) {
    if(e->key() == Qt::Key_W) {
        m_WHold = false;
    } else if (e->key() == Qt::Key_A) {
        m_AHold = false;
    } else if (e->key() == Qt::Key_S) {
        m_SHold = false;
    } else if (e->key() == Qt::Key_D) {
        m_DHold = false;
    } else if (e->key() == Qt::Key_Space) {
        m_SpaceHold = false;
    } else if (e->key() == Qt::Key_F) {
        //m_FHold = false;
    } else if (e->key() == Qt::Key_Q) {
        m_QHold = false;
    } else if (e->key() == Qt::Key_E) {
        m_EHold = false;
    }
}


// A function that takes in a QMouseEvent from MyGL's mouse press functions
// and updates the relevant member variables based on the event
void Player::handleMousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        m_leftButtonDown = true;
    } else if (e->button() == Qt::RightButton) {
        m_rightButtonDown = true;
    }
}

// A function that takes in a QMouseEvent from MyGL's mouse release event functions
// and updates the relevant member variables based on the event
void Player::handleMouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        m_leftButtonDown = false;
    } else if (e->button() == Qt::RightButton) {
        m_rightButtonDown = false;
    }
}

void Player::handleMouseMoveEvent(QMouseEvent* e) {
    m_cursorPos.x = e->pos().x();
    m_cursorPos.y = e->pos().y();

    // update camera attributes
    int delta_x = m_cursorPos.x - m_middleCursor.x;
    int delta_y = m_cursorPos.y - m_middleCursor.y;
    mp_camera->RotateAboutRight((-1) * delta_y * 1);
    mp_camera->RotateAboutUp((-1) * delta_x * 1);
}

void Player::updatePlayerPos(float delta_seconds, Terrain* p_terrain) {
    glm::vec3 lastPos = m_position;

    float velocity = 6.f;     // velocity constant

    //============================milestone2 feature===========================
    // determine whether player is in Water or lava
    if(p_terrain->getBlockTypeAt(glm::floor(mp_camera->eye.x), glm::floor(mp_camera->eye.y), glm::floor(mp_camera->eye.z)) == WATER) {
        m_inWater = true;
        m_inLava = false;
        velocity = velocity * 1.f / 3.f;
    } else if(p_terrain->getBlockTypeAt(glm::floor(mp_camera->eye.x), glm::floor(mp_camera->eye.y), glm::floor(mp_camera->eye.z)) == LAVA) {
        m_inLava = true;
        m_inWater = false;
        velocity = velocity * 1.f / 3.f;
    } else {
        m_inWater = false;
        m_inLava = false;
    }
    //====================================================================================

    if(m_FHold) {
        m_fly = true;
    } else {
        m_fly = false;
    }

    if (m_ZHold)
    {
        m_destroy = true;
    }
    else
    {
        m_destroy = false;
    }

    // in flying mode
    if(m_fly) {
        m_forwardV = glm::vec3(0, 0, 0);
        m_rightV = glm::vec3(0, 0, 0);
        m_upV = glm::vec3(0, 0, 0);

        if(m_WHold) {
            glm::vec3 forwardV = mp_camera->look;
            forwardV.y = 0;
            forwardV = glm::normalize(forwardV);
            m_forwardV += forwardV;
        }

        if(m_SHold) {
            glm::vec3 forwardV = (-1.f) * mp_camera->look;
            forwardV.y = 0;
            forwardV = glm::normalize(forwardV);
            m_forwardV += forwardV;
        }

        if(m_AHold) {
            glm::vec3 rightV = (-1.f) * mp_camera->right;
            m_rightV += rightV;
        }

        if(m_DHold) {
            glm::vec3 rightV = mp_camera->right;
            m_rightV += rightV;
        }

        if(m_QHold) {
            glm::vec3 upV = mp_camera->world_up;
            m_upV += upV;
        }

        if(m_EHold) {
            glm::vec3 upV = (-1.f) * mp_camera->world_up;
            m_upV += upV;
        }


        if(glm::length(m_forwardV + m_rightV + m_upV) != 0) {
            m_velocity = velocity * glm::normalize(m_forwardV + m_rightV + m_upV);
        } else {
            m_velocity = glm::vec3(0, 0, 0);
        }

        m_position = m_position + m_velocity * delta_seconds;
        mp_camera->eye = mp_camera->eye + m_velocity * delta_seconds;
        mp_camera->ref = mp_camera->ref + m_velocity * delta_seconds;

    }

    // not in flying mode
    else {
        // update m_grounded attribute
        glm::vec3 bottom_1(m_position.x, m_position.y, m_position.z);
        glm::vec3 bottom_2(m_position.x, m_position.y, m_position.z + 1);
        glm::vec3 bottom_3(m_position.x + 1, m_position.y, m_position.z + 1);
        glm::vec3 bottom_4(m_position.x + 1, m_position.y, m_position.z);

        if((p_terrain->getBlockTypeAt(glm::floor(bottom_1.x), glm::floor(bottom_1.y) - 1, glm::floor(bottom_1.z)) != EMPTY ||
                p_terrain->getBlockTypeAt(glm::floor(bottom_2.x), glm::floor(bottom_2.y) - 1, glm::floor(bottom_2.z)) != EMPTY ||
                p_terrain->getBlockTypeAt(glm::floor(bottom_3.x), glm::floor(bottom_3.y) - 1, glm::floor(bottom_3.z)) != EMPTY ||
                p_terrain->getBlockTypeAt(glm::floor(bottom_4.x), glm::floor(bottom_4.y) - 1, glm::floor(bottom_4.z)) != EMPTY)) {

            m_grounded = true;

            //========================mileStone2 feature===============================================
            if((p_terrain->getBlockTypeAt(glm::floor(bottom_1.x), glm::floor(bottom_1.y) - 1, glm::floor(bottom_1.z)) == WATER &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_2.x), glm::floor(bottom_2.y) - 1, glm::floor(bottom_2.z)) == WATER &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_3.x), glm::floor(bottom_3.y) - 1, glm::floor(bottom_3.z)) == WATER &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_4.x), glm::floor(bottom_4.y) - 1, glm::floor(bottom_4.z)) == WATER)) {

                m_grounded = false;
            }

            if((p_terrain->getBlockTypeAt(glm::floor(bottom_1.x), glm::floor(bottom_1.y) - 1, glm::floor(bottom_1.z)) == LAVA &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_2.x), glm::floor(bottom_2.y) - 1, glm::floor(bottom_2.z)) == LAVA &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_3.x), glm::floor(bottom_3.y) - 1, glm::floor(bottom_3.z)) == LAVA &&
                p_terrain->getBlockTypeAt(glm::floor(bottom_4.x), glm::floor(bottom_4.y) - 1, glm::floor(bottom_4.z)) == LAVA)) {

                m_grounded = false;
            }
            //==================================================================================================

        } else {
            m_grounded = false;
        }

        // update player velocity
        m_forwardV = glm::vec3(0, 0, 0);
        m_rightV = glm::vec3(0, 0, 0);
        if(m_grounded) {
            m_upV = glm::vec3(0, 0, 0);
        }

        if(m_WHold) {
            glm::vec3 forwardV = mp_camera->look;
            forwardV.y = 0;
            forwardV = glm::normalize(forwardV);
            m_forwardV += forwardV;
        }

        if(m_SHold) {
            glm::vec3 forwardV = (-1.f) * mp_camera->look;
            forwardV.y = 0;
            forwardV = glm::normalize(forwardV);
            m_forwardV += forwardV;
        }

        if(m_AHold) {
            glm::vec3 rightV = (-1.f) * mp_camera->right;
            m_rightV += rightV;
        }

        if(m_DHold) {
            glm::vec3 rightV = mp_camera->right;
            m_rightV += rightV;
        }

        if(m_SpaceHold) {
            m_upV = mp_camera->world_up;
        } else {
            if(!m_grounded) {
                m_upV.y = m_upV.y - 9.8 * delta_seconds;
            }
        }

        if(glm::length(m_forwardV + m_rightV) != 0) {
            m_velocity = velocity * glm::normalize(m_forwardV + m_rightV) + velocity * m_upV;
        } else {
            if(glm::length(m_upV) != 0) {
                m_velocity = velocity * m_upV;
            } else {
                m_velocity = glm::vec3(0, 0, 0);
            }
        }

        // update player position
        if(glm::length(m_velocity) != 0) {
            glm::vec3 newPos = m_position + m_velocity * delta_seconds;
            glm::vec3 newEyePos = mp_camera->eye + m_velocity * delta_seconds;
            glm::vec3 newRefPos = mp_camera->ref + m_velocity * delta_seconds;

            bool collide = false;
            float min_t = FLT_MAX;
            collisionDetect(newPos, collide, min_t, p_terrain);

            if(collide) {
                glm::vec3 delta_dist = newPos - m_position;
                m_position = m_position + min_t * delta_dist;
                mp_camera->eye = mp_camera->eye + min_t * delta_dist;
                mp_camera->ref = mp_camera->ref + min_t * delta_dist;

                if(!m_grounded) {

                    glm::vec3 newPos = m_position + m_velocity * delta_seconds;

                    bool collide = false;
                    float min_t = FLT_MAX;
                    collisionDetect(newPos, collide, min_t, p_terrain);

                    if(!collide) {
                        m_position = m_position + velocity * m_upV * delta_seconds;
                        mp_camera->eye = mp_camera->eye + velocity * m_upV * delta_seconds;
                        mp_camera->ref = mp_camera->ref + velocity * m_upV * delta_seconds;
                    }
                }


            } else {
                m_position = newPos;
                mp_camera->eye = newEyePos;
                mp_camera->ref = newRefPos;
            }
        }
    }

    //==========================playing sound feature=================================
    //for water sound
    if(m_inLava || m_inWater) {
        if(m_velocity.length() > 0) {
            m_swimming = true;
        } else {
            m_swimming = false;
        }
    }
    if(!m_inLava && !m_inWater) {
        m_swimming = false;
    }

    // for walk sould
    if(!m_inLava && !m_inWater) {
        if(lastPos == m_position) {
            m_walking = false;
        } else {
            if(m_grounded) {
                m_walking = true;
            } else {
                if(p_terrain->getBlockTypeAt(glm::floor(lastPos.x), glm::floor(lastPos.y) - 2, glm::floor(lastPos.z)) != EMPTY &&
                   p_terrain->getBlockTypeAt(glm::floor(lastPos.x), glm::floor(lastPos.y) - 2, glm::floor(lastPos.z)) != WATER &&
                   p_terrain->getBlockTypeAt(glm::floor(lastPos.x), glm::floor(lastPos.y) - 2, glm::floor(lastPos.z)) != LAVA){
                    m_walking = true;
                } else {
                    m_walking = false;
                }
            }
        }
    }
    if(m_fly) {
        m_walking = false;
    }

}

bool Player::collisionDetect(glm::vec3 newPos, bool& collide, float& min_t, Terrain* p_terrain) {
    std::vector<glm::vec3> newVertexPos;
    newVertexPos.clear();

    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y, newPos.z));
    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y, newPos.z));

    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y + 1, newPos.z));
    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y + 1, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y + 1, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y + 1, newPos.z));

    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y + 2, newPos.z));
    newVertexPos.push_back(glm::vec3(newPos.x, newPos.y + 2, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y + 2, newPos.z + 1));
    newVertexPos.push_back(glm::vec3(newPos.x + 1, newPos.y + 2, newPos.z));

    std::vector<glm::vec3> origVertexPos;
    origVertexPos.clear();
    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y, m_position.z));
    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y, m_position.z));

    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y + 1, m_position.z));
    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y + 1, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y + 1, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y + 1, m_position.z));

    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y + 2, m_position.z));
    origVertexPos.push_back(glm::vec3(m_position.x, m_position.y + 2, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y + 2, m_position.z + 1));
    origVertexPos.push_back(glm::vec3(m_position.x + 1, m_position.y + 2, m_position.z));

    collide = false;
    glm::ivec3 collideIdx(0, 0, 0);
    glm::vec3 collidePos(0, 0, 0);
    for(int i = 0; i < newVertexPos.size(); ++i) {
        glm::vec3 newVPos = newVertexPos[i];
        glm::vec3 origVPos = origVertexPos[i];
        glm::vec3 delta_dist = newVPos - origVPos;

        float increment = 0.1;
        for(float t = increment; t <= 1; t = t + increment) {
            glm::vec3 marchDist = delta_dist * t;
            glm::vec3 marchPos = origVPos + marchDist;
            glm::ivec3 index(glm::floor(marchPos.x), glm::floor(marchPos.y), glm::floor(marchPos.z));

            if(p_terrain->getBlockTypeAt(index.x, index.y, index.z) != LAVA &&
               p_terrain->getBlockTypeAt(index.x, index.y, index.z) != WATER &&
               p_terrain->getBlockTypeAt(index.x, index.y, index.z) != EMPTY) {

                collide = true;
                collideIdx = index;
                collidePos = m_position + delta_dist * t;
                if((t - increment) <= min_t) {
                    min_t = t - increment;
                    break;
                }
            }
        }
    }

    return collide;

}

glm::vec4 Player::getCameraPos() const
{
    return glm::vec4(mp_camera->eye, 1);
}

HPBar::HPBar(OpenGLContext *context) : Drawable(context), m_color(glm::vec3(1, 0, 0)), HPValue(10) {}

HPBar::~HPBar() {}

void HPBar::create()
{

    GLuint idx[6] = {0, 1, 2, 0, 2, 3};
    std::vector<glm::vec4> pnc = {glm::vec4(-0.9, 0.85, 0.005, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.6),
                                  glm::vec4(-0.9 + 0.08 * HPValue, 0.85, 0.005, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.6),
                                  glm::vec4(-0.9 + 0.08 * HPValue, 0.9, 0.005, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.6),
                                  glm::vec4(-0.9, 0.9, 0.005, 1), glm::vec4(0, 0, 0, 0), glm::vec4(m_color, 0.6)};

    count = 6;

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    generateAttr();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufAttr);
    context->glBufferData(GL_ARRAY_BUFFER, pnc.size() * sizeof(glm::vec4), pnc.data(), GL_STATIC_DRAW);


}

GLenum HPBar::drawMode()
{
    return GL_TRIANGLES;
}

std::vector<Chunk*> Player::destroySurroundings(Terrain *terrain)
{
    int thisX = glm::floor(m_position.x);
    int thisY = glm::floor(m_position.y);
    int thisZ = glm::floor(m_position.z);

    std::vector<Chunk*> modifyChunks;

    for (int i = -2; i < 3; ++i)
    {
        for (int j = -2; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                int blockX = thisX + i;
                int blockY = thisY + k;
                int blockZ = thisZ + j;

                BlockType b = terrain->getBlockTypeAt(blockX, blockY, blockZ);

                if (b != BlockType::EMPTY)
                {
                    if (b == BlockType::DIRT)
                    {
                        m_dirtInventory += 1;
                    }

                    BlockType empty = BlockType::EMPTY;
                    terrain->setBlockTypeAt(blockX, blockY, blockZ, empty);

                    Chunk *aChunk = terrain->getChunk(terrain->convertBlockCoord2ChunkCoord(blockX, blockZ));

                    if (aChunk != nullptr && std::find(modifyChunks.begin(), modifyChunks.end(), aChunk) == modifyChunks.end())
                    {
                        modifyChunks.push_back(aChunk);
                    }
                }
            }
        }
    }

    return modifyChunks;
}

void Player::eatDirt()
{
    if (m_dirtInventory > 10 && m_HP < 10)
    {
        m_dirtInventory -= 10;
    }
}



