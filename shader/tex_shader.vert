#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out mat3 TBN;

uniform mat4 project;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightmatrix;

void main() {
    vec3 T = normalize(vec3(model * vec4(inTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(inBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal,    0.0)));
    TBN = mat3(T, B, N);

    fragPos = vec3(model*vec4(inPos, 1.0));
    fragPosLightSpace = lightmatrix*model*vec4(inPos, 1.0);
    Normal = normalize(mat3(transpose(inverse(model)))*inNormal);
    TexCoord = inTexCoord;
    gl_Position = project*view*model*vec4(inPos, 1.0);
}
