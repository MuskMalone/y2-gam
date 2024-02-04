#version 450 core
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

layout(std430, binding = 14) buffer Pctls {
    Particle Particles[];
};

uniform mat4 vertViewProjection;
out mat4 vertTransform;
out vec4 vertFragColor;

mat4 rotate(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    return mat4(
        c, s, 0.0, 0.0,
        -s, c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}
mat4 translate(vec4 pos) {
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(vec3(pos), 1.0)
    );
}
mat4 scale(vec2 scale) {
    return mat4(
        scale.x, 0.0, 0.0, 0.0,
        0.0, scale.y, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
//    return mat4(
//        100, 0.0, 0.0, 0.0,
//        0.0, 100, 0.0, 0.0,
//        0.0, 0.0, 1.0, 0.0,
//        0.0, 0.0, 0.0, 1.0
//    );
}

void main() {
    if (!Particles[gl_VertexID].alive) {
        vertTransform = mat4(0.0);
    } else {
        vertTransform = vertViewProjection * translate(Particles[gl_VertexID].pos) * rotate(Particles[gl_VertexID].rot) * scale(Particles[gl_VertexID].size);
        vertFragColor = Particles[gl_VertexID].col;
    }
}