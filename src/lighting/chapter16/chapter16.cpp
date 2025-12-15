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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//functions declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window, Shader * shader);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);

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

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
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

    //create shaders
    Shader shaderProgram("src/shaders/vertex_shader.glsl",
                        "src/shaders/fragment_shader.glsl");
    Shader lightShader("src/shaders/vertex_shader.glsl",
                        "src/shaders/fragment_shader_light.glsl");   
           
    //create texture
    unsigned int diffuseMap = loadTexture(std::string("src/textures/container2.png").c_str());
    unsigned int specularMap = loadTexture(std::string("src/textures/container2_specular.png").c_str());
                  
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
    glEnableVertexAttribArray(1);

    //binding (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
    (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    shaderProgram.use();

    // shaderProgram.setVec3("material.specular",glm::vec3(1.0f));
    shaderProgram.setFloat("material.shininess", 32.0f);
    shaderProgram.setInt("material.diffuse", 0);
    shaderProgram.setInt("material.specular", 1);


    shaderProgram.setVec3("light.ambient", (float)0.1 * lightColor);
    shaderProgram.setVec3("light.diffuse", (float)0.5 * lightColor);
    shaderProgram.setVec3("light.specular", lightColor);
    shaderProgram.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shaderProgram.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    // shaderProgram.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shaderProgram.setFloat("light.constant", 1.0f);
    shaderProgram.setFloat("light.linear", 0.09f);
    shaderProgram.setFloat("light.quadratic", 0.032f);
    

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderProgram.use();
        shaderProgram.setVec3("light.position", camera.cameraPos);
        shaderProgram.setVec3("light.direction", camera.cameraFront);

        glm::mat4 projection = glm::mat4 (1.0f);
        projection = glm::perspective(glm::radians(camera.zoom), 800.0f/600.0f,
                                            0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMat();

        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);


        //Positions Coords Transforms
        for(int i = 0; i < 10; i++)
        {   

            glm::mat model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            shaderProgram.setMat4("model", model);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);

            shaderProgram.setVec3("viewPos", camera.cameraPos);
            
            //CUBE DRAWING
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        //Light source drawing
        lightShader.use();


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightSourcePosition);
        model = glm::scale(model, glm::vec3(0.3f));
        
        lightShader.setMat4("model", model);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        lightShader.setVec3("lightColor", lightColor);

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

unsigned int loadTexture(char const * path)
{
    //generate texture
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    //set the texture wraping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       

    //load texture
    int height, width, nrChannels;
    unsigned char * data = stbi_load(path, &width, &height,
                                    &nrChannels, 0);

    if(data){
        GLenum format;
        if(nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 
                    0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else{
        std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
