#pragma once

#include <shader.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

class Camera
{
    
    // ------------------------------------------
    // Camera Constants
    // ------------------------------------------

    // ------------------------------------------
    // Camera Settings    
    // ------------------------------------------
    
    // Projection settings
    GLfloat camZPos = -3.0;

    GLfloat fov = 45.0;
    GLfloat aspect_ratio = 4.0/3.0;

    GLfloat nearClip = 0.1;
    GLfloat farClip = 100.0;

    GLfloat baseCameraSpeed = 2.5f;
    GLfloat baseSensitivity = 0.1f;

    // ------------------------------------------
    // Camera States
    // ------------------------------------------

    GLfloat deltaTime = 0.0;
    GLfloat lastFrame = 0.0;

    // View State
    glm::mat4 view, projection;
    
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    GLfloat pitch;
    GLfloat yaw = -90.0;
    
    // Mouse State
    GLboolean firstMouse = true;
    GLfloat lastX = 0;
    GLfloat lastY = 0;


    public:
    Camera() = default;

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto* self = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if(self)
            self->processMouseCallback(xpos, ypos);
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto* self = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        if(self)
            self->processScrollCallback(xoffset, yoffset);
    }

    void processMouseCallback(double xpos, double ypos)
    {
        xpos = static_cast<GLfloat>(xpos);
        ypos = static_cast<GLfloat>(ypos);

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

        GLfloat sensitivity = baseSensitivity;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        // Restrict Pitch
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

    void processScrollCallback(double xoffset, double yoffset)
    {
        xoffset = static_cast<GLfloat>(xoffset);
        yoffset = static_cast<GLfloat>(yoffset);

        fov -= yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f; 
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