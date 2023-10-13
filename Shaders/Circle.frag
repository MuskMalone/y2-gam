/*******************************************************************************/
/*!
\par        Image Engine
\file       Line.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Fragment shader for basic color rendering.

            This shader takes in vertex colors and outputs them directly to the
            fragment color. It's a simple pass-through shader for color values.

\usage      Attach to a shader program to render objects with vertex colors.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) out vec4 fragColor;

in vec4 v_Color;

void main(){
	fragColor = v_Color;
}