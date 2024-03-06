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
#define FREQUENCY 15

uniform sampler2D screenTex; //texture to apply postprocessing
uniform float time;
uniform vec2 resolution;
uniform float radius;
uniform vec2 circleCenter;
vec2 uv;

//vec2 resolution = vec2(1600, 900);

in vec2 fragTexCoord; 
out vec4 fragColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float c){
	return rand(vec2(c,1.0));
}

float randomLine(float seed)
{
	float b = 0.01 * rand(seed);
	float a = rand(seed+1.0);
	float c = rand(seed+2.0) - 0.5;
	float mu = rand(seed+3.0);
	
	float l = 1.0;
	
	if ( mu > 0.2)
		l = pow(  abs(a * uv.x + b * uv.y + c ), 1.0/8.0 );
	else
		l = 2.0 - pow( abs(a * uv.x + b * uv.y + c), 1.0/8.0 );				
	
	return mix(0.5, 1.0, l);
}

// Generate some blotches.
float randomBlotch(float seed)
{
	float x = rand(seed);
	float y = rand(seed+1.0);
	float s = 0.01 * rand(seed+2.0);
	
	vec2 p = vec2(x,y) - uv;
	p.x *= resolution.x / resolution.y;
	float a = atan(p.y,p.x);
	float v = 1.0;
	float ss = s*s * (sin(6.2831*a*x)*0.1 + 1.0);
	
	if ( dot(p,p) < ss ) v = 0.2;
	else
		v = pow(dot(p,p) - ss, 1.0/16.0);
	
	return mix(0.3 + 0.2 * (1.0 - (s / 0.02)), 1.0, v);
}

vec4 applyFilmGrainEffect(vec4 color, vec2 uv, float time) {
    // Calculate random seed based on time
    float seed = fract(sin(dot(uv.xy, vec2(12.9898, 78.233)) + time) * 43758.5453);

    // Film grain effect logic (simplified for brevity)
    // Apply film grain effect based on the random seed and uv coordinates
    float grain = clamp(rand(seed) - 0.5, 0.0, 1.0);
    vec3 grainColor = color.rgb + grain * vec3(0.05); // Adjust grain intensity as needed

    // Combine the original color with the grain effect
    vec4 resultColor = vec4(grainColor, color.a);

    return resultColor;
}

void main() {
//	fragColor = texture(screenTex, fragTexCoord);
//	vec2 uv = fragTexCoord;
//
//	float t = float(int(time * FREQUENCY));
//		
//	// Get some image movement
//	//vec2 suv = uv + 0.002 * vec2( rand(t), rand(t + 23.0));
//	vec2 suv = uv;
//		
//	// Get the image
//	vec3 image = texture( screenTex, vec2(suv.x, suv.y) ).xyz;
//
//	// Create a time-varying vignetting effect
//	float vI = 16.0 * (uv.x * (1.0-uv.x) * uv.y * (1.0-uv.y));
//	vI *= 0.7;
//	//vI *= mix( 0.7, 1.0, rand(t + 0.5));
//
//		
//	// Add additive flicker
//	//vI += 1.0 + 0.4 * rand(t+8.);
//	vI += 1.0;
//		
//	// Add a fixed vignetting (independent of the flicker)
//	vI *= pow(16.0 * uv.x * (1.0-uv.x) * uv.y * (1.0-uv.y), 0.4);
//
//	int l = int(8.0 * rand(t+7.0));
//		
//	if ( 0 < l ) vI *= randomLine( t+6.0+17.* float(0));
//	if ( 1 < l ) vI *= randomLine( t+6.0+17.* float(1));
//	if ( 2 < l ) vI *= randomLine( t+6.0+17.* float(2));		
//	if ( 3 < l ) vI *= randomLine( t+6.0+17.* float(3));
//	if ( 4 < l ) vI *= randomLine( t+6.0+17.* float(4));
//	if ( 5 < l ) vI *= randomLine( t+6.0+17.* float(5));
//	if ( 6 < l ) vI *= randomLine( t+6.0+17.* float(6));
//	if ( 7 < l ) vI *= randomLine( t+6.0+17.* float(7));
//
//	int s = int( max(8.0 * rand(t+18.0) -2.0, 0.0 ));
//
//	if ( 0 < s ) vI *= randomBlotch( t+6.0+19.* float(0));
//	if ( 1 < s ) vI *= randomBlotch( t+6.0+19.* float(1));
//	if ( 2 < s ) vI *= randomBlotch( t+6.0+19.* float(2));
//	if ( 3 < s ) vI *= randomBlotch( t+6.0+19.* float(3));
//	if ( 4 < s ) vI *= randomBlotch( t+6.0+19.* float(4));
//	if ( 5 < s ) vI *= randomBlotch( t+6.0+19.* float(5));
//	
//	
//	// Show the image modulated by the defects
//    fragColor.xyz = image * vI;
//		
//	// Add some grain (thanks, Jose!)
//    fragColor.xyz *= (1.0+(rand(uv+t*.01)-.2)*.15);		

//---------------------------------------------------------------
    vec2 uv = fragTexCoord;
    vec4 texColor = texture(screenTex, fragTexCoord);
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    vec4 finalColor = texColor;

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
            // Calculate glow factor based on distance from the circle edge
            float glowFactor = smoothstep(glowRadius, radius, dist);
    
            // Blend between the normal color and the inverted color based on glow factor
            vec4 glowColor = mix(texColor, vec4(0.94, 0.90, 0.55, texColor.a), glowFactor * glowIntensity);
    
            // Apply the glow color
            finalColor = glowColor;
        } else {
            // Outside the circle, render normally
            finalColor = texColor;
        }
    } else {
        // If radius is 0, render the fragment as the original texture color
        finalColor = texColor;
    }
fragColor = finalColor;
//--------------------------------------------------------------------------------------

    
    // Apply the vintage effect to the texture color

    //fragColor = finalColor;
}