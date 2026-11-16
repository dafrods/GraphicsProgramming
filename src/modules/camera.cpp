#include "glm/matrix.hpp"
#include <camera.h>

#include <spdlog/spdlog.h>

static glm::mat4 lookAt(const glm::vec3&, const glm::vec3&, const glm::vec3&);

Camera::Camera()
{
    mode = FREECAM;
    projection = glm::perspective(glm::radians(fov), aspect_ratio, nearClip, farClip);
};

Camera::Camera(GLint mode) : mode(mode)
{
    projection = glm::perspective(glm::radians(fov), aspect_ratio, nearClip, farClip);
};

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* self = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if(self)
        self->processMouseCallback(xpos, ypos);
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* self = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if(self)
        self->processScrollCallback(xoffset, yoffset);
}

void Camera::processMouseCallback(double xpos, double ypos)
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


void Camera::processScrollCallback(double xoffset, double yoffset)
{
    xoffset = static_cast<GLfloat>(xoffset);
    yoffset = static_cast<GLfloat>(yoffset);

    fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}

void Camera::updateView()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateViewUserImpl()
{   
    view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processInput(GLFWwindow* window)
{
    GLfloat cameraSpeed = baseCameraSpeed * deltaTime + (baseCameraSpeed * speedFactor);
    glm::vec3 cameraNext = cameraFront;
    if(mode == FPSCAM)
        cameraNext = glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraNext;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraNext;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraNext, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraNext, cameraUp)) * cameraSpeed;
}

void Camera::updateDelta()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
}

void Camera::updateUniform(Shader& shader)
{
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
}

void Camera::setCameraSpeed(GLfloat speed)
{
    speedFactor = speed;
}

static glm::mat4 lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up = glm::vec3(0.0,1.0,0.0))
{
    // Define Camera Direction
    glm::vec3 direction = glm::normalize(center - eye); 

    // Define camera right vector
    glm ::vec3 right = glm::normalize(glm::cross(direction, up));
    
    // Define camera up
    glm::vec3 camUp = glm::cross(right, direction);

    
    glm::mat4 view = glm::mat4{
        glm::vec4(right, -glm::dot(right, eye)),
        glm::vec4(camUp, -glm::dot(camUp, eye)),
        glm::vec4(-direction,glm::dot(direction, eye)),
        glm::vec4(0.0,0.0,0.0,1.0)
    };

    return glm::transpose(view);
}


