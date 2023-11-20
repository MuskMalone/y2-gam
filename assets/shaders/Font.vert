#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;
out vec2 TexCoords;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * vec4(a_Position, 0.0, 1.0);
    TexCoords = a_TexCoord;
}