#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.hpp"
#include "include/camera.hpp"
#include "include/Mesh.hpp"
#include "include/model.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //initialize glew
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);



    //create shaders
    Shader shader("src/shaders/chapter21.vs", "src/shaders/chapter21.fs");
   
    std::cout << "Iniciando o carregamento do modelo..." << std::endl;
    Model backpack("src/textures/backpack/backpack.obj");
    std::cout << "Modelo carregado com sucesso!" << std::endl;

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, &shader);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        glm::mat4 projection = glm::mat4 (1.0f);
        projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f,
                                            0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMat();

        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        backpack.Draw(shader);
   
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
