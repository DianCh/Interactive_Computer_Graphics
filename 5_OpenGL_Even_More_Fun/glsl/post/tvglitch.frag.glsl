#version 150
// noOp.vert.glsl:
// A fragment shader used for post-processing that simply reads the
// image produced in the first render pass by the surface shader
// and outputs it to the frame buffer


in vec2 fs_UV;

out vec3 color;

uniform sampler2D u_RenderedTexture;
uniform int u_Time;

vec2 glitch(vec2 uv)
{
    // Make horizontal displacement
    float displacement = cos(fs_UV.y * 12) * 0.2;

    // Create bands of displacement
    if ((uv.y > 0.1 && uv.y < 0.2) || (uv.y > 0.25 && uv.y < 0.42) || (uv.y > 0.6 && uv.y < 0.9))
    {
        uv = uv - vec2(displacement, 0);
    }
    if (uv.y > 0.45 && uv.y < 0.52)
    {
        uv = uv + vec2(displacement, 0);
    }

    // Return displaced uv coordinates
    return uv;
}
void main()
{
    vec2 uv = fs_UV;

    // Perform glitch once a while
    if ((u_Time % 200 < 20) || (u_Time % 200 > 40 && u_Time % 200 < 50))
    {
        uv = glitch(fs_UV);
    }

    // Output color
    color = texture(u_RenderedTexture, uv).rgb;
}
