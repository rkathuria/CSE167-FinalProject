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
int width, height;

Cube* cube;
Maze* maze;
Object* currentObj; // The object currently displaying.

glm::vec3 cameraEye(0, 0, 20); // Camera position.
glm::vec3 cameraCenter(0, 0, 0); // The point we are looking at.
glm::vec3 cameraUp(0, 1, 0); // The up direction of the camera.

glm::vec3 lightPosition;
glm::vec3 lightCenter;
glm::vec3 lightUp(0,0,1);
bool useLightView = false;

float fovy = 60;
float near = 1;
float far = 1000;
glm::mat4 view = glm::lookAt(cameraEye, cameraCenter, cameraUp); // View matrix, defined by eye, center and up.
glm::mat4 projection; // Projection matrix.

int dir = 0;

bool collisionToggle = true;
bool cubeDraw = false;

GLuint program; // The shader program id.
GLuint projectionLoc; // Location of projection in shader.
GLuint viewLoc; // Location of view in shader.
GLuint modelLoc; // Location of model in shader.
GLuint colorLoc; // Location of color in shader.

GLuint shadowProg;

ShadowMapFBO *shadowMap;
GLuint shadowShader;
GLuint depthProg;


GLuint particleProg;
bool particleToggle = true;
bool cubeDraw = true;
bool glowToggle = false;

GLuint blurProg;
    GLuint glowProg;
    unsigned int hdrFBO;
PointCloud* cloud;
ParticleGenerator* generator;
unsigned int quadVAO;
unsigned int textureColorbuffer;

PointCloud* cloud;
ParticleGenerator* generator;
};

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    particleProg = LoadShaders("shaders/particle.vert", "shaders/particle.frag");
    glowBlurProg = LoadShaders("shaders/lighting.vert", "shaders/lighting.frag");
//    glowProg = LoadShaders("shaders/glow.vert", "shaders/glow.frag");
//    blurProg = LoadShaders("shaders/blur.vert", "shaders/blur.frag");
    
    // Check the shader program.
    if (!program || !particleProg || !glowBlurProg || !glowProg || !blurProg || !depthProg)
    {
        std::cerr << "Failed to initialize a shader program" << std::endl;
        return false;
    }
    
    if (!glowBlurProg)
    {
        std::cerr << "Failed to initialize lighting program" << std::endl;
        return false;
    }
//
//    if (!glowProg)
//    {
//        std::cerr << "Failed to initialize lighting program" << std::endl;
//        return false;
//    }
//
//    if (!blurProg)
//    {
//        std::cerr << "Failed to initialize lighting program" << std::endl;
//        return false;
//    }

    glUniform1d(glGetAttribLocation(glowProg, "scene"), 0);
    glUniform1d(glGetAttribLocation(glowProg, "bloomBlur"), 1);
    
    generator = new ParticleGenerator();

    setupGlow();
    return true;
}

void Window::setupGlow() {
    int SCR_WIDTH=width;
    int SCR_HEIGHT=height;
    
    
    unsigned int quadVBO;
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    


    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)


    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    

    
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



bool Window::initializeObjects()
{
    // Create a dragon sprite which the user will navigate
    cloud = new PointCloud("objFolder/dragon.obj", 2.0f);
    cloud->setColor(glm::vec3(0.5f));
    cloud->scale(glm::vec3(1.0f, 1.45f, 1.0f));
    cloud->translate(glm::vec3(-12,-1.0f,0));

    // Create a cube of size 5.
    cube = new Cube(10.0f);
    cube->scale(glm::vec3(0.2f, 0.2f, 0.05f));
    cube->translate(glm::vec3(-12,0,0));
    
    maze = new Maze();

    return true;
}


void Window::cleanUp()
{
    // Deallcoate the objects.
    delete generator;
    delete cube;
    delete cloud;
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
    width = w*2;
    height = h*2;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
        (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    generator->update(glm::vec2(cube->location[0], cube->location[1]), dir);
}

void Window::displayCallback(GLFWwindow* window)
{
    
    if(glowToggle)
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(particleProg);
    glUniformMatrix4fv(glGetUniformLocation(particleProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(particleProg, "view"), 1, GL_FALSE, glm::value_ptr(view));

    if(particleToggle)
        generator->draw(glGetUniformLocation(particleProg, "offset"), glGetUniformLocation(particleProg, "color"));

    
    glUseProgram(program);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    if(!cubeDraw) {
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(cube->getModel()));
        glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(cube->getColor()));
        cube->draw();
    }
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(cloud->getModel()));
    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(cloud->getColor()));
    cloud->draw();


    maze->draw(glGetUniformLocation(program, "model"), glGetUniformLocation(program, "color"));
        
    if(glowToggle) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(glowBlurProg);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);    // use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::renderScene()
{
    
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
            case GLFW_KEY_2:
                if (useLightView)
                {
                    view = glm::lookAt(cameraEye, cameraCenter, cameraUp);
                    projection = glm::perspective(glm::radians(fovy),
                    (float)width / (float)height, near, far);
                }
                else
                {
                    view = glm::lookAt(lightPosition, lightCenter, lightUp);
                    projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near, far);
                }
                useLightView = !useLightView;
                break;
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                // Set currentObj to cube
                currentObj = cube;
                break;
            case GLFW_KEY_A:
                cube->translate(glm::vec3(-1,0,0));
                cloud->translate(glm::vec3(-1,0,0));
                if(maze->checkCollision(cube->location, 1) && collisionToggle) {
                    cube->translate(glm::vec3(1,0,0));
                    cloud->translate(glm::vec3(1,0,0));
                    cube->setColor(glm::vec3(1,0,0));
                    cloud->setColor(glm::vec3(1,0,0));
                }
                else {
                    dir = 2;
                    cloud->setColor(glm::vec3(0.5f));
                    cube->setColor(glm::vec3(1));
                }
                break;
            case GLFW_KEY_S:
                cube->translate(glm::vec3(0,-1,0));
                cloud->translate(glm::vec3(0,-1,0));
                if(maze->checkCollision(cube->location, 1) && collisionToggle) {
                    cube->translate(glm::vec3(0,1,0));
                    cloud->translate(glm::vec3(0,1,0));
                    cube->setColor(glm::vec3(1,0,0));
                    cloud->setColor(glm::vec3(1,0,0));

                }
                else {
                    dir = 3;
                    cloud->setColor(glm::vec3(0.5f));
                    cube->setColor(glm::vec3(1));
                }
                break;
            case GLFW_KEY_W:
                cube->translate(glm::vec3(0,1,0));
                cloud->translate(glm::vec3(0,1,0));
                if(maze->checkCollision(cube->location, 1) && collisionToggle) {
                    cube->translate(glm::vec3(0,-1,0));
                    cloud->translate(glm::vec3(0,-1,0));
                    cube->setColor(glm::vec3(1,0,0));
                    cloud->setColor(glm::vec3(1,0,0));

                }
                else {
                    dir = 1;
                    cloud->setColor(glm::vec3(0.5f));
                    cube->setColor(glm::vec3(1));
                }
                break;
            case GLFW_KEY_D:
                cube->translate(glm::vec3(1,0,0));
                cloud->translate(glm::vec3(1,0,0));
                if(maze->checkCollision(cube->location, 1) && collisionToggle) {
                    cube->translate(glm::vec3(-1,0,0));
                    cloud->translate(glm::vec3(-1,0,0));
                    cube->setColor(glm::vec3(1,0,0));
                    cloud->setColor(glm::vec3(1,0,0));

                }
                else {
                    dir = 0;
                    cube->setColor(glm::vec3(1));
                    cloud->setColor(glm::vec3(0.5f));
                }
                break;
                
            case GLFW_KEY_P:
                particleToggle = !particleToggle;
                break;
            case GLFW_KEY_C:
                collisionToggle = !collisionToggle;
                break;
            case GLFW_KEY_V:
                cubeDraw = !cubeDraw;
                break;
            case GLFW_KEY_G:
                glowToggle = !glowToggle;
                break;

            default:
                break;
        }
    }
}

