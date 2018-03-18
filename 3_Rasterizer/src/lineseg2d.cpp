#include "lineseg2d.h"
#include "math.h"

LineSeg2D::LineSeg2D(glm::vec2 p1, glm::vec2 p2) :
    p1(p1), p2(p2), k(abs(p1[0] - p2[0]) < EPSILON ? BIGSLOPE : ((p1[1] - p2[1]) / (p1[0] - p2[0])))
{}

bool LineSeg2D::ComputeX(float y, float *x)
{
    if(y > fminf(p1.y, p2.y) && y <= fmaxf(p1.y, p2.y))
    {
        if(fabsf(p1.x - p2.x) < EPSILON)
        {
            *x = fminf(p1.x, p2.x);
        }
        else
        {
            *x = p1.x + (y - p1.y) / k;
        }
        return true;
    }
    else
    {
        return false;
    }
}

