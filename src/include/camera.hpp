#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class cam{
public:
    //constructor
    cam(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));

    //camera physics
    void camPos(GLFWwindow * window, float camSpeed);
    void camDir(GLFWwindow * window, double xpos, double ypos);
    void camZoom(double yoffset);

    //camera Transformations
    glm::mat4 getViewMat();

    float yaw;
    float pitch;
    float zoom;

    float lastX, lastY;

    bool firstMouseMovement;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

private:
    float sensitivity;
    float velocity;
};


//constructor
cam::cam(glm::vec3 cameraPos,
        glm::vec3 cameraFront,
        glm::vec3 cameraUp)
{
    this->yaw = 0.0f, this->pitch = 0.0f;

    this->zoom = 45.0f;

    this->cameraPos = cameraPos;
    this->cameraFront = cameraFront;
    this->cameraUp = cameraUp;

    this->sensitivity = 0.1f;
    this->velocity = 2.5f;
    this->firstMouseMovement = true;
}

//camera physics
void cam::camPos(GLFWwindow * window, float deltaTime)
{
    float camSpeed = deltaTime * velocity;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        this->cameraPos += camSpeed * this->cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        this->cameraPos -=  camSpeed * this->cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        this->cameraPos +=  camSpeed * glm::normalize(glm::cross(this->cameraFront, this->cameraUp));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        this->cameraPos -=  camSpeed * glm::normalize(glm::cross(this->cameraFront, this->cameraUp));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        this->cameraPos += camSpeed * this->cameraUp; 
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        this->cameraPos -= camSpeed * this->cameraUp;         
    }
}

void cam::camDir(GLFWwindow * window, double xpos, double ypos)
{
    if(firstMouseMovement){
        lastX = xpos, lastY = ypos;
        firstMouseMovement = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos, lastY = ypos;
    
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) *
                  cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->cameraFront = glm::normalize(direction);
}
   
void cam::camZoom(double yoffset){
    zoom -= (float)yoffset;
    if(zoom < 1.0f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}

//CAMERA TRANSFORMS
glm::mat4 cam::getViewMat()
{
    return glm::lookAt(this->cameraPos, 
                    this->cameraPos + this->cameraFront,
                    this->cameraUp);
}

#endif

