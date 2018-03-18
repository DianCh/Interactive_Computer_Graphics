#ifndef PLAYER_H
#define PLAYER_H

#include "la.h"
#include "camera.h"
#include "scene/terrain.h"
#include <QKeyEvent>
#include <QSound>


class Player
{
public:
    Player();

    glm::vec3 m_position;     // A position in 3D space
    glm::vec3 m_velocity;     // A velocity in 3D space
    glm::vec3 m_forwardV;   // velocity componet in forward direnction
    glm::vec3 m_rightV;   // velocity componet in right direnction
    glm::vec3 m_upV;   // velocity componet in up direnction
    bool m_grounded;   // a bool value indicating if the player is on the ground or in the air
    bool m_fly;        // a bool value indicating id the player is in flying mode
    bool m_inWater;    // a bool value indicating id the player is in water
    bool m_inLava;    // a bool value indicating id the player is in Lava

    bool m_walking;   // a bool value indicating if the player is walking
    bool m_swimming;   // a bool value indicating if the player is swimming

    Camera* mp_camera;        // A pointer to a Camera
    int m_rotate_right;         // a int to store how many degrees the camera has rotate about camera's right vector

    bool m_WHold;             // A bool variable indicating if the W key is hold
    bool m_AHold;             // A bool variable indicating if the A key is hold
    bool m_SHold;             // A bool variable indicating if the S key is hold
    bool m_DHold;             // A bool variable indicating if the D key is hold
    bool m_SpaceHold;         // A bool variable indicating if the Space key is hold
    bool m_QHold;             // A bool variable indicating if the Q key is hold
    bool m_EHold;             // A bool variable indicating if the E key is hold
    bool m_FHold;             // A bool variable indicating if the F key is hold

    glm::ivec2 m_cursorPos;    // cursor's X and Y coordinates
    glm::ivec2 m_middleCursor;      // stores the middle position of the mygl window
    bool m_leftButtonDown;    // A bool variable indicating if the left mouse button is hold
    bool m_rightButtonDown;   // A bool variable indicating if the right mouse button is hold

    bool m_destroy;
    bool m_ZHold;
    int m_HP;
    int m_dirtInventory;

    void handleKeyPressEvent(QKeyEvent* e);         // A function that takes in a QKeyEvent from MyGL's key press event functions,
                                               // and updates the relevant member variables based on the event
    void handleKeyReleaseEvent(QKeyEvent* e);         // A function that takes in a QKeyEvent from MyGL's key release event functions,
                                               // and updates the relevant member variables based on the event

    void handleMousePressEvent(QMouseEvent* e);     // A function that takes in a QMouseEvent from MyGL's mouse press event functions
                                               // and updates the relevant member variables based on the event
    void handleMouseReleaseEvent(QMouseEvent* e);     // A function that takes in a QMouseEvent from MyGL's mouse release event functions
                                               // and updates the relevant member variables based on the event
    void handleMouseMoveEvent(QMouseEvent* e);

    void updatePlayerPos(float delta_seconds, Terrain* terrain);

    bool collisionDetect(glm::vec3 newPos, bool& collide, float& min_t, Terrain* p_terrain);

    glm::vec4 getCameraPos() const;

    std::vector<Chunk*> destroySurroundings(Terrain* terrain);

    void eatDirt();


};

class HPBar : public Drawable
{
public:
    glm::vec3 m_color;
    int HPValue;

    HPBar(OpenGLContext* context);
    virtual ~HPBar();
    void create() override;
    GLenum drawMode() override;

};

#endif // PLAYER_H
