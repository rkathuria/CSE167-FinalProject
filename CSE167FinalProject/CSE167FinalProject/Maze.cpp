#include "Maze.h"
#include <glm/gtx/string_cast.hpp>


Maze::Maze()
{
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            arr[i][j] = new Cube(10.0f);
            arr[i][j]->scale(glm::vec3(0.2f, 0.2f, 0.05f));
            arr[i][j]->translate(glm::vec3(2*i - rows + 1,2*j - cols + 1,0));
        }
    }
}

bool Maze::checkCollision(glm::vec3 objLoc, float size) {

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(maze[i][j] == 1) {
                glm::vec3 loc = arr[i][j]->location;
                if(checkCollision(objLoc[0], objLoc[1], size, loc[0], loc[1], 2))
                    return true;
            }
        }
    }
    return false;
}

bool Maze::checkCollision(float x, float y, float size, float x2, float y2, float size2) {
    bool collisionX = (x + size >= x2) && (x + size <= x2 + size2);
    // Collision y-axis?
    bool collisionY = (y + size >= y2) && (y + size <= y2 + size2);
    // Collision only if on both axes
    return collisionX && collisionY;
}

void Maze::draw(GLuint modelLoc, GLuint colorLoc) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(maze[i][j] == 1) {
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE,   glm::value_ptr(arr[i][j]->getModel()));
                arr[i][j]->draw();
            }
                        
        }
    }
}
