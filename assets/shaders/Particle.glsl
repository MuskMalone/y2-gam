#version 450 core
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

#define VCOUNT_POINT 1 //point
#define VCOUNT_LINE 2 //line
#define VCOUNT_QUAD 4 //quad
#define VCOUNT_ELLIPSE 5//circle

//type of emmission for all emitter types or points
#define EMT_TYPE_GRADUAL 0

//type of emissions for lines
#define EMT_TYPE_RAIN 1 //particles are fired with random angle wrt normal of line
#define EMT_TYPE_LAZER 2 //fires particles in a line in normal of line

//type of emissions for quads
#define EMT_TYPE_DUST 4
#define EMT_TYPE_DISINTEGRATE 5 // the tetricity block disintegrate

//presets for emitters
#define ALPHA_OVER_LIFETIME 0 //decreasing alpha over lifetime
#define SIZE_OVER_LIFETIME 1 //decreasing size over lifetime
#define ALPHA_SIZE_DECR_OVER_LIFETIME 2  //decreasing alpha and size over lifetime
#define ALPHA_SIZE_INCR_OVER_LIFETIME 3  //increasing alpha and size over lifetime

struct Particle {
    vec4 col;     // 16 bytes
    vec4 pos;     // 12 bytes

    vec2 vel;     // 8 bytes
    vec2 gravity; // 8 bytes
    vec2 size;    // 8 bytes (vec2 is aligned to 8 bytes)
    float rot;    // 4 bytes
    float age;    // 4 bytes
    float lifetime; // 4 bytes
    float angvel;  // 4 bytes
    int emtIdx;
    bool alive;   // 4 bytes (bool is often aligned to 4 bytes, same as float)
    // Possibly padding here to align the structure size to a multiple of 16 bytes
};

struct Emitter {
    //vertices are stored in clockwise order;
    vec4 vertices[4]; // Each vec4 is 16 bytes, total 64 bytes
    vec4 col;         // 16 bytes (vec3 is aligned like vec4)

    vec2 gravity; // 8 bytes
    vec2 size;    // 8 bytes (vec2 is aligned to 8 bytes)
    float rot;    // 4 bytes
    float lifetime; // 4 bytes
    float angvel;  // 4 bytes
    float speed;     // 8 bytes

    float time;       // 4 bytes, but due to the vec3 above, you can expect padding here
    float frequency;  // 4 bytes

    int type;         // 4 bytes
    // 1 for point, 2 for line, 4 for rect
    int vCount;       // 4 bytes
    int preset;    // 4 bytes //alpha over lifetime etc
    int particlesPerFrame; // 4 bytes

    bool alive;       // 4 bytes (bools are often treated as 4 bytes for alignment)
};


//they are the same size owo
layout( std430, binding=13 ) buffer Emts
{ Emitter Emitters[]; };
layout( std430, binding=10 ) buffer PctlsStrt
{ Particle ParticlesStart[]; };
layout( std430, binding=14 ) buffer Pctls
{ Particle Particles[]; };

// local work group is 100 large. I believe ideal local size would be GCD(num_cores, num_particles)
// More testing needed
layout( local_size_x = 1000, local_size_y = 1, local_size_z = 1 ) in;

// uniform control variables
uniform float DT;

uniform uint bufferMaxCount;

//lerp
vec2 linearLerp(vec2 a, vec2 b, float t){
	return a + (b - a) * t;
}
vec3 linearLerp(vec3 a, vec3 b, float t){
	return a + (b - a) * t;
}
vec4 linearLerp(vec4 a, vec4 b, float t){
	return a + (b - a) * t;
}
float linearLerp(float a, float b, float t){
	return a + (b - a) * t;
}

void main() {
    // gid used as index into SSBO to find the particle
    // that any particular instance is controlling
    uint gid = gl_GlobalInvocationID.x;
        
    if (Particles[gid].alive == true){

        Particles[gid].age += DT;
        Particles[gid].vel += Particles[gid].gravity;
        Particles[gid].pos += vec4(Particles[gid].vel, 0, 1) * DT;
        Particles[gid].rot += Particles[gid].angvel * DT;
        
        switch(Emitters[Particles[gid].emtIdx].preset){
        case ALPHA_OVER_LIFETIME:{
            Particles[gid].col.a = linearLerp(ParticlesStart[gid].col.a, 0, Particles[gid].age / Particles[gid].lifetime);
            }
			break;
        case SIZE_OVER_LIFETIME:{
            Particles[gid].size = linearLerp(ParticlesStart[gid].size, vec2(0, 0), Particles[gid].age / Particles[gid].lifetime);
            }
            break;
        case ALPHA_SIZE_DECR_OVER_LIFETIME:{
            Particles[gid].col.a = linearLerp(ParticlesStart[gid].col.a, 0, Particles[gid].age / Particles[gid].lifetime);

            Particles[gid].size = linearLerp(ParticlesStart[gid].size, vec2(0, 0), Particles[gid].age / Particles[gid].lifetime);
            }
            break;
        case ALPHA_SIZE_INCR_OVER_LIFETIME:{
                Particles[gid].col.a = linearLerp(ParticlesStart[gid].col.a, 0, Particles[gid].age / Particles[gid].lifetime);
                Particles[gid].size = linearLerp(vec2(0), ParticlesStart[gid].size, Particles[gid].age / Particles[gid].lifetime);
            }
            break;
        }
        //if particles are dead
        if (Particles[gid].age >= Particles[gid].lifetime){
            Particles[gid].alive = false;
        }

    }
}