/*******************************************************************************/
/*!
\par        Image Engine
\file       Line.vert

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Vertex shader for transforming object vertices and passing through 
            vertex colors.

            This shader takes in vertex positions and colors, applies a view 
            projection transformation to the positions, and passes the colors 
            directly to the fragment shader.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;
		
uniform mat4 u_ViewProjMtx;

void main(){
	v_Color = a_Color;
	gl_Position = u_ViewProjMtx * vec4(a_Position, 1.0);
} 