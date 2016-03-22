#version 330 core

in vec3 afs_Color;

out vec4 out_Color;

void main()
{
    out_Color = vec4(afs_Color, 1.f);
}
