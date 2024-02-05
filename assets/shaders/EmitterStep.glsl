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
#define ALPHA_OVER_LIFETIME 0
#define SIZE_OVER_LIFETIME 1
#define ALPHA_SIZE_OVER_LIFETIME 2

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
    
    // Padding might be added here to align the entire structure size
};


//they are the same size owo
layout(std430, binding=11) buffer RandIdx //the index for Rands, increments every time random() is called
{ uint randIdx; };
layout(std430, binding=12) buffer Rands //list of random floats generated by the cpu
{ float RandomFloats[]; };  
layout( std430, binding=13 ) buffer Emts
{ Emitter Emitters[]; };
layout( std430, binding=10 ) buffer PctlsStrt
{ Particle ParticlesStart[]; };
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

uniform uint bufferMaxCount = 5000000;
//from -1 to 1
float random(){ //retun
    atomicAdd(randIdx, uint(1));
	return RandomFloats[randIdx % bufferMaxCount];
    //return 1.0;
}

//from 0 to 1
float rand(){
    return fract(sin(dot(vec2(random()), vec2(12.9898, 78.233))) * 43758.5453);
}
float randRange(vec2 range){
    return range.x + ((range.y - range.x) * rand());
}
vec2 randDir(float startAngle, float endAngle, float magnitude){
    float angle = randRange(vec2(startAngle, endAngle));
    float s = sin(radians(angle));
    float c = cos(radians(angle));
    mat2 rotmat = mat2(
        c,-s,s,c
    );

    return (vec2(1,0) * rotmat) * magnitude;
}
vec2 randDirInRange(vec2 baseDir, float angleDegree, float magnitude) {
    
    float angleRange = radians(angleDegree);
    // Normalize the base direction
    vec2 normBaseDir = normalize(baseDir);

    // Generate a random angle offset within the specified range
    float angleOffset = randRange(vec2(-angleRange / 2.0, angleRange / 2.0));

    // Create a rotation matrix
    float s = sin(angleOffset);
    float c = cos(angleOffset);
    mat2 rotMat = mat2(
        c, -s,
        s, c
    );

    // Apply the rotation and scale by magnitude
    return (normBaseDir * rotMat) * magnitude;
}
void spawnParticle(Particle pctl){
    uint idx = atomicAdd(usableParticleCount, uint(1));
    Particles[idx % bufferMaxCount] = pctl;
    ParticlesStart[idx % bufferMaxCount] = pctl;
}
void spawnParticlePoint(uint emtidx){
    spawnParticle(Particle(
        Emitters[emtidx].col, //
        vec4(Emitters[emtidx].vertices[0]),
        randDir(0, 360, Emitters[emtidx].speed), // velocity
        Emitters[emtidx].gravity,
        Emitters[emtidx].size,
        Emitters[emtidx].rot, 
        0, 
        Emitters[emtidx].lifetime, 
        Emitters[emtidx].angvel, 
        int(emtidx),
        true
    ));
}

//emtidz is the indx of the Emitter
//vtx1 and vtx2 are the indices of the vertices
//angleRange is the range of angles in degrees wrt to normal
void spawnParticleRange(uint emtidx, int vtx1, int vtx2, float angleRange){
    //rotate by 90 degrees to find normal;
    vec2 vec = vec2(Emitters[emtidx].vertices[vtx2] - Emitters[emtidx].vertices[vtx1]);
    vec2 nml = normalize(vec2(-vec.y, vec.x));
    vec2 point = vec2(Emitters[emtidx].vertices[vtx1]) + (rand() * vec);
    
    spawnParticle(Particle(
        Emitters[emtidx].col, //
        vec4(point, 0, 1),
        randDirInRange(nml, angleRange, Emitters[emtidx].speed), // velocity
        Emitters[emtidx].gravity,
        Emitters[emtidx].size,
        Emitters[emtidx].rot, 
        0, 
        Emitters[emtidx].lifetime, 
        Emitters[emtidx].angvel, 
        int(emtidx),
        true
    ));
}

void spawnEmitterParticle(uint emtidx){
    int type = Emitters[emtidx].type;
    int vCount = Emitters[emtidx].vCount;
    int particlesPerFrame = Emitters[emtidx].particlesPerFrame;
    if (vCount == 1){ // is point
        if (type == EMT_TYPE_GRADUAL){
            //inside the if block so that lesser comparisions per loop
            for (int i = 0; i < particlesPerFrame; ++i){
                spawnParticlePoint(emtidx);
            }
        }
    }
    else if (vCount == 2){// is line
        if (type == EMT_TYPE_GRADUAL){
            for (int i = 0; i < particlesPerFrame; ++i){
                spawnParticleRange(emtidx, 0, 1, 360);
            }
        }
        else if (type == EMT_TYPE_RAIN){
            for (int i = 0; i < particlesPerFrame; ++i){
                spawnParticleRange(emtidx, 0, 1, 30);}
        }
		else if (type == EMT_TYPE_LAZER){
            for (int i = 0; i < particlesPerFrame; ++i){
                spawnParticleRange(emtidx, 0, 1, 0);}
        }
    }
    else if (vCount == 4){// is quad
        if (type == EMT_TYPE_GRADUAL){
            for (int i = 0; i < particlesPerFrame; ++i){
                int randCode = int(randRange(vec2(1, 4.99)));
                int start, end, angle;
                if (randCode == 1) {start = 0; end = 1;}
                else if (randCode == 2) {start = 1; end = 2;}
                else if (randCode == 3) {start = 2; end = 3;}
                else if (randCode == 4) {start = 3; end = 0;}
                spawnParticleRange(emtidx, start, end, 180);}
        }
        else if (type == EMT_TYPE_DUST){
            vec4 maxVert = max(Emitters[emtidx].vertices[0], Emitters[emtidx].vertices[2]); // max values from top left and bottom right
            vec4 minVert = min(Emitters[emtidx].vertices[0], Emitters[emtidx].vertices[2]); // min values from top left and bottom right

            for (int i = 0; i < particlesPerFrame; ++i){
                
                vec2 point = vec2(randRange(vec2(minVert.x, maxVert.x)), randRange(vec2(minVert.y, maxVert.y)));
                spawnParticle(Particle(
				    Emitters[emtidx].col, //
				    vec4(point, 0, 1),
				    randDir(0, 360, Emitters[emtidx].speed), // velocity
				    Emitters[emtidx].gravity,
				    Emitters[emtidx].size,
				    Emitters[emtidx].rot, 
				    0, 
				    Emitters[emtidx].lifetime, 
				    Emitters[emtidx].angvel, 
				    int(emtidx),
				    true
			    ));}
        }
        else if (type == EMT_TYPE_DISINTEGRATE){
            
            vec2 maxVert = vec2(max(Emitters[emtidx].vertices[0], Emitters[emtidx].vertices[2])); // max values from top left and bottom right
            vec2 minVert = vec2(min(Emitters[emtidx].vertices[0], Emitters[emtidx].vertices[2])); // min values from top left and bottom right
            vec2 center = (maxVert + minVert) / 2;
            vec2 dims = maxVert - minVert;
            vec2 invdims = dims / float(particlesPerFrame);

            for (int i = 0; i < particlesPerFrame; ++i){
                for (int j = 0; j < particlesPerFrame; ++j){
                    vec2 point = vec2(minVert) + (vec2(invdims) * vec2(float(i), float(j)) + vec2(invdims * 0.5));
                    spawnParticle(Particle(
				        Emitters[emtidx].col, //
				        vec4(point, 0, 1),
				        randDir(0, 360, Emitters[emtidx].speed),
				        Emitters[emtidx].gravity,
				        invdims,
				        Emitters[emtidx].rot, 
				        0, 
				        Emitters[emtidx].lifetime, 
				        Emitters[emtidx].angvel, 
				        int(emtidx),
				        true
			        ));
                }
            }
        }


    }
}

void main() {
    // gid used as index into SSBO to find the particle
    // that any particular instance is controlling
    uint gid = gl_GlobalInvocationID.x;
    if (Emitters[gid].alive == true){
        //for each emitter
        Emitters[gid].time += DT;
        if (Emitters[gid].time >= Emitters[gid].frequency){
            Emitters[gid].time = 0.0;
            spawnEmitterParticle(gid);
        }
    }
}