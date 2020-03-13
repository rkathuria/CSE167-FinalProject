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
	std::string windowTitle("GLFW Starter Project");

	Cube* cube;
	Object* currentObj; // The object currently displaying.

	glm::vec3 eye(0, 0, 20); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint shader; // The shader program id.
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

    Scene *scene;
    GLuint shadowShader;
};

namespace app
{
    int width, height;
}

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	shader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// Check the shader program.
	if (!shader)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(shader);
    
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(shader, "projection");
	viewLoc = glGetUniformLocation(shader, "view");
	modelLoc = glGetUniformLocation(shader, "model");
	colorLoc = glGetUniformLocation(shader, "color");
    
    
    // Create the shadow shader program
    shadowShader = LoadShaders("shaders/shadow.vert", "shaders/shadow.frag");
    if (!shadowShader) {
        std::cerr << "Failed to initialize the shadow shader program" << std::endl;
        return false;
    }
    

	return true;
}



bool Window::initializeObjects()
{
    // Create a cube of size 5.
    cube = new Cube(5.0f);
//
//    cloud = new PointCloud("objFolder/dragon.obj", 2.0f);
//    // Set cube to be the first to display
//    currentObj = cloud;

//    cloud->scale(glm::vec3(8,8,8));
//    cloud->translate(glm::vec3(0,-5,0));
    
//    maze = new Maze();

    return true;
}


void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;

	// Delete the shader program.
	glDeleteProgram(shader);
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
	glfwGetFramebufferSize(window, &app::width, &app::height);
#endif
	app::width = w;
	app::height = h;
	// Set the viewport size.
	glViewport(0, 0, app::width, app::height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)app::width / (float)app::height, near, far);
}

void Window::idleCallback()
{
//    generator->update();

}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shader);
    
    glm::mat4 pointlightView = glm::lookAt(eye, center, up);
    
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(cube->getModel()));
    glUniform3f(glGetUniformLocation(shader,"color"), 1, 1, 1);
    cube->draw();
    
    
    glm::mat4 aerialView = glm::lookAt(glm::vec3(0,0,20), glm::vec3(0,0,0), glm::vec3(0,1,20));
    
    //DRAWING PARTICLES
//    glUseProgram(particleProg);
//    glUniformMatrix4fv(glGetUniformLocation(particleProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniformMatrix4fv(glGetUniformLocation(particleProg, "view"), 1, GL_FALSE, glm::value_ptr(view));
////
//    generator->draw(glGetUniformLocation(shadowProg, "offset"), glGetUniformLocation(shadowProg, "color"));

    
    
    //DRAWING MAZE
//    glUseProgram(program);
//
//    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
//    maze->draw(glGetUniformLocation(program, "model"), glGetUniformLocation(program, "color"));
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
                currentObj = cube;
                break;
            case GLFW_KEY_G:
//                cloud->translate(glm::vec3(2,0,0));
                break;
            default:
                break;
        }
    }
}
