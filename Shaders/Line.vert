#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;
		
uniform mat4 u_ViewProjMtx;

void main(){
	v_Color = a_Color;
	gl_Position = u_ViewProjMtx * vec4(a_Position, 1.0);
} 