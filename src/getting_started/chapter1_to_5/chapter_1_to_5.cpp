#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//global variables
const char * vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char * fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "   fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

//functions declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
    


int main()
{
    glfwInit();
    
    
    //just to make sure the version is the 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Request the compatibility profile so attribute setup without a VAO
    // works (on platforms/drivers that provide a compatibility context).
    // Note: macOS does not support the compatibility profile - this is Linux only.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); For MacOS
    
    //initialize window object
    GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == nullptr){
        std::cerr <<"Failed to create GLFW window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    //initialize glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //creating buffers
    float vertices[] = {
        //first triangle
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        // -0.5f, 0.5f, 0.0f,
        //second triangle
        // 0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    //creating EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //binding buffer
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //creating buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    //creating VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //bind the VAO
    glBindVertexArray(VAO);
    //passing the vertices to the gpu buffer VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //for checking the compilation and linking errors
    int success;
    char infoLog[512];

    //vertex shader compilation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<< "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << 
        infoLog <<std::endl;
    }

    //fragment shader compilation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<< "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
        infoLog <<std::endl;
    }

    //linking shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<< "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
        infoLog <<std::endl;
    }
    
    //deleting shaders as they are linked now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);





    //render loop
    while(!glfwWindowShouldClose(window)){
        //input
        processInput(window);

        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //drawing the objects

        //use the shader program we want to render the object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        //now draw the object
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        
        //check events and swap buffers
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

void processInput(GLFWwindow * window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose (window, true);
}