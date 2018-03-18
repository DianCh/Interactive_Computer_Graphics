#include "rasterizer.h"

Rasterizer::Rasterizer(const std::vector<Polygon>& polygons)
    : m_polygons(polygons)
{}

QImage Rasterizer::RenderScene()
{
    // TODO: Complete the various components of code that make up this function
    // It should return the rasterized image of the current scene

    // Notice that even though it should return a QImage this function compiles.
    // Remember, C++'s compiler is dumb (though it will at least warn you that
    // the function doesn't return anything).
    // BEWARE! If you use a function that is missing its return statement,
    // it will return garbage memory!

    // Initialize the displayed image and fill black.
    QImage q(WIDTH, HEIGHT, QImage::Format_RGB32);
    q.fill(0);

    // Temporate storage for bounding box of the triangle to be examined.
    float boundXLeft = 0;
    float boundXRight = WIDTH;
    float boundYUpper = 0;
    float boundYLower = HEIGHT;

    float z_buffer[WIDTH * HEIGHT];
    std::fill_n(z_buffer, WIDTH * HEIGHT, 1.5);

    // Loop over each polygon.
    for (Polygon &poly : m_polygons)
    {
        // Loop over each triangle of this polygon.
        for (Triangle &tri : poly.m_tris)
        {
            // Construct three 2D lines for this triangle.
            LineSeg2D l1(glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[0]].m_pos)), glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[1]].m_pos)));

            LineSeg2D l2(glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[0]].m_pos)), glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[2]].m_pos)));

            LineSeg2D l3(glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[1]].m_pos)), glm::vec2(glm::vec3(poly.m_verts[tri.m_indices[2]].m_pos)));

            // Determine the bounding box for a particular triangle.
            boundXLeft = std::min(std::min((poly.m_verts[tri.m_indices[0]].m_pos)[0], (poly.m_verts[tri.m_indices[1]].m_pos)[0]),
                                  (poly.m_verts[tri.m_indices[2]].m_pos)[0]);

            boundXRight = std::max(std::max((poly.m_verts[tri.m_indices[0]].m_pos)[0], (poly.m_verts[tri.m_indices[1]].m_pos)[0]),
                                   (poly.m_verts[tri.m_indices[2]].m_pos)[0]);

            boundYUpper = std::min(std::min((poly.m_verts[tri.m_indices[0]].m_pos)[1], (poly.m_verts[tri.m_indices[1]].m_pos)[1]),
                                   (poly.m_verts[tri.m_indices[2]].m_pos)[1]);

            boundYLower = std::max(std::max((poly.m_verts[tri.m_indices[0]].m_pos)[1], (poly.m_verts[tri.m_indices[1]].m_pos)[1]),
                                   (poly.m_verts[tri.m_indices[2]].m_pos)[1]);

            // Find the start & end for scan line, and check if they need to be clipped to the screen.
            float startY = (boundYUpper < 0) ? 0 : boundYUpper;
            float endY = (boundYLower > HEIGHT) ? HEIGHT : boundYLower;

            // Loop over rows that are covered.
            for (int i = ceil(startY); i < endY; i++)
            {

                float startX = (boundXLeft < 0) ? 0 : boundXLeft;
                float endX = (boundXRight > WIDTH) ? WIDTH : boundXRight;

                float interX1 = startX;
                float interX2 = startX;

                if(l1.ComputeX(i, &interX1) && l2.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }
                else if(l2.ComputeX(i, &interX1) && l3.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }
                else if(l3.ComputeX(i, &interX1) && l1.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }

                startX = (startX < 0) ? 0 : startX;
                endX = (endX > WIDTH) ? WIDTH : endX;

                // Loop over the pixels inside the triangle.
                for (int j = ceil(startX); j < endX; j++)
                {
                    glm::vec3 baryCords = poly.ComputeBaryCords2D(tri, glm::vec4(glm::vec2(j,i), poly.m_verts[tri.m_indices[0]].m_pos[2], 0));
                    float z = baryCords[0] * poly.m_verts[tri.m_indices[0]].m_pos[2] +
                              baryCords[1] * poly.m_verts[tri.m_indices[1]].m_pos[2] +
                              baryCords[2] * poly.m_verts[tri.m_indices[2]].m_pos[2];
                    if (fabsf((baryCords[0] + baryCords[1] + baryCords[2]) - 1) < EPSILON)
                    {
                        if (z < z_buffer[WIDTH * j + i])
                        {
                            z_buffer[WIDTH * j + i] = z;
                            glm::vec3 interpColor = baryCords[0] * poly.m_verts[tri.m_indices[0]].m_color +
                                                    baryCords[1] * poly.m_verts[tri.m_indices[1]].m_color +
                                                    baryCords[2] * poly.m_verts[tri.m_indices[2]].m_color;

                            QColor color(interpColor[0], interpColor[1], interpColor[2]);


                            q.setPixelColor(j, i, color);
                        }
                    }

                }
            }

        }
    }
    return q;
}

QImage Rasterizer::RenderScene3D(Camera &myCam)
{
    // Initialize the displayed image and fill black.
    QImage q(WIDTH, HEIGHT, QImage::Format_RGB32);
    q.fill(0);

    // Temporate storage for bounding box of the triangle to be examined.
    float boundXLeft = 0;
    float boundXRight = WIDTH;
    float boundYUpper = 0;
    float boundYLower = HEIGHT;

    float z_buffer[WIDTH * HEIGHT];
    std::fill_n(z_buffer, WIDTH * HEIGHT, 1000);

    // Loop over each polygon.
    for (Polygon &poly : m_polygons)
    {
        // Loop over each triangle of this polygon.
        for (Triangle &tri : poly.m_tris)
        {
            // Compute the front face norm of this triangle
            glm::vec3 vAB = glm::vec3(poly.m_verts[tri.m_indices[1]].m_pos - poly.m_verts[tri.m_indices[0]].m_pos);
            glm::vec3 vBC = glm::vec3(poly.m_verts[tri.m_indices[2]].m_pos - poly.m_verts[tri.m_indices[1]].m_pos);

            glm::vec3 frontNorm = glm::cross(vAB, vBC);

            // Back-face culling
            if (glm::dot(frontNorm, glm::vec3(myCam.getForward())) > 0)
            {
                continue;
            }

            // Transform the 3 vertices to normal device space
            glm::vec4 vert_0 = myCam.getPerspProjMatrix() * myCam.getViewMatrix() * poly.m_verts[tri.m_indices[0]].m_pos;
            glm::vec4 vert_1 = myCam.getPerspProjMatrix() * myCam.getViewMatrix() * poly.m_verts[tri.m_indices[1]].m_pos;
            glm::vec4 vert_2 = myCam.getPerspProjMatrix() * myCam.getViewMatrix() * poly.m_verts[tri.m_indices[2]].m_pos;

            vert_0 = vert_0 / vert_0[3];
            vert_1 = vert_1 / vert_1[3];
            vert_2 = vert_2 / vert_2[3];

            // Transform again to the pixel space.
            glm::vec2 vert_pix_0((vert_0[0] + 1) * WIDTH / 2, (1 - vert_0[1]) * HEIGHT / 2);
            glm::vec2 vert_pix_1((vert_1[0] + 1) * WIDTH / 2, (1 - vert_1[1]) * HEIGHT / 2);
            glm::vec2 vert_pix_2((vert_2[0] + 1) * WIDTH / 2, (1 - vert_2[1]) * HEIGHT / 2);

            // Determine the bounding box for this particular triangle.
            boundXLeft = std::min(std::min(vert_pix_0[0], vert_pix_1[0]), vert_pix_2[0]);

            boundXRight = std::max(std::max(vert_pix_0[0], vert_pix_1[0]), vert_pix_2[0]);

            boundYUpper = std::min(std::min(vert_pix_0[1], vert_pix_1[1]), vert_pix_2[1]);

            boundYLower = std::max(std::max(vert_pix_0[1], vert_pix_1[1]), vert_pix_2[1]);

            // Construct three 2D lines for this triangle.
            LineSeg2D l1(vert_pix_0, vert_pix_1);

            LineSeg2D l2(vert_pix_1, vert_pix_2);

            LineSeg2D l3(vert_pix_2, vert_pix_0);

            // Find the start & end for scan line, and check if they need to be clipped to the screen.
            float startY = (boundYUpper < 0) ? 0 : boundYUpper;
            float endY = (boundYLower > HEIGHT) ? HEIGHT : boundYLower;

            // Loop over rows that are covered.
            for (int i = ceil(startY); i < endY; i++)
            {

                // The start & end points for the scan line
                float startX = (boundXLeft < 0) ? 0 : boundXLeft;
                float endX = (boundXRight > WIDTH) ? WIDTH : boundXRight;

                float interX1 = startX;
                float interX2 = startX;

                // Compute the start & end points using intersection
                if(l1.ComputeX(i, &interX1) && l2.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }
                else if(l2.ComputeX(i, &interX1) && l3.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }
                else if(l3.ComputeX(i, &interX1) && l1.ComputeX(i, &interX2))
                {
                    startX = fminf(interX1, interX2);
                    endX = fmaxf(interX1, interX2);
                }

                // Clamp again to the screen
                startX = (startX < 0) ? 0 : startX;
                endX = (endX + 2 > WIDTH) ? WIDTH : endX + 2;  // The + 2 is used to fix floating number precision issue

                // Loop over the pixels inside the triangle.
                for (int j = (int)(startX); j < (int)(endX); j++)
                {
                    // Compute the barycentric coordinates
                    glm::vec3 baryCords = poly.ComputeBaryCords3D(vert_pix_0, vert_pix_1, vert_pix_2, glm::vec2(j, i));

                    // Verify that the pixel of interest is inside the triangle indeed.
                    if (fabsf(baryCords[0] + baryCords[1] + baryCords[2] - 1) < EPSILON)
                    {
                        // Compute the corrected z
                        float a = baryCords[0] / vert_0.z;
                        float b = baryCords[1] / vert_1.z;
                        float c = baryCords[2] / vert_2.z;

                        float z = a + b + c;

                        z = 1 / z;

                        // Paint only if the pixel is in between the frustum and is closer than previous ones.
                        if (z > 0 && z < 1 && z < z_buffer[WIDTH * j + i])
                        {
                            // Update z-buffer
                            z_buffer[WIDTH * j + i] = z;

                            // Interpolate uv coordinates.
                            glm::vec2 uv = z * (baryCords[0] * poly.m_verts[tri.m_indices[0]].m_uv / vert_0.z +
                                                baryCords[1] * poly.m_verts[tri.m_indices[1]].m_uv / vert_1.z +
                                                baryCords[2] * poly.m_verts[tri.m_indices[2]].m_uv / vert_2.z);

                            // Interpolate normals.
                            glm::vec4 normal = z * (baryCords[0] * poly.m_verts[tri.m_indices[0]].m_normal / vert_0.z +
                                                    baryCords[1] * poly.m_verts[tri.m_indices[1]].m_normal / vert_1.z +
                                                    baryCords[2] * poly.m_verts[tri.m_indices[2]].m_normal / vert_2.z);

                            // Interpolate colors.
                            glm::vec3 color = z * (baryCords[0] * poly.m_verts[tri.m_indices[0]].m_color / vert_0.z +
                                                   baryCords[1] * poly.m_verts[tri.m_indices[1]].m_color / vert_1.z +
                                                   baryCords[2] * poly.m_verts[tri.m_indices[2]].m_color / vert_2.z);

                            // Retrieve color from texture map and set color to the pixel.
                            glm::vec3 texture = GetImageColor(uv, poly.mp_texture);
                            float ambient = 0.15;

                            glm::vec3 ambientT = texture * ambient;

                            texture = texture * (glm::dot(glm::vec3(normal), glm::vec3(- myCam.getForward())));

                            texture = glm::clamp(texture, glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));

                            texture = texture + ambientT;

                            texture = glm::clamp(texture, glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));

                            q.setPixelColor(j, i, QColor(texture[0], texture[1], texture[2]));
                        }
                    }

                }
            }

        }
    }
    return q;
}

void Rasterizer::ClearScene()
{
    m_polygons.clear();
}
