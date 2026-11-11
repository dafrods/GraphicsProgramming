#pragma once

#include <shader.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
    public:
    const GLfloat baseCameraSpeed = 2.5f;

    GLfloat deltaTime = 0.0;
    GLfloat lastFrame = 0.0;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::mat4 view;

    // GLfloat roll;
    GLfloat pitch;
    GLfloat yaw = -90.0;

    GLfloat lastX = 0;
    GLfloat lastY = 0;
    GLboolean firstMouse = true;

    Camera() = default;
    
    // void updateViewAngle(GLfloat roll , GLfloat pitch, GLfloat yaw = -90.0)
    // {
    //     direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //     direction.y = sin(glm::radians(pitch));
    //     direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // }

    void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
    
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    void updateView()
    {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void processInput(GLFWwindow* window)
    {
        GLfloat cameraSpeed = baseCameraSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    void updateDelta()
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
    }

    void updateUniform(Shader& shader)
    {
        shader.setMat4("view", view);
    }

};