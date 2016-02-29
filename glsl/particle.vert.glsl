#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraRight;
uniform vec3 u_CameraUp;

in vec3 avs_Color;
in vec3 avs_Position;

out vec3 afs_Color;

void main()
{
    afs_Color = avs_Color;
    vec4 modelPosition = vec4(avs_Position, 1);
    gl_Position = u_ViewProjection * modelPosition;
}
