#version 330 core
layout (location = 0) in vec3 inPos;

out vec3 texcoords;

uniform mat4 project;
uniform mat4 view;

void main() {
    texcoords = inPos;
    gl_Position = project*view*vec4(inPos, 1.0);
}
