//
//  Render.hpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/10/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

void renderScene(const GLuint shader);

#endif /* Render_hpp */
