//
//  Particle.cpp
//  CSE167FinalProject
//
//  Created by Raman Kathuria on 3/4/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#include "Particle.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>


Particle::Particle(glm::vec2 pos, glm::vec2 vel, glm::vec4 color, GLfloat life) {
    this->pos = pos;
    this->vel = vel;
    this->color = color;
    this->life = life;
    
    init();
}

Particle::Particle() {
    this->pos = glm::vec2(2,0);
    this->vel = glm::vec2(0,0);
    this->color = glm::vec4(1.0);
    this->life = 0.0f;
    
    init();
}

void Particle::init() {
    GLuint vbo;
//    GLfloat particle_quad[] = {
//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 0.0f, 0.0f,
//
//        0.0f, 1.0f, 0.0f, 1.0f,
//        1.0f, 1.0f, 1.0f, 1.0f,
//        1.0f, 0.0f, 1.0f, 0.0f
//    };
    GLfloat particle_quad[] = {
        -0.05f, 0.05f, 0.0f, 1.0f,
        0.05f, -0.05f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f,

        -0.05f, 0.05f, 0.0f, 1.0f,
        0.05f, 0.05f, 1.0f, 1.0f,
        0.05f, -0.05f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)

    // load and generate the texture
    int width, height, nrChannels;
//    std::string filePath = "images/particle.png";
    unsigned char *data = stbi_load("images/particle6.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Particle::~Particle()
{
//    // Delete the VBOs and the VAO.
//    glDeleteBuffers(2, vbos);
//    glDeleteVertexArrays(1, &vao);
}

void Particle::draw() {
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

