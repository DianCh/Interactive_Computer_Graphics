#ifndef LINESEG2D_H
#define LINESEG2D_H

#include <glm/glm.hpp>
#include "math.h"

#define EPSILON 0.000001
#define BIGSLOPE 1000000

class LineSeg2D
{
private:
    glm::vec2 p1;
    glm::vec2 p2;
    float k;
public:
    LineSeg2D(glm::vec2 p1, glm::vec2 p2);
    bool ComputeX(float y, float *x);
};

#endif // LINESEG2D_H
