#version 150

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;
uniform int u_Time;
uniform ivec2 u_Dimensions;

void main()
{
    // TODO Homework 5
    // Record the delta to make in x & y direction using width & height
    float dx = 1.0 / u_Dimensions[0];
    float dy = 1.0 / u_Dimensions[1];

    // Construct the horizontal & vertical sobel kernels
    const float sobelHorizontal[9] = float[9](3, 0, -3,
                                              10, 0, -10,
                                              3, 0, -3);
    const float sobelVertical[9] = float[9](3, 10, 3,
                                            0, 0, 0,
                                            -3, -10, -3);

    // Initalize the horizontal & vertical gradients
    float gradientHRed = 0.0;
    float gradientVRed = 0.0;
    float gradientHGreen = 0.0;
    float gradientVGreen = 0.0;
    float gradientHBlue = 0.0;
    float gradientVBlue = 0.0;

    // Retrieve the color values and compute the gradients
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j<= 1; j++)
        {
            vec3 c = texture(u_RenderedTexture, fs_UV + vec2(j * dx, i * dy)).rgb;
            gradientHRed += sobelHorizontal[j + 1 + (i + 1) * 3] * c.r;
            gradientVRed += sobelVertical[j + 1 + (i + 1) * 3] * c.r;

            gradientHGreen += sobelHorizontal[j + 1 + (i + 1) * 3] * c.g;
            gradientVGreen += sobelVertical[j + 1 + (i + 1) * 3] * c.g;

            gradientHBlue += sobelHorizontal[j + 1 + (i + 1) * 3] * c.b;
            gradientVBlue += sobelVertical[j + 1 + (i + 1) * 3] * c.b;
        }
    }

    // Use the length of the gradients as color value in each channel
    color = vec3(sqrt(gradientHRed * gradientHRed + gradientVRed * gradientVRed),
                 sqrt(gradientHGreen * gradientHGreen + gradientVGreen * gradientVGreen),
                 sqrt(gradientHBlue * gradientHBlue + gradientVBlue * gradientVBlue));
}
