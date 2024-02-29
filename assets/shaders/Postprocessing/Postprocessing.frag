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
uniform float time;
vec2 resolution = vec2(1600, 900);

out vec4 fragColor;

void main() {
    vec2 uv = fragTexCoord;
    vec4 texColor = texture(screenTex, fragTexCoord);
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    // Circle properties
    float radius = 0.5 + sin(time) * 0.25; // Circle radius, oscillating between 0.25 and 0.75
    vec2 center = vec2(0.0, 0.0); // Center of the circle

    // Determine if the current pixel is inside the circle
    float dist = distance(uv, center);
    bool insideCircle = dist < radius;

    if (insideCircle) {
        // Invert colors inside the circle
        fragColor = vec4(1.0 - texColor.rgb, texColor.a);
    } else {
        // Outside the circle, render normally
        fragColor = texColor;
    }

    //fragColor = vec4(1.0 - texColor.rgb, texColor.a);
}