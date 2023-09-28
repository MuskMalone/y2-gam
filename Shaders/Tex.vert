/*******************************************************************************/
/*!
\par        Image Engine
\file       Tex.vert

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Vertex shader for rendering textured objects with vertex colors.

            This shader takes in vertex attributes such as position, color, 
            texture coordinates, and texture index. It then passes the color, 
            texture coordinates, and texture index to the fragment shader while 
            transforming the vertex position using the provided view-projection matrix.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

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