#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

in vec4 fs_Col;
uniform int u_Time;
uniform int u_TickingFlag;

out vec4 out_Col;

void main()
{

    out_Col = fs_Col;

    if (u_TickingFlag == 1)
    {
        out_Col[3] = abs(cos(u_Time / 23.0 * 1.3)) * out_Col[3];
    }

}
