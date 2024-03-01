#version 450 core
#extension GL_ARB_bindless_texture : enable
#extension GL_EXT_shader_explicit_arithmetic_types_float64 : require

in vec4 geomColor;
in vec2 geomTexCoord;
flat in uvec2 geomTexHdl;

out vec4 FragColor;

void main() {
    if (geomTexHdl.x != 0 && geomTexHdl.y != 0){
        FragColor = texture(sampler2D(geomTexHdl), geomTexCoord) * geomColor;
        //FragColor = vec4(1,0,1,1);
    }else{
        FragColor = geomColor; // Example: orange color
    }
}
