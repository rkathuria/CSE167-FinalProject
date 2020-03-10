#ifndef _MAZE_H_
#define _MAZE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Cube.h"


class Maze 
{
public:
	Maze();
    Cube* cube;
    void draw(GLuint modelLoc, GLuint colorLoc);
    Cube* arr[100][100];
    int maze[10][10] =
    {
        {1,1,1,1,1,1,0,1,1,1},
        {1,1,1,1,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,0,1},
        {1,1,0,0,0,0,0,1,0,1},
        {1,1,0,1,1,1,0,1,0,1},
        {1,1,0,0,0,1,0,1,0,1},
        {1,1,1,1,0,1,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,1}
    };
    int rows = sizeof(maze)/sizeof(maze[0]);
    int cols = sizeof(maze[0])/sizeof(maze[0][0]);
};

#endif

