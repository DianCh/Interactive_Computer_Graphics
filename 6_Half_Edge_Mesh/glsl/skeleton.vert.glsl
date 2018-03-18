#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

in vec4 vs_Pos;
in vec4 vs_Col;

in ivec2 vs_IDs;                        // New feature in hw8
in vec2 vs_Weights;                     // New feature in hw8
uniform mat4 u_BindMatrices [50];        // New feature in hw8
uniform mat4 u_TransMatrices [50];       // New feature in hw8

out vec4 fs_Col;

void main()
{
    fs_Col = vs_Col;

    vec4 pos_1 = u_TransMatrices[vs_IDs[0]] * u_BindMatrices[vs_IDs[0]] * vs_Pos;
    vec4 pos_2 = u_TransMatrices[vs_IDs[1]] * u_BindMatrices[vs_IDs[1]] * vs_Pos;

    vec4 blend_pos = vs_Weights[0] * pos_1 + vs_Weights[1] * pos_2;

    vec4 modelposition = u_Model * blend_pos;


    //built-in things to pass down the pipeline
    gl_Position = u_ViewProj * modelposition;

}
