#version 150

in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;

void main()
{
    // TODO Homework 5
    // Retrieve the color from texture map
    vec3 originalColor = texture(u_RenderedTexture, fs_UV).rgb;

    // Compose the greyscale value
    float grey = dot(vec3(0.21, 0.72, 0.07), originalColor);

    // Compute the distance from the center, and the attenuating coefficient accordingly
    float distFromCenter = sqrt(dot((fs_UV - vec2(0.5, 0.5)), (fs_UV - vec2(0.5, 0.5))));
    float attenuate = 1 / (1 + 5 * pow(distFromCenter, 2));

    // Output the final color
    color = vec3(1, 1, 1) * grey * attenuate;
}
