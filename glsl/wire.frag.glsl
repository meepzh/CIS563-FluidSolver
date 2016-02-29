#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform vec4 u_Color;

out vec4 out_Color;

void main()
{
    out_Color = u_Color;
}
