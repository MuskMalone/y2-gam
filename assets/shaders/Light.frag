/*******************************************************************************/
/*!
\par        Image Engine
\file       Light.frag

\author     tan cheng hian t.chenghian
\date       Oct 13, 2023

\brief      

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/
#version 450 core

layout(location = 0) out vec4 fragColor;

flat in vec4 v_Color;
flat in mat4 v_inverseTexViewProjMtx;
flat in float v_radius;
flat in vec2 v_origin;
flat in float v_intensity;
in vec2 fragTexCoord;

uniform sampler2D screenTex; //texture to apply postprocessing
void main(){
    if (gl_FragCoord.z < 0.5) discard;
    vec2 fragWorldCoord = vec2(v_inverseTexViewProjMtx * vec4(fragTexCoord, 0, 1));

    vec2 diff = fragWorldCoord - v_origin;
    float distSquared = dot(diff, diff);
    if (distSquared < v_radius * v_radius){
        float dis = sqrt(distSquared);
        vec4 texColor = texture(screenTex, fragTexCoord);
        // float t = 1.0 - smoothstep(0.0, v_radius, dis);
        // vec4 finalColor = v_Color * t;
        //vec4 startColor = mix(texColor, v_Color, v_intensity);
        fragColor = texColor / mix(vec4(vec3(1) - v_Color.xyz, 1) * v_intensity, vec4(1), dis / v_radius);
    }else discard;  
    memoryBarrier();
}