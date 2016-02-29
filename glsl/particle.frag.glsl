#version 150
// ^ Change this to version 130 if you have compatibility issues

in vec3 afs_Color;

out vec3 out_Color;

void main()
{
    out_Color = afs_Color;
}
