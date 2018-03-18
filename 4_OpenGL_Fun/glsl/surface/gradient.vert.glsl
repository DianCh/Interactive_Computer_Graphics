#version 150

uniform mat4 u_Model;
uniform mat3 u_ModelInvTr;
uniform mat4 u_View;
uniform mat4 u_Proj;

uniform vec4 u_Eye;         // The camera's position in world coordinates

in vec4 vs_Pos;
in vec4 vs_Nor;

out vec4 fs_Nor;
out vec4 fs_LightVec;       // The direction of the virtual light source

void main()
{
    // TODO Homework 4
    // Compute the norm to pass to fragment shader
    fs_Nor = normalize(vec4(u_ModelInvTr * vec3(vs_Nor), 0));

    // Compute model position
    vec4 modelposition = u_Model * vs_Pos;

    // Get the light vector from the object to the light
    fs_LightVec = u_Eye - modelposition;

    // Convert model to screen space
    gl_Position = u_Proj * u_View * modelposition;
}
