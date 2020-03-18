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
    
    // Generate framebuffer
    glGenFramebuffers(1, &depthMapFBO);
    // bind fbo to texture
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    
    // draw everything in to this fbo => texture    ;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // bind fbo to the screen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    
    
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMapFBO::renderDepthMap(GLuint width, GLuint height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}
