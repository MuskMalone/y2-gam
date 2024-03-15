/*******************************************************************************/
/*!
\par        Image Engine
\file       Light.vert

\author     tan cheng hian t.chenghian
\date       Oct 13, 2023

\brief      

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) in vec2 origin;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 pos;
layout(location = 3) in float radius;
layout(location = 4) in float intensity;

flat out vec4 v_Color;
flat out mat4 v_inverseTexViewProjMtx;
flat out float v_radius;
flat out vec2 v_origin;
flat out float v_intensity;
out vec2 fragTexCoord;

uniform mat4 u_ViewProjMtx;
mat4 convertMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);
void main(){
    v_intensity = intensity;
    v_radius = radius;
    v_origin = origin;
	v_Color = vec4(color, 1.0);
    mat4 concatMat = convertMatrix * u_ViewProjMtx;
    v_inverseTexViewProjMtx = inverse(concatMat);
    fragTexCoord = vec2(concatMat * vec4(pos,0.0, 1.0));

	gl_Position = u_ViewProjMtx * vec4(pos,0.0, 1.0);
} 