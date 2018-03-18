#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include "math.h"

class Camera
{
private:
    glm::vec4 forward;
    glm::vec4 right;
    glm::vec4 up;
    float fOV;
    glm::vec4 position;
    float nearCP;
    float farCP;
    float aspectRatio;

public:
    Camera();
    Camera(glm::vec4 f, glm::vec4 r, glm::vec4 u, float fov, glm::vec4 p, float ncp, float fcp, float ap);
    void setAxes(glm::vec4 newForward, glm::vec4 newRight, glm::vec4 newUp);
    void setFOV(float newFOV);
    void setPosition(glm::vec4 newPosition);
    void setNearCP(float newNCP);
    void setFarCP(float newFCP);
    void setAspectRatio(float newAP);

    glm::vec4 getForward();
    glm::mat4 getViewMatrix();
    glm::mat4 getPerspProjMatrix();
    glm::mat4 cameraToWorld();

    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);

    void rollZ(float delta);
    void pitchX(float delta);
    void yawY(float delta);
};

#endif // CAMERA_H
