/*******************************************************************************/
/*!
\par        Image Engine
\file       Postprocessing.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       Feb 28, 2024

\brief      Fragment shader for postprocessing.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

in vec2 fragTexCoord; // Received texture coordinate from vertex shader

uniform sampler2D screenTex; //texture to apply postprocessing

out vec4 fragColor;

void main() {
    vec4 texColor = texture(screenTex, fragTexCoord);

    fragColor = vec4(1.0 - texColor.rgb, texColor.a);
}