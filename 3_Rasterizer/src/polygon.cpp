// modified by Dian Chen for CIS560 homework 3

#include "polygon.h"
#include <glm/gtx/transform.hpp>

/// Definition of Triangle class
// Default constructor, automatically takes the first 3 vertices.
Triangle::Triangle() : m_indices{0, 1, 2} {}

// Parametrized constructor.
Triangle::Triangle(unsigned int v1, unsigned int v2, unsigned int v3)
    : m_indices{v1, v2, v3} {}


void Polygon::Triangulate()
{
    //TODO: Populate list of triangles
    for(unsigned int i = 0; i < m_verts.size() - 2; i++)
    {
        m_tris.push_back(Triangle{0, i + 1, i + 2});
    }
}

// Creates a polygon from the input list of vertex positions and colors
Polygon::Polygon(const QString& name, const std::vector<glm::vec4>& pos, const std::vector<glm::vec3>& col)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    for(unsigned int i = 0; i < pos.size(); i++)
    {
        m_verts.push_back(Vertex(pos[i], col[i], glm::vec4(), glm::vec2()));
    }
    //Triangulate();
    EarClipTriangulate();
}

// Creates a regular polygon with a number of sides indicated by the "sides" input integer.
// All of its vertices are of color "color", and the polygon is centered at "pos".
// It is rotated about its center by "rot" degrees, and is scaled from its center by "scale" units
Polygon::Polygon(const QString& name, int sides, glm::vec3 color, glm::vec4 pos, float rot, glm::vec4 scale)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    glm::vec4 v(0.f, 1.f, 0.f, 1.f);
    float angle = 360.f / sides;
    for(int i = 0; i < sides; i++)
    {
        glm::vec4 vert_pos = glm::translate(glm::vec3(pos.x, pos.y, pos.z))
                           * glm::rotate(rot, glm::vec3(0.f, 0.f, 1.f))
                           * glm::scale(glm::vec3(scale.x, scale.y, scale.z))
                           * glm::rotate(i * angle, glm::vec3(0.f, 0.f, 1.f))
                           * v;
        m_verts.push_back(Vertex(vert_pos, color, glm::vec4(), glm::vec2()));
    }

    Triangulate();
    //EarClipTriangulate();

}

Polygon::Polygon(const QString &name)
    : m_tris(), m_verts(), m_name(name), mp_texture(nullptr), mp_normalMap(nullptr)
{}

Polygon::Polygon()
    : m_tris(), m_verts(), m_name("Polygon"), mp_texture(nullptr), mp_normalMap(nullptr)
{}

Polygon::Polygon(const Polygon& p)
    : m_tris(p.m_tris), m_verts(p.m_verts), m_name(p.m_name), mp_texture(nullptr), mp_normalMap(nullptr)
{
    if(p.mp_texture != nullptr)
    {
        mp_texture = new QImage(*p.mp_texture);
    }
    if(p.mp_normalMap != nullptr)
    {
        mp_normalMap = new QImage(*p.mp_normalMap);
    }
}

Polygon::~Polygon()
{
    delete mp_texture;
}

void Polygon::SetTexture(QImage* i)
{
    mp_texture = i;
}

void Polygon::SetNormalMap(QImage* i)
{
    mp_normalMap = i;
}

void Polygon::AddTriangle(const Triangle& t)
{
    m_tris.push_back(t);
}

void Polygon::AddVertex(const Vertex& v)
{
    m_verts.push_back(v);
}

void Polygon::ClearTriangles()
{
    m_tris.clear();
}

Triangle& Polygon::TriAt(unsigned int i)
{
    return m_tris[i];
}

Triangle Polygon::TriAt(unsigned int i) const
{
    return m_tris[i];
}

Vertex &Polygon::VertAt(unsigned int i)
{
    return m_verts[i];
}

Vertex Polygon::VertAt(unsigned int i) const
{
    return m_verts[i];
}

glm::vec3 Polygon::ComputeBaryCords2D(const Triangle &t, const glm::vec4 &p)
{

    float s = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[t.m_indices[1]].m_pos - m_verts[t.m_indices[0]].m_pos),
                                           glm::vec3(m_verts[t.m_indices[2]].m_pos - m_verts[t.m_indices[0]].m_pos)));

    float s1 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[t.m_indices[1]].m_pos - p),
                                            glm::vec3(m_verts[t.m_indices[2]].m_pos - p)));

    float s2 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[t.m_indices[2]].m_pos - p),
                                            glm::vec3(m_verts[t.m_indices[0]].m_pos - p)));

    float s3 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[t.m_indices[0]].m_pos - p),
                                            glm::vec3(m_verts[t.m_indices[1]].m_pos - p)));

    return glm::vec3(s1 / s, s2 / s, s3 / s);
}

glm::vec3 Polygon::ComputeBaryCords3D(const glm::vec2 &v0, const glm::vec2 &v1, const glm::vec2 &v2, const glm::vec2 &p)
{
//    glm::mat3 constraintMatrix(glm::vec3(v0, 1),
//                               glm::vec3(v1, 1),
//                               glm::vec3(v2, 1));

//    return glm::inverse(constraintMatrix) * glm::vec3(p, 1);

    float s = 0.5 * glm::length(glm::cross(glm::vec3((v0 - v1), 0), glm::vec3((v0 - v2), 0)));

    float s1 = 0.5 * glm::length(glm::cross(glm::vec3((v1 - p), 0), glm::vec3((v2 - p), 0)));

    float s2 = 0.5 * glm::length(glm::cross(glm::vec3((v2 - p), 0), glm::vec3((v0 - p), 0)));

    float s3 = 0.5 * glm::length(glm::cross(glm::vec3((v0 - p), 0), glm::vec3((v1 - p), 0)));

    return glm::vec3(s1 / s, s2 / s, s3 / s);

//    float lambda1 = ((v1[1] - v2[1]) * (p[0] - v2[0]) - (v2[0] - v1[0]) * (p[1] - v2[1])) / ((v1[1] - v2[1]) * (v0[0] - v2[0]) - (v2[0] - v1[0]) * (v0[1] - v2[1]));

//    float lambda2 = ((v2[1] - v0[1]) * (p[0] - v2[0]) - (v0[0] - v2[0]) * (p[1] - v2[1])) / ((v1[1] - v2[1]) * (v0[0] - v2[0]) - (v2[0] - v1[0]) * (v0[1] - v2[1]));

//    return glm::vec3(lambda1, lambda2, 1 - lambda1 - lambda2);
}

void Polygon::EarClipTriangulate()
{
    // Find a convex point of the polygon
    int start = FindMeAConvexPoint();
    int N = m_verts.size();

    // Given a convex vertex, found the front face norm defined for this polygon
    glm::vec3 frontNorm = glm::cross((glm::vec3(m_verts[start].m_pos) - glm::vec3(m_verts[(start - 1 + N) % N].m_pos)),
                                      glm::vec3(m_verts[(start + 1) % N].m_pos) - glm::vec3(m_verts[start].m_pos));

    // Construct a dynamic list for the indices of vertices
    std::vector<unsigned int> currentList;
    for (unsigned int i = 0; i < (unsigned int)(N); i++)
    {
        currentList.push_back(i);
    }

    // Initialize the index retriever
    int j = start;

    // Loop if there are more than 3 vertices, meaning the polygon can be further triangulated
    while (N > 3)
    {
        if (isConvexPoint(frontNorm, currentList, j))
        {
            if (isFeasible(currentList, j))
            {
                m_tris.push_back(Triangle{currentList[j], currentList[(j + 1) % N], currentList[(j - 1 + N) % N]});
                currentList.erase(currentList.begin() + j);
                N = currentList.size();
                j = j % N;
            }
            else
            {
                j = (j + 1) % N;
            }
        }
        else
        {
            j = (j + 1) % N;
        }

    }
    m_tris.push_back(Triangle{currentList[j], currentList[(j + 1) % N], currentList[(j - 1 + N) % N]});
}

int Polygon::FindMeAConvexPoint()
{
    // The vertex with the largest x coordinate is guaranteed to be a convex vertex.
    // Record the number of vertices.
    int N = m_verts.size();
    for (int i = 0; i < N; i++)               // Loop over N vertices
    {
        bool flag = true;

        glm::vec4 current = ProjectToXY(m_verts[i].m_pos);

        for (int j = 1; j < N; j++)       // Compare with the other N - 1 vertices
        {
            glm::vec4 other = ProjectToXY(m_verts[(i + j) % N].m_pos);

            if (current[0] < other[0])
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            return i;
        }
    }
}

glm::vec4 Polygon::ProjectToXY(glm::vec4 worldPos)
{
    // Project the polygon's points in world coordinates into its own xy plane
    glm::vec3 polyNorm = glm::cross(glm::vec3(m_verts[0].m_pos), glm::vec3(m_verts[1].m_pos));
    polyNorm = glm::normalize(polyNorm);

    glm::vec3 worldZ(0, 0, 1);

    glm::vec3 w = glm::cross(worldZ, polyNorm);
    float theta = acos(glm::dot(worldZ, w));

    // Construct a rodriguez matrix
    glm::vec4 v1(cos(theta) + w[0] * w[0] * (1 - cos(theta)),
                 w[2] * sin(theta) + w[0] * w[1] * (1 - cos(theta)),
               - w[1] * sin(theta) + w[0] * w[2] * (1 - cos(theta)),
                 0);

    glm::vec4 v2( - w[2] * sin(theta) + w[0] * w[1] * (1 - cos(theta)),
                 cos(theta) + w[1] * w[1] * (1 - cos(theta)),
                 w[0] * sin(theta) + w[1] * w[2] * (1 - cos(theta)),
                 0);

    glm::vec4 v3(w[1] * sin(theta) + w[0] * w[2] * (1 - cos(theta)),
               - w[0] * sin(theta) + w[1] * w[2] * (1 - cos(theta)),
                 cos(theta) + w[2] * w[2] * (1 - cos(theta)),
                 0);

    glm::vec4 v4(0, 0, 0, 1);

    glm::mat4 rodriguez(v1, v2, v3, v4);

    // Convert world coordinates to the polygon's own frame.
    return glm::inverse(rodriguez) * worldPos;

}

bool Polygon::isConvexPoint(glm::vec3 &frontNorm, std::vector<unsigned int> &currentList, int thisOne)
{
    // Store the current number of vertices of interest
    int N = currentList.size();

    // Compute the cross product cross(AB, BC), with B being the current vertex and A, B being the neighbors
    glm::vec3 v1 = glm::vec3(m_verts[currentList[thisOne]].m_pos - m_verts[currentList[(thisOne - 1 + N) % N]].m_pos);
    glm::vec3 v2 = glm::vec3(m_verts[currentList[(thisOne + 1) % N]].m_pos - m_verts[currentList[thisOne]].m_pos);

    glm::vec3 thisNorm = glm::cross(v1, v2);

    // Only convex if the cross product have the same direction as front face norm
    if(glm::dot(thisNorm, frontNorm) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Polygon::isFeasible(std::vector<unsigned int> &currentList, int thisOne)
{
    // Keep the current count of vertices of interest
    int N = currentList.size();
    int v1 = thisOne;
    int v2 = (thisOne + 1) % N;
    int v3 = (thisOne - 1 + N) % N;

    // Initialize the flag to be true
    bool flag = true;

    // Check if all the other vertices lie outside the proposed triangle. If one is found to lie
    // inside, immediately return false;
    for (int i = 1; i < N - 3; i++)
    {
        int p = (thisOne + 1 + i) % N;

        float s = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[currentList[v1]].m_pos - m_verts[currentList[v3]].m_pos),
                                               glm::vec3(m_verts[currentList[v2]].m_pos - m_verts[currentList[v1]].m_pos)));

        float s1 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[currentList[v3]].m_pos - m_verts[currentList[p]].m_pos),
                                                glm::vec3(m_verts[currentList[v2]].m_pos - m_verts[currentList[p]].m_pos)));

        float s2 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[currentList[v1]].m_pos - m_verts[currentList[p]].m_pos),
                                                glm::vec3(m_verts[currentList[v3]].m_pos - m_verts[currentList[p]].m_pos)));

        float s3 = 0.5 * glm::length(glm::cross(glm::vec3(m_verts[currentList[v2]].m_pos - m_verts[currentList[p]].m_pos),
                                                glm::vec3(m_verts[currentList[v1]].m_pos - m_verts[currentList[p]].m_pos)));

        glm::vec3 baryCords(s1 / s, s2 / s, s3 / s);

        if (fabsf(baryCords[0] + baryCords[1] + baryCords[2] - 1) < EPSILON)
        {
            return false;
        }
    }
    return flag;
}

glm::vec3 GetImageColor(const glm::vec2 &uv_coord, const QImage* const image)
{
    if(image)
    {
        int X = glm::min(image->width() * uv_coord.x, image->width() - 1.0f);
        int Y = glm::min(image->height() * (1.0f - uv_coord.y), image->height() - 1.0f);
        QColor color = image->pixel(X, Y);
        return glm::vec3(color.red(), color.green(), color.blue());
    }
    return glm::vec3(255.f, 255.f, 255.f);
}

