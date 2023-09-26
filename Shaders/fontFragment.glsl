#version 450 core

in vec2 TexCoords;
layout(location = 0) out vec4 color;

uniform sampler2D uText;
uniform vec3 uTextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uText, TexCoords).r);
    color = vec4(uTextColor, 1.0) * sampled;
}
