#version 330

//This is a fragment shader. If you've opened this file first, please open and read lambert.vert.glsl before reading on.
//Unlike the vertex shader, the fragment shader actually does compute the shading of geometry.
//For every pixel in your program's output screen, the fragment shader is run for every bit of geometry that particular pixel overlaps.
//By implicitly interpolating the position data passed into the fragment shader by the vertex shader, the fragment shader
//can compute what color to apply to its pixel based on things like vertex position, light position, and vertex color.

uniform sampler2D u_Texture; // The texture to be read from by this shader

//These are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec2 fs_UV;

in vec4 fs_CameraPos;
in vec4 fs_Pos;

layout(location = 0) out vec3 out_Col;//This is the final output color that you will see on your screen for the pixel that is currently being processed.

void main()
{
    // TODO Homework 4
    // Retrieve the base color from texture
    vec4 diffuseColor = texture(u_Texture, fs_UV);

    // Compute relevant vectors for BlinnPhong
    vec4 View = fs_CameraPos - fs_Pos;
    vec4 Light = fs_LightVec;
    vec4 Halfway = (View + Light) / 2;

    // Specular term for BlinnPhong
    float specularTerm = max(pow(dot(normalize(Halfway), normalize(fs_Nor)), 30), 0);

    // Diffuse term for Lambertian
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    // Ambient term
    float ambientTerm = 0.2;

    // Compute the combined coefficients
    float lightIntensity = specularTerm + diffuseTerm + ambientTerm;

    out_Col = vec3(diffuseColor.rgb * lightIntensity);
}
