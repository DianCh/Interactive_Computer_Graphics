#version 150

uniform mat4 u_Model;
uniform mat3 u_ModelInvTr;
uniform mat4 u_View;
uniform mat4 u_Proj;

uniform int u_Time;

in vec4 vs_Pos;
in vec4 vs_Nor;
in vec2 vs_UV;

out vec4 fs_Pos;
out vec3 fs_Nor;
out vec2 fs_UV;

// ===vvvvv Make use of the noise functions in homework description
float mod289(float x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 perm(vec4 x)
{
    return mod289(((x * 34.0) + 1.0) * x);
}

float noise(vec3 p)
{
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = vec4(a.xxy, a.y) + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + vec4(a.zzz, a.z);
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}
// ===^^^^^ Make use of the noise functions in homework description

void main()
{
    // TODO Homework 4
    // In this shader we're deform the object into a wiggling ball inflated and squeezed in different directions
    fs_Nor = normalize(u_ModelInvTr * vec3(vs_Nor));

    // Get the unit vector from origin to this point
    vec3 direction = normalize(vec3(vs_Pos));

    // Define the radius of the sphere
    float rou = 5;

    // Compute the offsets in each directon
    vec4 offset = vec4(rou * direction - vec3(vs_Pos), 1);

    // Deform the offsets in time domain
    offset[0] = offset[0] * 0.5 * (sin(u_Time / 9.9) + 1);

    offset[1] = offset[1] * 0.5 * (sin(u_Time / 9.9 + 3.1415 / 4) + 1);

    offset[2] = offset[2] * 0.5 * (cos(u_Time / 9.9) + 1);

    // Apply the offsets
    vec4 offsetP = vs_Pos + vec4(vec3(offset), 0);

    // Add some random noise to each direction
    vec3 offsetNoises = vec3(noise(offset.xzy), noise(offset.zyy), noise(offset.yzx));

    // Output the final deformed position
    offsetP = offsetP + vec4(offsetNoises, 0);

    fs_Pos = offsetP;
    vec4 modelposition = u_Model * offsetP;

    gl_Position = u_Proj * u_View * modelposition;

    // Still pass uv coordinates
    fs_UV = vs_UV;
}
