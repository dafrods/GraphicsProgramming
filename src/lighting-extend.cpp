#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opengl_util.h>
#include <spdlog/spdlog.h>

#include <logger.h>
#include <shader.h>
#include <camera.h>

#include <iterator>
#include <filesystem>
#include <format>

#define STB_IMAGE_IMPLEMENTATION // This line should be defined in one source file only
#include <stb_image.h>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

const char* FILENAME = "lighting-extend";

// ************************************************************************************************
//  External Sources
// ************************************************************************************************

// Consider script for making shader files
const std::string vShaderPath = std::format("assets/shaders/{}.vert", FILENAME);
const std::string fShaderPath = std::format("assets/shaders/{}.frag", FILENAME);
const std::string fLightShaderPath = std::format("assets/shaders/{}_Light.frag", FILENAME);

const char* texturePath1 = "assets/textures/container.jpg";
const char* texturePath2 = "assets/textures/awesomeface.png";

// ************************************************************************************************

int main(int argc, char* argv[])
{

    // ----------------------------------------------------
    //  Init
    // ----------------------------------------------------

    std::filesystem::path exePath = std::filesystem::canonical(argv[0]);
    std::filesystem::path exeDir = exePath.parent_path();

    std::filesystem::path logPath = exeDir;

    loggerInit(exeDir.string().c_str());

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, FILENAME, NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

// ************************************************************************************************
//  Object Initialization
// ************************************************************************************************

Camera camera{FREECAM};

// ----------------------------------------------------
//  glfw window contexts
// ----------------------------------------------------

glfwMakeContextCurrent(window);
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

// set Passthrough
glfwSetWindowUserPointer(window, &camera);

// input callbacks
glfwSetCursorPosCallback(window, camera.mouseCallback);
glfwSetScrollCallback(window, camera.scrollCallback);

// Mouse Capture
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// glad: load all OpenGL function pointers
// ---------------------------------------
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    spdlog::error("Failed to initialize GLAD");
    return -1;
}

    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST); 
    
    Shader lightShader{vShaderPath.c_str(), fShaderPath.c_str()};
    Shader shader{vShaderPath.c_str(), fLightShaderPath.c_str()};

// ************************************************************************************************
//  Vertex Handling
// ************************************************************************************************
    
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  -1.0f,  -1.0f, -1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  -1.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,   1.0f,   1.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,   1.0f,   1.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  -1.0f,   1.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  -1.0f,  -1.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  -1.0f,  -1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  -1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  -1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,   1.0f,   1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,   1.0f,  -1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  -1.0f,  -1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  -1.0f,  -1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  -1.0f,   1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,   1.0f,   1.0f,

     0.5f,  0.5f,  0.5f,  1.0f,   1.0f,   1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,   1.0f,  -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  -1.0f,  -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  -1.0f,  -1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  -1.0f,   1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,   1.0f,   1.0f,

    -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,  -1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, -1.0f,  -1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, -1.0f,   1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, -1.0f,   1.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,   1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,  -1.0f,

    -0.5f,  0.5f, -0.5f,  -1.0f,  1.0f,  -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  1.0f,  -1.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  1.0f,   1.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  1.0f,   1.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  1.0f,   1.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  1.0f,  -1.0f
};

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // Handlers
    unsigned int VBO, VAO;

    // Generate Handlers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and Send Data to Buffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal Pos
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ------------------------------------------
    //  Lighting
    // ------------------------------------------

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


// ************************************************************************************************
//  Lighting
// ************************************************************************************************
    
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// ************************************************************************************************
//  Main Loop
// ************************************************************************************************

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    // shader.use();
    
    // shader.setInt("texture1", 0);
    // shader.setInt("texture2", 1);
    
    // shader.use();
    
    camera.setCameraSpeed(0.01);
    glm::mat4 model = glm::mat4(1.0f);
    
    GLfloat speed = 0.01;
    GLfloat direction = -1;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, camera);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0,0,0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        camera.updateDelta();
        camera.updateView();


        shader.use();


        for(unsigned int i = 0; i < std::size(cubePositions); ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            
            if(i == 1 || (i % 3 == 0))
            {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            else 
            {
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            
            shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            shader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
            shader.setMat4("model", model);
            shader.setVec3("lightPos", lightPos); 
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        camera.updateUniform(shader);
        
        
        lightShader.use();
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        lightShader.setMat4("model", model);
    
        glDrawArrays(GL_TRIANGLES, 0, 36);
        camera.updateUniform(lightShader);

        if (lightPos.x >= 5)
            direction = -1;
        else if(lightPos.x <= -5)
            direction = 1;
            
        lightPos.x += speed * direction;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

