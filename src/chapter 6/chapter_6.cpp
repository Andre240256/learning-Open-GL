#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "include/shader.hpp"


//shaders sources
// const char * vertexShaderSource = 
//     "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "layout (location = 1) in vec3 aColor;\n"
//     "out vec3 ourColor;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos, 1.0);\n"
//     "   ourColor = aColor;\n"
//     "}\0";

//     const char* fragmentShaderSource =
//     "#version 330 core\n"
//     "in vec3 ourColor;\n"
//     "out vec4 fragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   fragColor = vec4(ourColor, 1.0);\n"
//     "}\0";

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

    //checking the number of vertex attributes supported
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
    // << std::endl;

    float vertices[] = {
        //positions         //colors
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top
    };

    //creating shaders program
    // GLint success;
    
    //vertex Shaders
    // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if(!success)
    // {
    //     char infoLog[512];
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::COMPILE::VERTEXSHADER::" << std::endl <<
    //     infoLog << std::endl;
    // }

    // //fragmentShader
    // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if(!success)
    // {
    //     char infoLog[512];
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::COMPILE::FRAGMENTSHADER::" << std::endl
    //     << infoLog << std::endl;
    // }

    // //final shader Program
    // GLuint shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if(!success)
    // {
    //     char infoLog[512];
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::LINKING::SHADERPROGRAM" << std::endl
    //     << infoLog << std::endl;
    // }

    // //destroy the shaders alredy linked
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    Shader shaderprogram("src/shaders/vertex_shader.glsl",
                            "src/shaders/fragment_shader.glsl");

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