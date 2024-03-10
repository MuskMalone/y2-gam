/*******************************************************************************/
/*!
\par        Image Engine
\file       Perticle.geom

\author     tan cheng hian t.chengian
\date       Oct 13, 2023

\brief      

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/*******************************************************************************/
#version 450 core
#extension GL_ARB_bindless_texture : enable

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

struct TextureData{
    vec2 texCoords[4];
    uvec2 texHdl;
};

layout(std430, binding = 9) buffer TextureHandles 
{ TextureData TexHdls[]; }; 

in mat4 vertTransform[];
in vec4 vertFragColor[];
in uint texIdx[];

out vec4 geomColor;
out vec2 geomTexCoord;
flat out uvec2 geomTexHdl;

void EmitVertexCube(vec4 position, vec3 offset, vec2 texCoord, uvec2 texHdl) {
    gl_Position = vertTransform[0] * (position + vec4(offset, 0.0));
    geomTexCoord = texCoord;
    geomTexHdl = texHdl;
    EmitVertex();
}

void main() {
    vec4 position = vec4(0,0,0,1);//gl_in[0].gl_Position;  // The position of the point
    geomColor = vertFragColor[0];  // Pass the color

    float size = 0.5;  // Half-size of the cube, since it's 1x1x1

// only front face needed
    EmitVertexCube(position, vec3(-size, -size, 0), TexHdls[texIdx[0]].texCoords[0], TexHdls[texIdx[0]].texHdl); //bl
    EmitVertexCube(position, vec3(size, -size, 0), TexHdls[texIdx[0]].texCoords[1], TexHdls[texIdx[0]].texHdl); //br
    EmitVertexCube(position, vec3(-size, size, 0), TexHdls[texIdx[0]].texCoords[2], TexHdls[texIdx[0]].texHdl); //tl
    EmitVertexCube(position, vec3(size, size, 0), TexHdls[texIdx[0]].texCoords[3], TexHdls[texIdx[0]].texHdl); //tr
    EndPrimitive();

}