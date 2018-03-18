#version 150

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;

int N = 80;

// Basic noise generator
vec2 random2(vec2 p)
{
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p,vec2(269.5, 183.3))))*43758.5453);
}

// Worley noise generator
vec2 worleyNoisePoint(vec2 uv)
{
    int indexX = int(uv.x * N);
    int indexY = int(uv.y * N);

    // Get the indices of total 9 cells of interest. Clamp to duplicated cells if one goes out of bounds
    int nearbyCellX[3] = int[3](int(clamp(indexX - 1, 0, N - 1)), indexX, int(clamp(indexX + 1, 0, N - 1)));
    int nearbyCellY[3] = int[3](int(clamp(indexY - 1, 0, N - 1)), indexY, int(clamp(indexY + 1, 0, N - 1)));

    // Initialize the results of comparison
    float minDistance = 99;
    float distance = 1;
    vec2 thisControlPoint;

    // Generate random control points in 9 cells and find the closest one to this fragment
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Generate random control point for this cell
            vec2 ctrlP = random2(vec2(nearbyCellX[i], nearbyCellY[j]))
                         + vec2(nearbyCellX[i], nearbyCellY[j]);

            // Add some time-varying random movements
            vec2 randomDirection = random2(ctrlP);
            float randomPhase = randomDirection[0] * 3.1415;

            // Compute the distance to this control point and determine the control point
            // this fragment belongs to
            distance = sqrt(dot(ctrlP - uv * N, ctrlP - uv * N));
            if (distance < minDistance)
            {
                minDistance = distance;
                thisControlPoint = ctrlP;
            }
        }
    }

    // Return the coordinates of the control point of this fragment
    return thisControlPoint;
}

void main()
{
    // TODO Homework 5
    // Find this fragment's control point
    vec2 controlPoint = worleyNoisePoint(fs_UV) / N;

    // Retrieve the texture at this fragment's control point
    vec3 controlPointColor = texture(u_RenderedTexture, controlPoint).rgb;

    // Modify the color using combined light intensity
    color = controlPointColor;
}
