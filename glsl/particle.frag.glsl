#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform sampler2D u_BillboardTextureSampler;

in vec3 afs_Color;
in vec2 afs_UV;

out vec4 out_Color;

void main()
{
	vec4 tex_Color = texture(u_BillboardTextureSampler, afs_UV);
	vec4 in_Color = vec4(afs_Color, 1.f);
    out_Color = tex_Color * in_Color;
}
