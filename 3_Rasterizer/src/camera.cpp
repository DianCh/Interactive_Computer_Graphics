#include "camera.h"

Camera::Camera() :
    forward(glm::vec4(0, 0, -1, 0)), right(glm::vec4(1, 0, 0, 0)), up(glm::vec4(0, 1, 0, 0)),
    fOV(45), position(glm::vec4(0, 0, 10, 1)),
    nearCP(0.01), farCP(100.0), aspectRatio(1.0) {}

Camera::Camera(glm::vec4 f, glm::vec4 r, glm::vec4 u, float fov, glm::vec4 p, float ncp, float fcp, float ap) :
    forward(f), right(r), up(u),
    fOV(fov), position(p),
    nearCP(ncp), farCP(fcp), aspectRatio(ap) {}

void Camera::setAxes(glm::vec4 newForward, glm::vec4 newRight, glm::vec4 newUp)
{
    forward = newForward;
    right = newRight;
    up = newUp;
}

void Camera::setFOV(float newFOV)
{
    fOV = newFOV;
}

void Camera::setPosition(glm::vec4 newPosition)
{
    position = newPosition;
}

void Camera::setNearCP(float newNCP)
{
    nearCP = newNCP;
}

void Camera::setFarCP(float newFCP)
{
    farCP = newFCP;
}

void Camera::setAspectRatio(float newAP)
{
    aspectRatio = newAP;
}

glm::vec4 Camera::getForward()
{
    return forward;
}

glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 viewTranslate(glm::vec4(1, 0, 0, 0),
                            glm::vec4(0, 1, 0, 0),
                            glm::vec4(0, 0, 1, 0),
                            glm::vec4( - glm::vec3(position), 1));

    glm::mat4 viewOrient(glm::vec4(right[0], up[0], forward[0], 0),
                         glm::vec4(right[1], up[1], forward[1], 0),
                         glm::vec4(right[2], up[2], forward[2], 0),
                         glm::vec4(0, 0, 0, 1));

    return viewOrient * viewTranslate;
}

glm::mat4 Camera::getPerspProjMatrix()
{
    float fov_radian = fOV / 180.0 * M_PI;

    float p = farCP / (farCP - nearCP);
    float q = - farCP * nearCP / (farCP - nearCP);
    float s = 1 / tan(fOV / 2);

    return glm::mat4(glm::vec4(s / aspectRatio, 0, 0, 0),
                    glm::vec4(0, s, 0, 0),
                    glm::vec4(0, 0, p, 1),
                    glm::vec4(0, 0, q, 0));
}

glm::mat4 Camera::cameraToWorld()
{
    return glm::mat4(right, up, forward, position);
}

void Camera::moveForward(float delta)
{
    // Move delta in Z can be viewed as a point (0, 0, delta, 1) in current camera frame.
    // Transform this point to world frame to get new position.
    position = cameraToWorld() * glm::vec4(0, 0, delta, 1);
}

void Camera::moveRight(float delta)
{
    // Move delta in X can be viewed as a point (delta, 0, 0, 1) in current camera frame.
    // Transform this point to world frame to get new position.
    position = cameraToWorld() * glm::vec4(delta, 0, 0, 1);
}

void Camera::moveUp(float delta)
{
    // Move delta in Y can be viewed as a point (0, delta, 0, 1) in current camera frame.
    // Transform this point to world frame to get new position.
    position = cameraToWorld() * glm::vec4(0, delta, 0, 1);
}

void Camera::rollZ(float delta)
{
    float delta_radians = delta / 180.0 * M_PI;

    glm::mat4 rotateZ(glm::vec4(cos(delta_radians), sin(delta_radians), 0, 0),
                      glm::vec4(-sin(delta_radians), cos(delta_radians), 0, 0),
                      glm::vec4(0, 0, 1, 0),
                      glm::vec4(0, 0, 0, 1));

    // Compute the new camera-to-world transformation
    glm::mat4 newCamToWorld = cameraToWorld() * rotateZ;

    // Extract the new columns
    forward = newCamToWorld[2];  // Unchanged in fact
    right = newCamToWorld[0];
    up = newCamToWorld[1];
}

void Camera::pitchX(float delta)
{
    float delta_radians = delta / 180.0 * M_PI;

    glm::mat4 rotateX(glm::vec4(1, 0, 0, 0),
                      glm::vec4(0, cos(delta_radians), sin(delta_radians), 0),
                      glm::vec4(0, -sin(delta_radians), cos(delta_radians), 0),
                      glm::vec4(0, 0, 0, 1));

    // Compute the new camera-to-world transformation
    glm::mat4 newCamToWorld = cameraToWorld() * rotateX;

    // Extract the new columns
    forward = newCamToWorld[2];
    right = newCamToWorld[0];   // Unchanged in fact
    up = newCamToWorld[1];
}

void Camera::yawY(float delta)
{
    float delta_radians = delta / 180.0 * M_PI;

    glm::mat4 rotateY(glm::vec4(cos(delta_radians), 0, -sin(delta_radians), 0),
                      glm::vec4(0, 1, 0, 0),
                      glm::vec4(sin(delta_radians), 0, cos(delta_radians), 0),
                      glm::vec4(0, 0, 0, 1));

    // Compute the new camera-to-world transformation
    glm::mat4 newCamToWorld = cameraToWorld() * rotateY;

    // Extract the new columns
    forward = newCamToWorld[2];
    right = newCamToWorld[0];
    up = newCamToWorld[1];      // Unchanged in fact
}
