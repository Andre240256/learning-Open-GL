#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 normal;
out mat4 viewMat;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    FragPos = vec3(view *model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(view * model))) * aNormal;
    viewMat = view;
    
    gl_Position = projection * vec4(FragPos, 1.0);
}