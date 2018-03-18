#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please
// open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute
// the shading of geometry. For every pixel in your program's output
// screen, the fragment shader is run for every bit of geometry that
// particular pixel overlaps. By implicitly interpolating the position
// data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex
// position, light position, and vertex color.

uniform vec4 u_Color; // The color with which to render this instance of geometry.

uniform sampler2D u_Texture;
uniform int u_Time;
uniform vec4 u_EyePos;

// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;
in vec4 fs_Pos;

in vec2 fs_UV;
in float fs_Cosine;
flat in ivec2 fs_Flag;

out vec4 out_Col; // This is the final output color that you will see on your
                                       // screen for the pixel that is currently being processed.

void main()
{

    // Material base color (before shading)
    vec2 UV = fs_UV;


    // If flag 0 is set non-zero, perform animation using u_Time
    if (fs_Flag[0] == 1)
    {
        vec2 UV_lowerleft = vec2(floor(fs_UV[0] * 16), floor(fs_UV[1] * 16)) / 16.0;

        vec2 UV_displaced = (UV + u_Time / 2333.0 * vec2(1.5, 1)) * 16;
        vec2 UV_local = vec2(UV_displaced[0] - floor(UV_displaced[0]),
                             UV_displaced[1] - floor(UV_displaced[1])) / 16.0;

        UV = UV_local + UV_lowerleft;

    }

    vec4 diffuseColor = texture(u_Texture, UV);

    // Calculate the diffuse term for Lambert shading
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));

    // Calculate the specular term for Blinn-Phon
    vec4 View = u_EyePos - fs_Pos;
    vec4 Light = fs_LightVec;
    vec4 Halfway = (View + Light) / 2;

    float specularTerm = max(pow(dot(normalize(Halfway), normalize(fs_Nor)), fs_Cosine), 0);

    // Avoid negative lighting values
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    float ambientTerm = 0.2;

    float lightIntensity = diffuseTerm + ambientTerm + specularTerm;   //Add a small float value to the color multiplier
                                                        //to simulate ambient lighting. This ensures that faces that are not
                                                        //lit by our point light are not completely black.

    // Compute final shaded color
    out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a);

    // fog effect
    float d = distance(u_EyePos.xyz, fs_Pos.xyz);
    if(d > 32){
        float e = clamp(d / 128.f, 0, 0.9);
        out_Col = (1 - e) * out_Col + e * vec4(1, 1, 1, 1);
    }
}
