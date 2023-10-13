/*******************************************************************************/
/*!
\par        Image Engine
\file       Circle.vert

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 13, 2023

\brief      Vertex shader for circles.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;

out vec3 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness; 
out float v_Fade;
		
uniform mat4 u_ViewProjMtx;

void main(){

	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	gl_Position = u_ViewProjMtx * vec4(a_WorldPosition, 1.0);
} 