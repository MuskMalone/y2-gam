#version 450 core
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

#define EMT_TYPE_SMOKE 0
#define EMT_TYPE_FIRE 1
#define EMT_TYPE_BURST 2
#define EMT_TYPE_BURSTGRAV 3
#define EMT_TYPE_GRADUAL 4

#define EMT_SHAPE_POINT 1
#define EMT_SHAPE_LINE 2
#define EMT_SHAPE_RECT 3

struct Emitter {
    vec4 vertices[4]; // Each vec4 is 16 bytes, total 64 bytes
    vec3 pos;         // 16 bytes (vec3 is aligned like vec4)
    float time;       // 4 bytes, but due to the vec3 above, you can expect padding here
    float frequency;  // 4 bytes
    // type of emmission
    //0: smoke
    //1: fire
    //2: burst
    //3: burst with gravity
    //4: gradual emission
    int type;         // 4 bytes
    // 1 for point, 2 for line, 4 for rect
    int vCount;       // 4 bytes
    bool alive;       // 4 bytes (bools are often treated as 4 bytes for alignment)
    // Padding might be added here to align the entire structure size
};

struct Particle {
    vec4 col;     // 16 bytes
    vec3 pos;     // 12 bytes

    vec2 vel;     // 8 bytes
    vec2 size;    // 8 bytes (vec2 is aligned to 8 bytes)
    float rot;    // 4 bytes
    float age;    // 4 bytes
    float lifetime; // 4 bytes
    float angvel;  // 4 bytes

    bool alive;   // 4 bytes (bool is often aligned to 4 bytes, same as float)
    // Possibly padding here to align the structure size to a multiple of 16 bytes
};


//they are the same size owo
layout( std430, binding=13 ) buffer Emts
{ Emitter Emitters[]; };
layout( std430, binding=14 ) buffer Pctls
{ Particle Particles[]; };
layout(std430, binding = 15) buffer ParticleIndex {
    uint usableParticleCount;
};

// local work group is 100 large. I believe ideal local size would be GCD(num_cores, num_particles)
// More testing needed
layout( local_size_x = 1000, local_size_y = 1, local_size_z = 1 ) in;

// uniform control variables

uniform float DT;
//variables to store the new emitter
uniform int emtType; // type of emmission
uniform vec4 emtVertices[4];
uniform int emtVCount; // 1 for point, 2 for line, 4 for rect
uniform float emtFrequency;
uniform int emtTargetIdx = -1;

uniform int emitterMaxCount;
uniform int spawnEmitter = 0;
uniform uint emitterIdx = 0;

uniform uint bufferMaxCount;
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
void spawnParticle(Particle pctl){
    uint idx = atomicAdd(usableParticleCount, uint(1));
    atomicCompSwap(usableParticleCount, bufferMaxCount, uint(0));
    Particles[idx] = pctl;
}
void spawnParticlePoint(){
    
}
void spawnParticleRange(){
    
}
void spawnEmitterParticle(int emtidx){
    if (Emitters[emtidx].vCount == 1){ // is point
    
    }
    else if (Emitters[emtidx].vCount == 2){// is line
    
    }else if (Emitters[emtidx].vCount == 4){// is quad
    
    }
}

void main() {
    // gid used as index into SSBO to find the particle
    // that any particular instance is controlling
    uint gid = gl_GlobalInvocationID.x;
    // spawns emitter
    if (spawnEmitter == 1 && gid == 0){ // first thread should perform the update

        // throws an error, resolve this on application side
        //emitterIdx++; 
        //emitterIdx %= emitterMaxCount;
        //spawnEmitter = 0; 
    }
    // edits emitter
    if (emtTargetIdx >= 0 && gid == 0){
        //emtTargetIdx = -1; //handle in application side
    }

    //for each emitter
    Emitters[gid].time += DT;
    if (Emitters[gid].time >= Emitters[gid].frequency){
        Emitters[gid].time = 0.0;

    }
    if (gid == 0){
        spawnParticle(Particle(
            vec4(1.0,1.0,1.0,1.0),
            vec3(float(0), float(0), 1.0),
            vec2(0.0,0.0),
            vec2(10,10),
            0,0,0,0,
            true
        ));
    }
    


}