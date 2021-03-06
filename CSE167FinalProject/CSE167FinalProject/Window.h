#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <memory>

#include "shader.h"

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "Particle.h"
#include "Maze.h"
#include "ShadowMapFBO.hpp"

class Window
{
public:
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void createShadow();
    static void createParticles();
    static void updateParticles();
    static void setupGlow();
    static unsigned int loadTexture(char const * path);
    
    static void renderScene();
};



#endif
