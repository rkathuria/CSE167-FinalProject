#ifndef _PARTICLE_GENERATOR_H
#define _PARTICLE_GENERATOR_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Particle.h"
using namespace std;

class ParticleGenerator
{
public:
    ParticleGenerator();
    void update(glm::vec2 loc, int dir);
    void draw(GLuint posLoc, GLuint colorLoc);
    int nr_particles = 200;
    std::vector<Particle> particles;
    GLuint FirstUnusedParticle();
    void RespawnParticle(Particle &particle, glm::vec2 loc, glm::vec2 offset);
    
    glm::vec2 dirs[4] = {
        glm::vec2(-1.2f, 0.0f),
        glm::vec2(0.0f, -1.2f),
        glm::vec2(1.2f, 0.0f),
        glm::vec2(0.0f, 1.2f),
    };
    
};

#endif

