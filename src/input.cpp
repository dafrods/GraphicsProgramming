#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <opengl_util.h>
#include <spdlog/spdlog.h>

#include <logger.h>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// ************************************************************************************************
//  Input Logic
// ************************************************************************************************
const GLubyte GL_ALPHA_START = 65;
const GLubyte GL_ALPHA_END   = 90;

const GLubyte ASCII_DIFF_LOWERCASE = 32;

void yprocessInput(GLFWwindow *window);
// ************************************************************************************************
int main()
{
    loggerInit();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);

    glfwSetErrorCallback([](int error, const char* description) {
        spdlog::error("GLFW Error: 0x{0:x} - {1}", error, description);
    });

    // glfw window creationa
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

// ************************************************************************************************
//  Main Loop
// ************************************************************************************************

    while (!glfwWindowShouldClose(window))
    {

        yprocessInput(window);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void yprocessInput(GLFWwindow *window)
{

    static bool keyStates[GLFW_KEY_LAST + 1] = {};

    for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; key++)
    {
        bool pressed = glfwGetKey(window, key) == GLFW_PRESS;

        if (pressed && !keyStates[key])
        {
            spdlog::info("Pressed key: {}", static_cast<char>(key));
        }

        keyStates[key] = pressed;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}