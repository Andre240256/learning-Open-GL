#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

//functions declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window, Shader * shader);

//global variables
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;


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

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,  
        0.5f, -0.5f, 0.5f,    1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
        
        -0.5f, 0.5f, 0.5f,    1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,    1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,     1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,    0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,    1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,    1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,     1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,    0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 3, 
        1, 2, 3
    };
    
    glm::vec3 cubePositions [] = {
        glm::vec3( 0.0f, 0.0f,0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };


    Shader shaderprogram("src/shaders/vertex_shader.glsl",
        "src/shaders/fragment_shader.glsl");
        
    //create buffers
    
    // creating EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);

    
    
    //create VBO and VBO
    GLuint VBO;
    GLuint VAO;
    
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    //Binding the VAO
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    //binding (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),
    (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    //binding (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
                            (void* )(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //disbind VBO and VAO
    glBindVertexArray(0);

    //textures
    stbi_set_flip_vertically_on_load(true);

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //configurations of textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height , nrChannels;
    unsigned char * data1 = stbi_load("src/textures/container.jpg", &width, &height,
                                    &nrChannels, 0);
    if(data1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                    GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout <<"Failed to load texture" <<std::endl;
    }

    stbi_image_free(data1);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //configurations of textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char * data2 = stbi_load("src/textures/awesomeface.png", &width, &height,
                                    &nrChannels, 0);
    if(data2){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout <<"Failed to load texture" <<std::endl;
    }

    stbi_image_free(data2);

    shaderprogram.use();
    shaderprogram.setInt("texture1", 0);
    shaderprogram.setInt("texture2", 1);
    shaderprogram.setFloat("mixValue", 0.2f);
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f,
                                    0.1f, 100.0f);
    shaderprogram.setMat4("projection", &projection);

    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderprogram.ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shaderprogram.setMat4("view", &view);

        glBindVertexArray(VAO);
        for(int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f*i;
            
            if(i%3 == 0){
                angle = glfwGetTime();
                angle = sin(angle);
                angle *= 100*angle;
            }

            model = glm::rotate(model, glm::radians(angle), 
                                glm::vec3(1.0f, 0.3f, 0.5f));

            shaderprogram.setMat4("model", &model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        
        processInput(window, &shaderprogram);
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
    float cameraSpeed = 2.5f * deltaTime;
    
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

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -=  cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos +=  cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -=  cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}