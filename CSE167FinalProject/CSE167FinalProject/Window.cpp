#include "Window.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <glm/gtx/string_cast.hpp>
#include "ParticleGenerator.h"

/*
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace
{
int width, height;
std::string windowTitle("GLFW Starter Project");

Cube* cube;
Maze* maze;
//Object* currentObj; // The object currently displaying.

glm::vec3 eye(0, 0, 20); // Camera position.
glm::vec3 center(0, 0, 0); // The point we are looking at.
glm::vec3 up(0, 1, 0); // The up direction of the camera.
float fovy = 60;
float near = 1;
float far = 1000;
glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
glm::mat4 projection; // Projection matrix.

GLuint program; // The shader program id.
GLuint projectionLoc; // Location of projection in shader.
GLuint viewLoc; // Location of view in shader.
GLuint modelLoc; // Location of model in shader.
GLuint colorLoc; // Location of color in shader.

GLuint shadowProg;

GLuint lightingProg;

GLuint particleProg;

//PointCloud* cloud;
ParticleGenerator* generator;

std::pair<glm::vec3, glm::vec3> directionalLight(glm::vec3(-10,0,0), glm::vec3(1.0f,1.0f,1.0f));
};

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    // Check the shader program.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    // Create shader program for the particle light
    particleProg = LoadShaders("shaders/particle.vert", "shaders/particle.frag");
    if (!particleProg)
    {
        std::cerr << "Failed to initialize lighting program" << std::endl;
        return false;
    }
        
    return true;
}



bool Window::initializeObjects()
{
    // Construct particle generator
    generator = new ParticleGenerator();
    
    // Create a cube of size 5.
    cube = new Cube(5.0f);

//    cloud = new PointCloud("objFolder/dragon.obj", 2.0f);
    // Set cube to be the first to display
//    currentObj = cloud;

//    cloud->scale(glm::vec3(8,8,8));
//    cloud->translate(glm::vec3(0,-5,0));
    
    maze = new Maze();

    return true;
}


void Window::cleanUp()
{
    // Deallcoate the objects.
    delete generator;
//    delete cube;
//    delete cloud;
    delete maze;

    // Delete the shader program.
    glDeleteProgram(program);
    glDeleteProgram(particleProg);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    width = w;
    height = h;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
        (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
//    generator->update();

}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //DRAWING PARTICLES
//    glUseProgram(particleProg);
//    glUniformMatrix4fv(glGetUniformLocation(particleProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniformMatrix4fv(glGetUniformLocation(particleProg, "view"), 1, GL_FALSE, glm::value_ptr(view));
//
//    generator->draw(glGetUniformLocation(shadowProg, "offset"), glGetUniformLocation(shadowProg, "color"));

    
    
    //DRAWING MAZE
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(cube->getModel()));
    maze->draw(glGetUniformLocation(program, "model"), glGetUniformLocation(program, "color"));
//    cube->draw();
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
     * TODO: Modify below to add your key callbacks.
     */

     // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                // Set currentObj to cube
//                currentObj = cube;
                break;
            case GLFW_KEY_G:
//                cloud->translate(glm::vec3(2,0,0));
                break;
            default:
                break;
        }
    }
}
