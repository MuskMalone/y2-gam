/*******************************************************************************/
/*!
\par        Image Engine
\file       Postprocessing.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       8 March, 2024

\brief      Fragment shader for postprocessing. Includes vintage film effect and
			time slow grey scale effect

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core
uniform float time;
uniform sampler2D screenTex; //texture to apply postprocessing
in vec2 fragTexCoord; 
out vec4 fragColor;

void main() {
    vec4 texColor = texture(screenTex, fragTexCoord);

	fragColor = texColor + vec4(time - time);
}