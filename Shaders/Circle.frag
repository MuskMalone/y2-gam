/*******************************************************************************/
/*!
\par        Image Engine
\file       Circle.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 13, 2023

\brief      Fragment shader for circles.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) out vec4 fragColor;

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;
in float v_Fade;

void main(){
    
    float dist = 1.0 - length(v_LocalPosition);
    vec3 clr = vec3(smoothstep(0.0, v_Fade, dist));
    clr *= vec3(smoothstep(v_Thickness + v_Fade, v_Thickness, dist));
	fragColor = vec4(clr, 1.f);
    //fragColor.rgb *= v_Color;
}