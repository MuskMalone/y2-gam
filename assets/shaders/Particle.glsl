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
    vec4 col;         // 16 bytes (vec3 is aligned like vec4)
    vec2 gravity;
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
    vec2 gravity; // 8 bytes
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
uniform float uTimeElapsed;
//variables to store the new emitter
uniform vec4 uEmtvertices[4]; // Each vec4 is 16 bytes, total 64 bytes
uniform vec4 uEmtcol;         // 16 bytes (vec3 is aligned like vec4)
uniform vec2 uEmtgravity;
uniform float uEmtfrequency;  // 4 bytes
// type of emmission
//0: smoke
//1: fire
//2: burst
//3: burst with gravity
//4: gradual emission
uniform int uEmttype;         // 4 bytes
// 1 for point, 2 for line, 4 for rect
uniform int uEmtvCount;       // 4 byte
uniform int emtTargetIdx = -1;

uniform int spawnEmitter = 0;

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
void spawnEmitterParticle(uint emtidx){
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
    // edits emitter
    if (emtTargetIdx >= 0 && gid == 0){
        if (spawnEmitter == 1){ // spawn emitter
            Emitters[emtTargetIdx].vertices[0] = uEmtvertices[0]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[1] = uEmtvertices[1]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[2] = uEmtvertices[2]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[3] = uEmtvertices[3]; // Each vec4 is 16 bytes, total 64 bytes

            Emitters[emtTargetIdx].col = uEmtcol;         // 16 bytes (vec3 is aligned like vec4)
            Emitters[emtTargetIdx].gravity = uEmtgravity;
            Emitters[emtTargetIdx].time = 0.0;       // 4 bytes, but due to the vec3 above, you can expect padding here
            Emitters[emtTargetIdx].frequency = uEmtfrequency;  // 4 bytes
            Emitters[emtTargetIdx].type = uEmttype;         // 4 bytes
            Emitters[emtTargetIdx].vCount = uEmtvCount;       // 4 bytes
            Emitters[emtTargetIdx].alive = true;       // 4 bytes (bools are often treated as 4 bytes for alignment)

            
        }
        else if (spawnEmitter == -1){ //delete emitter
            Emitters[emtTargetIdx].alive = false;       // 4 bytes (bools are often treated as 4 bytes for alignment)
        }
        else{ //edit emitter
            Emitters[emtTargetIdx].vertices[0] = uEmtvertices[0]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[1] = uEmtvertices[1]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[2] = uEmtvertices[2]; // Each vec4 is 16 bytes, total 64 bytes
            Emitters[emtTargetIdx].vertices[3] = uEmtvertices[3]; // Each vec4 is 16 bytes, total 64 bytes

            Emitters[emtTargetIdx].col = uEmtcol;         // 16 bytes (vec3 is aligned like vec4)
            Emitters[emtTargetIdx].gravity = uEmtgravity;
            Emitters[emtTargetIdx].frequency = uEmtfrequency;  // 4 bytes
            Emitters[emtTargetIdx].type = uEmttype;         // 4 bytes
            Emitters[emtTargetIdx].vCount = uEmtvCount;       // 4 bytes

        }
        return;
    }
    //if there is no process happening atm;
    if (emtTargetIdx < 0){
        //for each emitter
        Emitters[gid].time += DT;
        if (Emitters[gid].time >= Emitters[gid].frequency){
            Emitters[gid].time = 0.0;
            spawnEmitterParticle(gid);
        }
        
        if (Particles[gid].alive){
            Particles[gid].age += DT;
            Particles[gid].vel += Particles[gid].gravity;
            Particles[gid].pos += vec3(Particles[gid].vel, 0) * DT;
            
            //if particles are dead
            if (Particles[gid].age >= Particles[gid].lifetime){
                Particles[gid].alive = false;
            }

        }
    }

    


}