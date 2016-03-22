#version 330 core

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

in vec3 avs_Color;
in vec3 avs_Position;

out vec3 afs_Color;

void main()
{
    afs_Color = avs_Color;
    vec4 modelPosition = u_Model * vec4(avs_Position, 1);
    gl_Position = u_ViewProjection * modelPosition;
}
