/*******************************************************************************/
/*!
\par        Image Engine
\file       Postprocessing.vert

\author     Xavier Choa (k.choa@digipen.edu)
\date       Feb 28, 2024

\brief      Vertex shader for postprocessing.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/

#version 450 core

layout(location = 0) in vec2 in_Position; // Vertex position
layout(location = 1) in vec2 in_TexCoord; // Texture coordinate

out vec2 fragTexCoord; // Pass texture coordinate to fragment shader

void main() {
    fragTexCoord = in_TexCoord;
    gl_Position = vec4(in_Position, 0.0, 1.0);
}