#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 lightmatrix;
uniform mat4 model;

void main() {
    gl_Position = lightmatrix*model*vec4(inPos, 1.0f);
}
