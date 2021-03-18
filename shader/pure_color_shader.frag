#version 330 core

out vec4 outColor;
in vec3 Normal;
in vec3 fragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
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

uniform Material material;
uniform DirectionLight directionLight;
uniform DotLight dotLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 CalcBasicLightAmbient(Material material, vec3 ambient) {
    return ambient*material.ambient;
}

vec3 CalcBasicLightDiffuse(Material material, vec3 diffuse, vec3 direction) {
    vec3 lightDir = -normalize(direction);
    return diffuse*max(dot(lightDir, Normal), 0.0)*material.diffuse;
}

vec3 CalcBasicLightSpecular(Material material, vec3 specular, vec3 direction) {
    vec3 reflectDir = reflect(normalize(direction), Normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    return specular*material.specular*pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
}

vec3 CalcDirectionLight(Material material) {
    vec3 ambient = CalcBasicLightAmbient(material, directionLight.base.ambient);
    vec3 diffuse = CalcBasicLightDiffuse(material, directionLight.base.diffuse, directionLight.direction);
    vec3 specular = CalcBasicLightSpecular(material, directionLight.base.specular, directionLight.direction);

    return ambient+diffuse+specular;
}

vec3 CalcDotLight(Material material, DotLight light) {
    vec3 color;
    float dist = length(light.position - fragPos);
    float attenuation = 1.0/(light.attenuation.constant+light.attenuation.linear*dist+light.attenuation.quadratic*dist*dist);

    vec3 ambient = CalcBasicLightAmbient(material, light.base.ambient);
    vec3 diffuse = CalcBasicLightDiffuse(material, light.base.diffuse, fragPos-light.position);
    vec3 specular = CalcBasicLightSpecular(material, light.base.specular, fragPos-light.position);

    color = attenuation*ambient;
    color += attenuation*diffuse;
    color += attenuation*specular;

    return color;
}


vec3 CalcSpotLight(Material material, SpotLight light) {
    float outer_cutoff = cos(radians(light.angle.outer_cutoff));
    float inner_cutoff = cos(radians(light.angle.inner_cutoff));
    float theta = dot(normalize(fragPos - light.position), normalize(light.direction));
    float intensity = clamp((theta - outer_cutoff)/(inner_cutoff - outer_cutoff), 0.0, 1.0 );
    if (theta > outer_cutoff) {
        vec3 ambient = intensity*CalcBasicLightAmbient(material, light.base.ambient);
        vec3 diffuse = intensity*CalcBasicLightDiffuse(material, light.base.diffuse, light.direction);
        vec3 specular = intensity*CalcBasicLightSpecular(material, light.base.specular, light.direction);

        return ambient+diffuse+specular;
    }
    return vec3(0, 0, 0);
}

void main() {
    vec3 color = CalcDirectionLight(material);
    color += CalcDotLight(material, dotLight);
    color += CalcSpotLight(material, spotLight);
    outColor = vec4(color, 1.0);
}
