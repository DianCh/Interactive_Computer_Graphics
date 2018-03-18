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

    // Construct the Gaussian kernel using an array
    int radius = 3;
    int dim = 2 * radius + 1;
    const float kernel[49] = float[49](0.002121, 0.005461, 0.009629, 0.011633, 0.009629, 0.005461, 0.002121,
                                       0.005461, 0.014059, 0.024791, 0.029949, 0.024791, 0.014059, 0.005461,
                                       0.009629, 0.024791, 0.043715, 0.052812, 0.043715, 0.024791, 0.009629,
                                       0.011633, 0.029949, 0.052812, 0.063802, 0.052812, 0.029949, 0.011633,
                                       0.009629, 0.024791, 0.043715, 0.052812, 0.043715, 0.024791, 0.009629,
                                       0.005461, 0.014059, 0.024791, 0.029949, 0.024791, 0.014059, 0.005461,
                                       0.002121, 0.005461, 0.009629, 0.011633, 0.009629, 0.005461, 0.002121);

    // Set the threshold for blooming
    float threshold = 0.3;

    // Initialize the final values for filtering
    float deltaR = 0.0;
    float deltaG = 0.0;
    float deltaB = 0.0;

    // Retrieve the color values needed and compute filtering
    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            vec3 c = texture(u_RenderedTexture, fs_UV + vec2(j * dx, i * dy)).rgb;
            deltaR += kernel[j + radius + (i + radius) * dim] * c.r * float(c.r > threshold);
            deltaG += kernel[j + radius + (i + radius) * dim] * c.g * float(c.g > threshold);
            deltaB += kernel[j + radius + (i + radius) * dim] * c.b * float(c.b > threshold);

        }
    }

    // Add blooming to original color and output
    color = vec3(deltaR, deltaG, deltaB) + texture(u_RenderedTexture, fs_UV).rgb;
}
