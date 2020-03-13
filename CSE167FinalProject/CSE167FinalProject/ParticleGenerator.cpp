#include "ParticleGenerator.h"
#include <glm/gtx/string_cast.hpp>


ParticleGenerator::ParticleGenerator()
{
    for (GLuint i = 0; i < nr_particles; ++i)
        particles.push_back(Particle());
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::FirstUnusedParticle()
{
    // Search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < nr_particles; ++i){
        if (particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle &particle, glm::vec2 offset)
{
    GLfloat random = ((rand() % 100) - 50) / 100.0f;
    GLfloat random2 = ((rand() % 100) - 50) / 100.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.pos = glm::vec2(random, random2) + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.vel = glm::vec2(2,0) * 0.1f;
}

void ParticleGenerator::update() {
    float dt = 0.2;
    GLuint nr_new_particles = 2;
    // Add new particles
    for (GLuint i = 0; i < nr_new_particles; ++i)
    {
        int unusedParticle = FirstUnusedParticle();
        RespawnParticle(particles[unusedParticle], glm::vec2(-1.2f, 0.5f));
    }
    // Uupdate all particles
    for (GLuint i = 0; i < nr_particles; ++i)
    {
        Particle &p = particles[i];
        p.life -= dt; // reduce life
        if (p.life > 0.0f)
        {    // particle is alive, thus update
            p.pos -= p.vel * dt;
            p.color.a -= dt * 2.5;
        }
    }
}

void ParticleGenerator::draw(GLuint posLoc, GLuint colorLoc) {
    for(Particle p : particles) {
        if(p.life > 0) {
            glUniform2fv(posLoc, 1, glm::value_ptr(p.pos));
            glUniform4fv(colorLoc, 1, glm::value_ptr(p.color));
            p.draw();
        }
    }
}
