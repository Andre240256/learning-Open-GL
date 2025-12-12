#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadradic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadradic;
};

#define NR_POINT_LIGHTS 4

//FUNCTIONS
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

void main()
{
    vec3 result = vec3(0.0f, 0.0f ,0.0f);

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    result += CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //AMBIENT LIGHT 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //DIFUSSE LIGHT
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //SPECULAR LIGHT
    vec3 reflectDir = normalize(reflect(- lightDir, normal));
    float spec = pow(max(dot(reflectDir, viewDir), 0.0),
                    material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //ATENUATION
    float dist = length(light.position - fragPos);
    float attenuation = 1.0f/(light.constant + light.linear * dist
                                + light.quadradic * dist * dist);

    //AMBIENT LIGHT
    vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, TexCoords));

    //DIFFUSE LIGHT
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = attenuation * light.diffuse * diff
                    * vec3(texture(material.diffuse, TexCoords));

    //SPECULAR LIGHT
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = attenuation * spec * light.specular * 
                    vec3(texture(material.specular, TexCoords));

    return specular + diffuse + ambient;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //CLACULATING CUTOFF
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,
                            0.0f, 1.0f);

    //ATTENUATION
    float dist = length(light.position - fragPos);
    float attenuation = 1.0f/(light.constant + light.linear * dist +
                                light.quadradic * dist * dist);
    
    //AMBIENT LIGHT
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    ambient *= intensity * attenuation;

    //DIFFUSE LIGHT
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    diffuse *= intensity * attenuation;

    //SPECULAR LIGHT
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    specular *= intensity * attenuation;

    return ambient + diffuse + specular;
}