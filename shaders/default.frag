#version 410 core

in vec2 texture_coord;

out vec4 color;

uniform sampler2D textureUnit;

void main()
{
    color = texture(textureUnit, texture_coord);
}
