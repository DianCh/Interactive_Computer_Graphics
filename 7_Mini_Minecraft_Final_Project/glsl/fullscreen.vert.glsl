#version 150
// ^ Change this to version 130 if you have compatibility issues

in vec4 vs_Pos;
in vec4 vs_UV;
in vec4 vs_Nor;

out vec2 fs_UV;

void main()
{
    fs_UV = vec2(vs_UV[0], vs_UV[1]);
    vec4 placeholder = vs_Nor;

    gl_Position = vs_Pos;
}
