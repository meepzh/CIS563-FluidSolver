#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraRight;
uniform vec3 u_CameraUp;
uniform float u_ParticleSize;

in vec3 avs_Billboard;
in vec3 avs_Color;
in vec3 avs_Position;

out vec3 afs_Color;
out vec2 afs_UV;

void main()
{
    afs_Color = avs_Color;

    vec3 modelPosition = avs_Position +
    					 u_CameraRight * avs_Billboard.x * u_ParticleSize +
    					 u_CameraUp * avs_Billboard.y * u_ParticleSize;
    gl_Position = u_ViewProjection * vec4(modelPosition, 1.f);

    afs_UV = avs_Billboard.xy + vec2(0.5, 0.5);
}
