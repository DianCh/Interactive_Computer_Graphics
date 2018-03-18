#version 150

uniform ivec2 u_Dimensions;
uniform int u_Time;

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;

int N = 9;

// Basic noise generator
vec2 random2(vec2 p)
{
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p,vec2(269.5, 183.3))))*43758.5453);
}

// Worley noise generator
float worleyNoiseBubble(vec2 uv)
{
    int indexX = int(uv.x * N);
    int indexY = int(uv.y * N);

    // Get the indices of total 9 cells of interest. Clamp to duplicated cells if one goes out of bounds
    int nearbyCellX[3] = int[3](int(clamp(indexX - 1, 0, N - 1)), indexX, int(clamp(indexX + 1, 0, N - 1)));
    int nearbyCellY[3] = int[3](int(clamp(indexY - 1, 0, N - 1)), indexY, int(clamp(indexY + 1, 0, N - 1)));

    // Initialize the results of comparison
    float minDistance = 99;
    float distance = 1;

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

            ctrlP.x = ctrlP.x + (randomDirection.x * sin(u_Time / 32.9) + 1) / 2;
            ctrlP.y = ctrlP.y + (randomDirection.y * sin(u_Time / 32.9 + randomPhase) + 1) / 2;
            ctrlP = clamp(ctrlP, vec2(nearbyCellX[i], nearbyCellY[j]), vec2(nearbyCellX[i] + 1, nearbyCellY[j] + 1));

            // Compute the distance to this control point and determine the min distance
            distance = sqrt(dot(ctrlP - uv * N, ctrlP - uv * N));
            if (distance < minDistance)
            {
                minDistance = distance;
            }
        }
    }

    // Use the distance to control point to generate a sphere, and use the height
    // at current position as the final worley noise output. In this way we can
    // generate "bubbles" touching each other on the border.
    float R = 5;
    float height = sqrt(R * R - minDistance * minDistance);

    return height;
}

void main()
{
    // TODO Homework 5
    // Record the delta to make in x & y direction using width & height
    float dx = 1.0 / u_Dimensions[0];
    float dy = 1.0 / u_Dimensions[1];

    float worleyCenter = worleyNoiseBubble(fs_UV);

    // Compute horizontal worley noise gradient
    float worleyRight = 2 - worleyNoiseBubble(vec2(dx, 0) + fs_UV);
    float worleyLeft = 2 - worleyNoiseBubble(vec2(-dx, 0) + fs_UV);
    float gradientX = (worleyRight - worleyLeft) / (2 * dx);

    float worleyDown = 2 - worleyNoiseBubble(vec2(0, dy) + fs_UV);
    float worleyUp = 2 - worleyNoiseBubble(vec2(0, -dy) + fs_UV);
    float gradientY = (worleyDown - worleyUp) / (2 * dy);

    // Use the gradient information to fake a "surface normal"
    vec3 norm = vec3(gradientX, gradientY, 1);

    // Fake the light condition for Lambertian lighting and Blinn-Phong Effect
    // Use a point light source
    vec3 cameraPos = vec3(0.5, 0.5, 2);
    vec3 lightPos = vec3(0.55, 0.8, 1);
    vec3 fragPos = vec3(fs_UV, 0);
    vec3 View = cameraPos - fragPos;
    vec3 Light = lightPos - fragPos;
    vec3 Halfway = (View + Light) / 2;

    // Compute the coefficients for different light effects
    float specularTerm = max(pow(dot(normalize(Halfway), normalize(norm)), 30), 0);
    float diffuseTerm = clamp(dot(normalize(norm), normalize(Light)), 0, 1);
    float ambientTerm = 0.4;
    float lightIntensity = specularTerm + diffuseTerm + ambientTerm;

    // Retrieve the texture using modified uv coordinates to fake light refraction through bubbles
    vec3 originalColor = texture(u_RenderedTexture, fs_UV - vec2(gradientX, gradientY) / (N * N)).rgb;

    // Construct the parameters for the palette
    vec3 a = vec3(0.2, 0.3, 0.4);
    vec3 b = vec3(0.8, 0.7, 0.8);
    vec3 c = vec3(1.6, 1.4, 2.7);
    vec3 d = vec3(0.15, 0.08, 0.21);

    // Compute the palette colors
    vec3 bubbleColor = a + b * cos(6.28318 * (c * diffuseTerm) + d);
    bubbleColor = clamp(bubbleColor, vec3(0, 0, 0), vec3(1, 1, 1));

    // Add palette color to bubbles to fake slight "rainbow" effect
    vec3 compoundColor = 0.96 * originalColor + 0.04 * bubbleColor;

    // Modify the color using combined light intensity
    color = clamp(compoundColor * (0.5 + 0.5 * lightIntensity), vec3(0, 0, 0), vec3(1, 1, 1));
}
