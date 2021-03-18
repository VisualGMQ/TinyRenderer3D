#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 fragPos;

uniform mat4 project;
uniform mat4 model;
uniform mat4 view;

void main() {
    fragPos = vec3(model*vec4(inPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model)))*inNormal);
    TexCoord = inTexCoord;
    gl_Position = project*view*model*vec4(inPos, 1.0);
}
