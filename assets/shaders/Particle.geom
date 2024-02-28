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

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in mat4 vertTransform[];
in vec4 vertFragColor[];

out vec4 geomColor;

void EmitVertexCube(vec4 position, vec3 offset) {
    gl_Position = vertTransform[0] * (position + vec4(offset, 0.0));
    EmitVertex();
}

void main() {
    vec4 position = vec4(0,0,0,1);//gl_in[0].gl_Position;  // The position of the point
    geomColor = vertFragColor[0];  // Pass the color

    float size = 0.5;  // Half-size of the cube, since it's 1x1x1

// only front face needed
    EmitVertexCube(position, vec3(-size, -size, 0));
    EmitVertexCube(position, vec3(size, -size, 0));
    EmitVertexCube(position, vec3(-size, size, 0));
    EmitVertexCube(position, vec3(size, size, 0));
    EndPrimitive();

}