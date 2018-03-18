#version 330

uniform sampler2D u_Texture; // The texture to be read from by this shader

in vec4 fs_Nor;
in vec4 fs_LightVec;

layout(location = 0) out vec3 out_Col;

void main()
{
    // TODO Homework 4
    // Construct the parameters for the palette
    vec3 a = vec3(0.2, 0.3, 0.4);
    vec3 b = vec3(0.8, 0.7, 0.8);
    vec3 c = vec3(1.6, 1.4, 2.7);
    vec3 d = vec3(0.15, 0.08, 0.21);

    // Compute the Lambertian dot product
    float t = dot(normalize(fs_Nor), normalize(fs_LightVec));

    // Compute the colors
    vec3 Col = a + b * cos(6.28318 * (c * t) + d);

    // Clamp to final output
    out_Col = clamp(Col, vec3(0, 0, 0), vec3(1, 1, 1));

}
