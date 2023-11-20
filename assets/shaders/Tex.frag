/*******************************************************************************/
/*!
\par        Image Engine
\file       Tex.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Fragment shader for rendering textured objects with vertex colors.

            This shader samples a texture based on the provided texture coordinates
            and texture index. The resulting texture color is then multiplied by 
            the vertex color to produce the final fragment color.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entity; //TODO CHANGE TO ENTITYID

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIdx;
in flat int v_Entity;

uniform sampler2D u_Tex[32]; //TODO CHANGE THIS IN FUTURE

void main(){
	vec4 texColor = texture2D(u_Tex[int(v_TexIdx)], v_TexCoord);
	fragColor = texColor * v_Color;
    entity = v_Entity;
}