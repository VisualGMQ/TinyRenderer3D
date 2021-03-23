#version 330 core

#define DOT_LIGHT_MAX_NUM 15
#define SPOT_LIGHT_MAX_NUM 15

out vec4 outColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;
in mat3 TBN;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    // TODO currently we don't support normal texture
    sampler2D normal_texture;
    sampler2D diffuse_texture;
    sampler2D specular_texture;

    sampler2D emission_texture;

    int normal_texture_num;
    int diffuse_texture_num;
    int specular_texture_num;
    int emission_texture_num;
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

uniform sampler2D shadow_texture;

float CalcShadow(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir) {
    vec3 proj_coords = frag_pos_light_space.xyz/frag_pos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    if (proj_coords.z > 1.0) {
        return 0.0;
    }
    float closest_depth = texture(shadow_texture, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    float shadow = current_depth - bias > closest_depth  ? 1.0 : 0.0;
    return shadow;
}

vec3 CalcNormal() {
    if (material.normal_texture_num == 1) {
        vec3 n = texture(material.normal_texture, TexCoord).rgb;
        n = normalize(n* 2.0 - 1.0);   
        n = normalize(TBN*n);
        return n;
    }
    return Normal;
}

vec3 CalcBasicLightAmbient(Material material, vec3 ambient) {
    if (material.diffuse_texture_num == 1) {
        return ambient*texture(material.diffuse_texture, TexCoord).rgb;
    }
    return ambient*material.ambient;
}

vec3 CalcBasicLightDiffuse(Material material, vec3 diffuse, vec3 direction, vec3 normal) {
    vec3 lightDir = -normalize(direction);
    vec3 diffuse_color;
    if (material.diffuse_texture_num == 1) {
        diffuse_color = texture(material.diffuse_texture, TexCoord).rgb;
    } else {
        diffuse_color = material.diffuse;
    }
    return diffuse*max(dot(lightDir, normal), 0.0)*diffuse_color;
}

vec3 CalcBasicLightSpecular(Material material, vec3 specular, vec3 direction, vec3 normal) {
    if (material.shininess == 0) {
        return vec3(0.0f);
    }
    vec3 reflectDir = reflect(normalize(direction), normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 specular_color;
    if (material.specular_texture_num == 1) {
        specular_color = texture(material.specular_texture, TexCoord).rgb;
    } else {
        specular_color = material.specular;
    }
    return specular*specular_color*pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
}

vec3 CalcBasicLight(Material material, LightBase base, vec3 direction, vec3 normal) {
    vec3 ambient = CalcBasicLightAmbient(material, base.ambient);
    vec3 diffuse = CalcBasicLightDiffuse(material, base.diffuse, direction, normal);
    vec3 specular = CalcBasicLightSpecular(material, base.specular, direction, normal);
    return ambient+diffuse+specular;
}

vec3 CalcDirectionLight(Material material, vec3 normal) {
    vec3 ambient = CalcBasicLightAmbient(material, directionLight.base.ambient);
    vec3 diffuse = CalcBasicLightDiffuse(material, directionLight.base.diffuse, directionLight.direction, normal);
    vec3 specular = CalcBasicLightSpecular(material, directionLight.base.specular, directionLight.direction, normal);
    float shadow = CalcShadow(fragPosLightSpace, normal, directionLight.direction);
    vec3 lighting = (ambient+(1.0-shadow)*(diffuse+specular));
    return lighting;
}

vec3 CalcDotLight(Material material, DotLight light, vec3 normal) {
    float dist = length(light.position - fragPos);
    float attenuation = 1.0/(light.attenuation.constant+light.attenuation.linear*dist+light.attenuation.quadratic*dist*dist);

    vec3 color = CalcBasicLight(material, light.base, normalize(fragPos - light.position), normal);

    return attenuation*color;
}

vec3 CalcSpotLight(Material material, SpotLight light, vec3 normal) {
    float outer_cutoff = cos(radians(light.angle.outer_cutoff));
    float inner_cutoff = cos(radians(light.angle.inner_cutoff));
    float theta = dot(normalize(fragPos - light.position), normalize(light.direction));
    float intensity = clamp((theta - outer_cutoff)/(inner_cutoff - outer_cutoff), 0.0, 1.0 );

    vec3 color = CalcBasicLight(material, light.base, light.direction, normal);

    if (theta > outer_cutoff) {
        return intensity*color;
    }
    return vec3(0, 0, 0);
}

void main() {
    vec3 normal = CalcNormal();
    if (material.emission_texture_num == 1) {
        outColor = texture(material.emission_texture, TexCoord);
    } else {
        vec3 color = CalcDirectionLight(material, normal);
        for (int i = 0; i < lightnum.dotlight && i < DOT_LIGHT_MAX_NUM; i++) {
            color += CalcDotLight(material, dotLights[i], normal);
        }
        for (int i = 0; i < lightnum.spotlight && i < SPOT_LIGHT_MAX_NUM; i++) {
            color += CalcSpotLight(material, spotLights[i], normal);
        }
        outColor = vec4(color, 1.0);
    }
} 
