//
//  ShadowMapper.cpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/10/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene(int width, int height, GLuint shadowShader) : shader(shadowShader) {
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glGenFramebuffers(1, &depthMapFBO);
    
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Scene::~Scene() {
    glDeleteTextures(1, &depthMap);
}

//void ShadowMapper::renderDepthMap(int width, int height) {
//    // 1. first render to depth map
//    glViewport(0, 0, width, height);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glClear(GL_DEPTH_BUFFER_BIT);
////    ConfigureShaderAndMatrices();
////    RenderScene();
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    // 2. then render scene as normal with shadow mapping (using depth map)
////    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////    ConfigureShaderAndMatrices();
//    glBindTexture(GL_TEXTURE_2D, depthMap);
////    RenderScene();
//}


void Scene::renderDepth(glm::vec3 lightPos, int width, int height) {
    // Render depth of scene to texture (from light's perspective)
    // -----------------------------------------------------------
    lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.f), glm::vec3(0.0, 1.f, 0));
    lightSpaceMatrix = lightProjection * lightView;
    
    // render scene from light's point of view
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
//    Render::renderScene(shader);
}
