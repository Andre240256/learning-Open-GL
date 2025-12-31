#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 tLightPos;
out vec3 tViewPos;
out vec3 tFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(FragPos, 1.0f);
    TexCoords = aTexCoords;


    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0f)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0f)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0f)));
    mat3 TBN = transpose(mat3(T, B, N));

    tLightPos = TBN * lightPos;
    tViewPos = TBN * viewPos;
    tFragPos = TBN * FragPos;
}