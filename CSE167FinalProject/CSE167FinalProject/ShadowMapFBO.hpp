//
//  ShadowMapFBO.hpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/16/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#ifndef ShadowMapFBO_hpp
#define ShadowMapFBO_hpp

#include <stdio.h>

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

#include <iostream>

class ShadowMapFBO
{
public:
    ShadowMapFBO(GLuint width, GLuint height);
    ~ShadowMapFBO();
    
//    bool Init(unsigned int windowWidth, unsigned int windowHeight);
    
    void renderDepthMap(GLuint width, GLuint height);
    
private:
    GLuint depthMap;
    GLuint depthMapFBO;
};

#endif /* ShadowMapFBO_hpp */
