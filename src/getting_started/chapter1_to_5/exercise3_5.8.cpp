#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//definitions

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

//functions declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//shaders
const char * vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


const char * fragmentShaderSource1 = 
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "   fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

const char * fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "   fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";


int main()
{
    glfwInit();

    GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Exercise 1", NULL, NULL);
    if(!window)
    {
        std::cerr<<"COULD NOT INITIALIZE A WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    // set viewport with correct width and height
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    int success;
    char infoLog[512];
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR IN VERTEX SHADER COMPILATION\n"<<infoLog<<std::endl;
    }
    
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout<<"ERROR IN FRAGMENT SHADER COMPILATION\n"<<infoLog<<std::endl;
    }

    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR IN FRAGMENT_2 SHADER COMPILATION" <<
        infoLog << std::endl;
    }

    
    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cerr << "ERROR IN THE PROGRAM LINKAGE\n" <<
        infoLog << std::endl;
    }
    
    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cerr << "ERROR IN THE LINKING SHADER2" << infoLog <<std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);


    float vertices1[] = {
        //first triangle
        0.5f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
    float vertices2[] = {
        //second triangle
        0.5f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f,
        -0.5f, -1.0f, 0.0f

    };

    unsigned int VBO1,VBO2;
    glGenBuffers(1, &VBO1);
    glGenBuffers(1,&VBO2);
    unsigned int VAO1, VAO2;
    // VAO must be generated with glGenVertexArrays
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);

    glBindVertexArray(VAO1);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
    (void *)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
                        (void *)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//functions implementations
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}