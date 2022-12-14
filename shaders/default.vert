#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coor;

out vec2 texture_coord;

void main()
{
    gl_Position = vec4(pos, 1.0f);
    texture_coord = tex_coor;
}
