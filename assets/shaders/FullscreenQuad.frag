/*******************************************************************************/
/*!
\par        Image Engine
\file       FullscrrenQuad.frag

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 24, 2023

\brief      Fragment shader for rendering fullscreen quad.

            This shader samples a texture based on the provided texture coordinates.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

in vec2 fragTexCoord; // Received texture coordinate from vertex shader

uniform sampler2D screenTex;

out vec4 fragColor;

void main() {
    fragColor = texture(screenTex, fragTexCoord);
}