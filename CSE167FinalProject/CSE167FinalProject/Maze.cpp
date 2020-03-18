#include "Maze.h"
#include <glm/gtx/string_cast.hpp>


Maze::Maze()
{
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            arr[i][j] = new Cube(10.0f);
            arr[i][j]->scale(glm::vec3(0.2f));
            arr[i][j]->translate(glm::vec3(2*i - rows + 1,2*j - cols + 1,0));
        }
    }
    
    cout << rows << " " << cols << endl;
    

    
    
    
}

void Maze::draw(GLuint shader) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(maze[i][j] == 1) {
                // update model
                glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,   glm::value_ptr(arr[i][j]->getModel()));
                
                // update color
                float r = rand() / float(RAND_MAX);
                float g = rand() / float(RAND_MAX);
                float b = rand() / float(RAND_MAX);
                glm::vec3 randColor(r,g,b);
                glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(randColor));
                
                // draw
                arr[i][j]->draw();
            }
                        
        }
    }
}
