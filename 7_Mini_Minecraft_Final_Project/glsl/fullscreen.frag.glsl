#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform sampler2D u_Texture;
uniform int u_Time;

uniform int u_DeformWater;
uniform int u_DeformLava;

in vec2 fs_UV;

out vec4 out_Col;

void main()
{
    vec2 UV = fs_UV;

    if (u_DeformWater == 1)
    {
        UV[0] = UV[0] + 0.01 * cos(u_Time / 23.0 * 1.3);
    }

    if (u_DeformLava == 1)
    {
        vec2 fromCenter = fs_UV - vec2(0.5, 0.5);
        float length = sqrt(fromCenter[0] * fromCenter[0] + fromCenter[1] * fromCenter[1]);

        UV = vec2(0.5, 0.5) + fromCenter * (1 + 0.05 * cos(u_Time / 23.0 * 1.3 + length));
    }

    out_Col = texture(u_Texture, UV);

}
