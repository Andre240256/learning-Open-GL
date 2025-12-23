#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    //constructor reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath,const std::string& geometryPath, const std::string& fragmentPath);

    void use();

    //setters
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int  value) const;
    void setFloat(const std::string &name, float  value) const;
    void setMat4(const std::string &name, const glm::mat4 & mat) const;
    void setVec3(const std::string &name, const glm::vec3 & vec) const;
    void setVec2(const std::string &name, const glm::vec2 & vec) const;

    //getters
    float getFloat(const std::string & name) const;
private:
};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode;
    std::ifstream vShaderFile;
    std::string fragmentCode;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit |
                        std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit |
                        std::ifstream::badbit);
    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
        << std::endl;
    }

    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED"
        << std::endl << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED"
        << std::endl << infoLog << std::endl;
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED"
        << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const std::string& vertexPath,const std::string& geometryPath, const std::string& fragmentPath)
{
    std::string vertexCode;
    std::ifstream vShaderFile;

    std::string geometryCode;
    std::ifstream gShaderFile;

    std::string fragmentCode;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | 
                        std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | 
                            std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | 
                            std::ifstream::badbit);

    try{
        vShaderFile.open(vertexPath);
        gShaderFile.open(geometryPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, gShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
        << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, geometry, fragment;
    GLint success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED"
        << std::endl << infoLog << std::endl;
    }

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED"
        << std::endl << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED"
        << std::endl << infoLog << std::endl;
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, geometry);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED"
        << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->ID);
}

//setters

void Shader::setBool(const std::string &name, bool  value) const{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int  value) const{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float  value) const{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, const glm::mat4 & mat) const{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, 
                        glm::value_ptr(mat));
}
void Shader::setVec3(const std::string &name, const glm::vec3 & vec) const{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(vec));
}
void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const {
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(vec));
}

//getters

float Shader::getFloat(const std::string & name) const{
    float value;
    glGetUniformfv(this->ID, glGetUniformLocation(this->ID, name.c_str()), &value);
    return value;
}
#endif