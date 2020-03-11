//
//  ShadowMapper.hpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/10/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Render.hpp"

#include <stdio.h>

class Scene {
private:
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    
    GLfloat near_plane = 1.f, far_plane = 7.5f;
public:
    GLuint depthMapFBO, depthMap;
    
    GLuint shader;
    
    Scene(int width, int height, GLuint shadowShader);
    ~Scene();
    
    void renderDepthMap(int width, int height);
    
    void renderDepth(glm::vec3 lightPos, int width, int height);
};

#endif /* _SCENE_HPP_ */
