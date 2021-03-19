#version 330 core

#define DOT_LIGHT_MAX_NUM 15
#define SPOT_LIGHT_MAX_NUM 15

out vec4 outColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct LightBase {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight {
    vec3 direction;
    LightBase base;
};

struct DotLight {
    vec3 position;
    LightBase base;

    struct Attenuation {
        float constant;
        float linear;
        float quadratic;
    }attenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    LightBase base;

    struct Angle {
        float inner_cutoff;
        float outer_cutoff;
    }angle;
};

struct LightNum {
    int dotlight;
    int spotlight;
};

uniform Material material;

uniform DirectionLight directionLight;
uniform DotLight dotLights[DOT_LIGHT_MAX_NUM];
uniform SpotLight spotLights[SPOT_LIGHT_MAX_NUM];
uniform LightNum lightnum;

uniform vec3 viewPos;

vec3 CalcBasicLightAmbient(Material material, vec3 ambient) {
    return ambient*texture(material.diffuse, TexCoord).rgb;
}

vec3 CalcBasicLightDiffuse(Material material, vec3 diffuse, vec3 direction) {
    vec3 lightDir = -normalize(direction);
    return diffuse*max(dot(lightDir, Normal), 0.0)*texture(material.diffuse, TexCoord).rgb;
}

vec3 CalcBasicLightSpecular(Material material, vec3 specular, vec3 direction) {
    vec3 reflectDir = reflect(normalize(direction), Normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    return specular*material.specular*pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
}

vec3 CalcBasicLight(Material material, LightBase base, vec3 direction) {
    vec3 ambient = CalcBasicLightAmbient(material, base.ambient);
    vec3 diffuse = CalcBasicLightDiffuse(material, base.diffuse, direction);
    vec3 specular = CalcBasicLightSpecular(material, base.specular, direction);
    return ambient+diffuse+specular;
}

vec3 CalcDirectionLight(Material material) {
    return CalcBasicLight(material, directionLight.base, directionLight.direction);
}

vec3 CalcDotLight(Material material, DotLight light) {
    float dist = length(light.position - fragPos);
    float attenuation = 1.0/(light.attenuation.constant+light.attenuation.linear*dist+light.attenuation.quadratic*dist*dist);

    vec3 color = CalcBasicLight(material, light.base, normalize(fragPos - light.position));

    return attenuation*color;
}

vec3 CalcSpotLight(Material material, SpotLight light) {
    float outer_cutoff = cos(radians(light.angle.outer_cutoff));
    float inner_cutoff = cos(radians(light.angle.inner_cutoff));
    float theta = dot(normalize(fragPos - light.position), normalize(light.direction));
    float intensity = clamp((theta - outer_cutoff)/(inner_cutoff - outer_cutoff), 0.0, 1.0 );

    vec3 color = CalcBasicLight(material, light.base, light.direction);

    if (theta > outer_cutoff) {
        return intensity*color;
    }
    return vec3(0, 0, 0);
}

void main() {
    vec3 color = CalcDirectionLight(material);
    for (int i = 0; i < lightnum.dotlight && i < DOT_LIGHT_MAX_NUM; i++) {
        color += CalcDotLight(material, dotLights[i]);
    }
    for (int i = 0; i < lightnum.spotlight && i < SPOT_LIGHT_MAX_NUM; i++) {
        color += CalcSpotLight(material, spotLights[i]);
    }
    outColor = vec4(color, 1.0);
} 
