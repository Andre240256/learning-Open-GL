#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "include/shader.hpp"

//functions declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);


int main()
{
    //initialize and configure GLFW
    glfwInit();
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
        //positions         //colors
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top
    };


    Shader shaderprogram("src/shaders/vertex_shader.glsl","src/shaders/fragment_shader.glsl");

    //create buffers

    //create VBO and VBO
    GLuint VBO;
    GLuint VAO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);

    //Binding the VAO
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                            (void*)0 );
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                            (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    //disBinding the VAO
    glBindVertexArray(0);

    float offset;
    std::cout << "ENTER OFFSET VALUE: ";
    std::cin >> offset;
    std::cout << std::endl;
    shaderprogram.use();
    shaderprogram.setFloat("offset", offset);

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderprogram.ID);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        processInput(window);
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

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}