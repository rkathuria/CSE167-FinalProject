//
//  Particle.h
//  CSE167FinalProject
//
//  Created by Raman Kathuria on 3/4/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#ifndef Particle_h
#define Particle_h


#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class Particle
{
private:
    GLuint vao;
    GLuint texture;
    
public:
    Particle(glm::vec2 pos, glm::vec2 vel, glm::vec4 color, GLfloat life);
    Particle();
    ~Particle();

    void draw();
    void update();
    void init();
    
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec4 color;
    GLfloat life;

};






#endif /* Particle_h */
