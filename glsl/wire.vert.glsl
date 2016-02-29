#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

in vec4 avs_Position;

void main()
{
    vec4 modelPosition = uModel * avs_Position;
    gl_Position = uViewProjection * modelPosition;
}
