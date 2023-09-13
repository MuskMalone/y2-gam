#version 450 core

layout(location = 0) out vec4 fragColor;

in vec4 v_Color;

void main(){
	fragColor = v_Color;
}