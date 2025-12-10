#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.hpp"
#include "include/camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

//functions declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window, Shader * shader);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

//global variables
cam camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//STRUCTURES
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;

    Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh)
        : ambient (a), diffuse (d), specular (s), shininess(sh) {}
};


int main()
{
    //initialize and configure GLFW
    glfwInit();
    if(!glfwInit()){
        std::cerr <<"Failed to initialize GLFW" <<std::endl;
        return -1;
    }

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(!window){
        std::cerr <<"Failed to create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //initialize glew
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float verticesCubo[] = {
        //positions           //normals           //texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        0.5f, 0.5f, -0.5f,    0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  
        0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        
        -0.5f, 0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,    0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,    0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector <glm::vec3> cubesPos = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(4.0f, 2.0f, 0.0f),
        glm::vec3(-3.0f, -5.0f, -0.3f)
    };

    std::vector <Material> materials = {
        //gold
        Material(
            glm::vec3(0.24725, 0.1995f, 0.0745),
            glm::vec3(0.75164, 0.60648f, 0.22648),
            glm::vec3(0.628281, 0.555802f, 0.366065),
            0.4*128
        ),
        //obsidian
        Material(
            glm::vec3(0.05375, 0.05, 0.06625),
            glm::vec3(0.18275, 0.17, 0.22525),
            glm::vec3(0.332741, 0.328634, 0.346435),
            0.3 * 128
        ),
        //Red plastic
        Material(
            glm::vec3(0.0),
            glm::vec3(0.5, 0.0, 0.0),
            glm::vec3(0.7, 0.6, 0.6),
            0.25 * 125
        )
    };

    Shader shaderProgram("src/shaders/vertex_shader.glsl",
                        "src/shaders/fragment_shader.glsl");
    Shader lightShader("src/shaders/vertex_shader.glsl",
                        "src/shaders/fragment_shader_light.glsl");   
    //create buffers

    
    
    //create VBO and VBO
    GLuint VBO;
    GLuint VAO;
    
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    //Binding the VAO
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

    //binding (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    //binding (locaion = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*)(3*sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    //disbind VBO and VAO
    glBindVertexArray(0);


    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //binding (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    glm::vec3 lightSourcePosition(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor = glm::vec3(1.0f);

    shaderProgram.use();
    shaderProgram.setVec3("light.position", lightSourcePosition);
    shaderProgram.setVec3("light.ambient", lightColor);
    shaderProgram.setVec3("light.diffuse", lightColor);
    shaderProgram.setVec3("light.specular", lightColor);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window, &shaderProgram);

        glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderProgram.use();

        
        //Positions Coords Transforms
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4 (1.0f);

        view = camera.getViewMat();
                            
        projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f,
                                        0.1f, 100.0f);
        
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);
       
        shaderProgram.setVec3("viewPos", camera.cameraPos);
        
        for(int i = 0; i < cubesPos.size(); i++){
            model = glm::translate(model, cubesPos[i]);

            shaderProgram.setMat4("model", model);

            shaderProgram.setVec3("material.ambient", materials[i].ambient);
            shaderProgram.setVec3("material.diffuse", materials[i].diffuse);   
            shaderProgram.setVec3("material.specular", materials[i].specular);
            shaderProgram.setFloat("material.shininess", materials[i].shininess);  

            //CUBE DRAWING
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        //Light source drawing
        lightShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightSourcePosition);
        model = glm::scale(model, glm::vec3(0.3f));
        
        lightShader.setMat4("model", model);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        lightShader.setVec3("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//functions implementations
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Shader * shader)
{ 
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        float mixValue = shader->getFloat("mixValue");
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && mixValue < 1.0f)
            mixValue += 0.01f;
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && mixValue > 0.0f)
            mixValue -= 0.01f;
        shader->setFloat("mixValue", mixValue);
    }

    camera.camPos(window, deltaTime);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
   camera.camDir(window, xpos, ypos);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
   camera.camZoom(yoffset);
}
