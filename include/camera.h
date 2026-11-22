#pragma once

#include <shader.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// CONSIDER ENUMS
// CAMERA MOVEMENT MODE
#define FREECAM 1
#define FPSCAM 2

class Camera
{
    
    // ------------------------------------------
    // Camera Constants
    // ------------------------------------------

    // * EMPTY * 

    // ------------------------------------------
    // Camera Settings    
    // ------------------------------------------
    
    // Camera Mode
    GLint mode; // OPTIONS: [1] - FREECAM; [2] - FPSCAM

    // Projection settings
    GLfloat camZPos = -3.0;

    GLfloat fov = 45.0;
    GLfloat aspect_ratio = 4.0/3.0;

    GLfloat nearClip = 0.1;
    GLfloat farClip = 100.0;

    GLfloat baseCameraSpeed = 2.5f;
    GLfloat baseSensitivity = 0.1f;

    GLfloat speedFactor = 0.0;

    // ------------------------------------------
    // Camera States
    // ------------------------------------------

    GLfloat deltaTime = 0.0;
    GLfloat lastFrame = 0.0;

    // View State
    glm::mat4 view, projection;
    
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(3.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    GLfloat pitch;
    GLfloat yaw = -90.0;
    
    // Mouse State
    GLboolean firstMouse = true;
    GLfloat lastX = 0;
    GLfloat lastY = 0;


    public:
    Camera();
    Camera(GLint);

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void processMouseCallback(double xpos, double ypos);
    
    void processScrollCallback(double xoffset, double yoffset);

    void updateView();

    void updateViewUserImpl();

    void processInput(GLFWwindow* window);

    void updateDelta();

    void updateUniform(Shader& shader) const;

    void setCameraSpeed(GLfloat);

    glm::vec3 getCameraPos() const;

};