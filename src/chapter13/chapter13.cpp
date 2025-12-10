#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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
    
    shaderProgram.use();
    shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shaderProgram.setVec3("lightPos", lightSourcePosition);

    lightShader.use();
    lightShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Cube drawing
        shaderProgram.use();

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4 (1.0f);
        
        view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront
                            ,camera.cameraUp);
                            
        projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f,
                                        0.1f, 100.0f);
        
        model = glm::rotate(model, glm::radians(0.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f));
        
        shaderProgram.setMat4("model", model);
        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);

        shaderProgram.setVec3("viewPos", camera.cameraPos);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //Light source drawing
        lightShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightSourcePosition);
        model = glm::scale(model, glm::vec3(0.3f));
        
        lightShader.setMat4("model", model);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);


        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
       
        processInput(window, &shaderProgram);
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
