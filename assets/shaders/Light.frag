#version 450 core

layout(location = 0) out vec4 fragColor;

flat in vec4 v_Color;
flat in mat4 v_inverseTexViewProjMtx;
flat in float v_radius;
flat in vec2 v_origin;
in vec2 fragTexCoord;

uniform sampler2D screenTex; //texture to apply postprocessing
void main(){
    vec4 texColor = texture(screenTex, fragTexCoord);
    vec2 fragWorldCoord = vec2(v_inverseTexViewProjMtx * vec4(fragTexCoord, 0, 1));

    vec2 diff = fragWorldCoord - v_origin;
    float distSquared = dot(diff, diff);
    if (distSquared < v_radius * v_radius){
        float dis = sqrt(distSquared);
        float t = dis / v_radius;
        t = 1.0 - t; // Quadratic falloff
        vec4 finalColor = v_Color * t;
        fragColor = texColor * finalColor;
    }else discard;
}