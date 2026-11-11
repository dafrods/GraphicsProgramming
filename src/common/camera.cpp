#include <camera.h>

#include <spdlog/spdlog.h>

Camera::Camera()
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

void Camera::processInput(GLFWwindow* window)
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
