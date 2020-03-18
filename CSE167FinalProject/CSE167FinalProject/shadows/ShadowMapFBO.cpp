//
//  ShadowMapFBO.cpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/16/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#include "ShadowMapFBO.hpp"

ShadowMapFBO::ShadowMapFBO(GLuint width, GLuint height)
{
    // TODO:
    // bind framebuffer
    // draw everything in to this fbo => texture
    
    // bind to screen buffer
    // bind the texture(depth map) => texture()
    
    glGenFramebuffers(1, &fbo);
    // bind fbo to texture
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // draw everything in to this fbo => texture
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // bind fbo to the screen buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    // No color buffer is drawn to
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "framebuffer has not completed projecting to the screen" << std::endl;
    }
}

void ShadowMapFBO::renderDepthMap(GLuint width, GLuint height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
}
