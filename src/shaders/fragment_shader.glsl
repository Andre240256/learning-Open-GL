#version 330 core

struct Light{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    // vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadradic;
};

struct Material{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

void main()
{
    //CALCULATING CUTOFF
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp ((theta - light.outerCutOff)/ epsilon, 0.0, 1.0);
    
    //ATENUATION
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
                                light.quadradic * dist * dist);

    //AMBIENT LIGHT
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    //DIFUSE LIGHT
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,
                                                        TexCoords));

    //SPECULAR LIGHT
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    diffuse *= intensity;
    specular *= intensity;

    vec3 result = attenuation * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}