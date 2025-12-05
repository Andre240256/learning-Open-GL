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

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        //positions         //texture coords
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f, //top right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,//bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f//top left
    };

    GLuint indices[] = {
        0, 1, 3, 
        1, 2, 3
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
    
    
    
    int counter = 0;
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderprogram.ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, 0.5, 0.0));

        float scale = glfwGetTime();
        scale = sin(scale);
        scale *= scale;
        trans = glm::scale(trans, glm::vec3(scale, scale, scale));
        shaderprogram.setMat4("transform", &trans);

        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        trans = glm::mat4(1.0f);
        shaderprogram.setMat4("transform", &trans);
        
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
}