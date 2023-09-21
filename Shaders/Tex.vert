#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIdx;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIdx;
		
uniform mat4 u_ViewProjMtx;
//uniform mat4 u_MdlViewMtx;

void main(){
	//gl_Position = u_ViewProjMtx * u_MdlViewMtx * vec4(a_Position, 1.0);
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIdx = a_TexIdx;
	gl_Position = u_ViewProjMtx * vec4(a_Position, 1.0);
} 