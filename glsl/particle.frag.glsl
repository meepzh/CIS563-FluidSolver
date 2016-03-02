#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform sampler2D u_BillboardTextureSampler;

in vec3 afs_Color;
out vec2 afs_UV;

out vec4 out_Color;

void main()
{
    //out_Color = texture(u_BillboardTextureSampler, afs_UV) * vec4(afs_Color, 1.f);
    out_Color = vec4(0, 0, 1, 1);
}
