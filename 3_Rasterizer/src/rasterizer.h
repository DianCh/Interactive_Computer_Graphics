#pragma once
#include <polygon.h>
#include <QImage>
#include <algorithm>
#include <iostream>
#include "math.h"
#include "camera.h"

#define WIDTH 512
#define HEIGHT 512

class Rasterizer
{
private:
    //This is the set of Polygons loaded from a JSON scene file
    std::vector<Polygon> m_polygons;
public:
    Rasterizer(const std::vector<Polygon>& polygons);
    QImage RenderScene();

    QImage RenderScene3D(Camera &myCam);
    void ClearScene();
};
