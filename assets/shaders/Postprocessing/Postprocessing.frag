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
#define FREQUENCY 8

uniform sampler2D screenTex; //texture to apply postprocessing
uniform float time;
uniform vec2 resolution;
uniform float radius;
uniform vec2 circleCenter;

//vec2 resolution = vec2(1600, 900);

in vec2 fragTexCoord; 
out vec4 fragColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float c){
	return rand(vec2(c,1.0));
}


// Generate some blotches.
float randomBlotch(float seed)
{
	float x = rand(seed);
	float y = rand(seed+1.0);
	float s = 0.01 * rand(seed+2.0);
	
	vec2 p = vec2(x,y) - fragTexCoord;
	p.x *= resolution.x / resolution.y;
	float a = atan(p.y,p.x);
	float v = 1.0;
	float ss = s*s * (sin(6.2831*a*x)*0.1 + 1.0);
	
	if ( dot(p,p) < ss ) v = 0.2;
	else
		v = pow(dot(p,p) - ss, 1.0/16.0);
	
	return mix(0.3 + 0.2 * (1.0 - (s / 0.02)), 1.0, v);
}

float randomLine(float seed)
{
	float b = 0.01 * rand(seed);
	float a = rand(seed+1.0);
	float c = rand(seed+2.0) - 0.5;
	float mu = rand(seed+3.0);
	
	float l = 1.0;
	
	if ( mu > 0.2)
		l = pow(  abs(a * fragTexCoord.x + b * fragTexCoord.y + c ), 1.0/8.0 );
	else
		l = 2.0 - pow( abs(a * fragTexCoord.x + b * fragTexCoord.y + c), 1.0/8.0 );				
	
	return mix(0.5, 1.0, l);
}

vec4 applyFilmEffect(vec4 color) {
    vec4 finalColor = color;
	float t = float(int(time * FREQUENCY));
		
	//Create a vignetting effect
	float vI = 16.0 * (fragTexCoord.x * (1.0-fragTexCoord.x) * fragTexCoord.y * (1.0-fragTexCoord.y));

	//Brighten
	vI += 0.5;	
	//Add a fixed vignetting
	vI *= pow(16.0 * fragTexCoord.x * (1.0-fragTexCoord.x) * fragTexCoord.y * (1.0-fragTexCoord.y), 0.4);

	//Add random lines
	int l = int(8.0 * rand(t+7.0));
	float lineEffect = 1.0;
	for (int i = 0; i < l; ++i) {
		lineEffect *= randomLine(t + 6.0 + 17.0 * float(i));
	}
	
	lineEffect = clamp(lineEffect, 0.0, 0.8);
	finalColor.xyz = mix(finalColor.xyz, finalColor.xyz *  lineEffect, 0.9);

	//Add random blotches
	int s = int( max(8.0 * rand(t+18.0) -2.0, 0.0 ));
	float blotchEffect = 1.0;
	for (int i = 0; i < s; ++i) {
		blotchEffect *= randomBlotch( t+6.0+19.* float(i));
	}

	finalColor.xyz = mix(finalColor.xyz, finalColor.xyz *  blotchEffect, 1.0);

	finalColor.xyz *= vI;
		
	// Add film grain
    finalColor.xyz *= (1.0+(rand(fragTexCoord+t*.01)-.2)*.15);		
	return finalColor;
}

void main() {

    vec2 uv = fragTexCoord;
    vec4 texColor = texture(screenTex, fragTexCoord);
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    vec4 finalColor = texColor;

	applyFilmEffect(finalColor);

    //circle properties
    //float radius = sin(time * 2.0) * 2.5; //circle radius
	float maxRadius = 2.6;
	float growthRate = 2.f;
    vec2 center = circleCenter; //position of player

    //glow properties
    float glowRadius = radius + 0.1;
    float glowIntensity = 0.8 ;

    //determine if the current pixel is inside the circle
    float dist = distance(uv, center);

    if (radius > 0.0) {
        if (dist < radius) {
            float grey = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));
            finalColor = vec4(grey, grey, grey, texColor.a);
        }
        else if (dist < glowRadius) {
            //calculate glow factor based on distance from the circle edge
            float glowFactor = smoothstep(glowRadius, radius, dist);
    
            //blend between the normal color and the grey color based on glow factor
            vec4 glowColor = mix(texColor, vec4(0.94, 0.90, 0.55, texColor.a), glowFactor * glowIntensity);
    
            //apply the glow color
            finalColor = glowColor;
        } else {
            //outside the circle, render normally
            finalColor = texColor;
        }
    } else {
        //radius 0
        finalColor = texColor;
    }

    fragColor = applyFilmEffect(finalColor);
	//fragColor = texColor;
}