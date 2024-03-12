#version 450 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : require

in vec4 geomColor;
in vec2 geomTexCoord;
flat in uvec2 geomTexHdl;

out vec4 FragColor;

void main() {
    uint64_t combinedValue = (uint64_t(geomTexHdl.y) << 32) | uint64_t(geomTexHdl.x);
    if (combinedValue != 0){
        
        FragColor = texture(sampler2D(geomTexHdl), geomTexCoord) * geomColor;
        //FragColor = vec4(1,0,1,1);
    }else{
        FragColor = geomColor; // Example: orange color
    }
}
