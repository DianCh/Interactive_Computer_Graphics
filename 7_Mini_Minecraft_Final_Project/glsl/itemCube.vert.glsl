#version 150
// ^ Change this to version 130 if you have compatibility issues

//This is a vertex shader. While it is called a "shader" due to outdated conventions, this file
//is used to apply matrix transformations to the arrays of vertex data passed to it.
//Since this code is run on your GPU, each vertex is transformed simultaneously.
//If it were run on your CPU, each vertex would have to be processed in a FOR loop, one at a time.
//This simultaneous transformation allows your program to run much faster, especially when rendering
//geometry with millions of vertices.

uniform mat4 u_Model;       // The matrix that defines the transformation of the
                            // object we're rendering. In this assignment,
                            // this will be the result of traversing your scene graph.
uniform ivec2 u_Dimension;

uniform mat4 u_ViewProj;

in vec4 vs_Pos;
in vec4 vs_UV;
in ivec2 vs_Flag;

out vec4 fs_UV;

void main()
{
    fs_UV = vs_UV;

    vec4 modelPos = u_Model * vs_Pos;

//    float scaleFactor = 0.2;
//    modelPos.x *= scaleFactor;
//    modelPos.y *= scaleFactor;

//    if(vs_Flag.x == 1) {
//        modelPos.y += 0.05;
//        modelPos.x += 0;
//    }
    modelPos = u_ViewProj * modelPos;
    modelPos.x *= 0.8;

    modelPos.x += 5;
    modelPos.y += 5;
    gl_Position = modelPos;
}




