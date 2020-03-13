//
//  Render.cpp
//  CSE167FinalProject
//
//  Created by Abinesh Sarvepalli on 3/10/20.
//  Copyright © 2020 Raman Kathuria. All rights reserved.
//

#include "Render.hpp"

void renderScene(GLuint shader) {
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glBindVertexArray(<#GLuint array#>);
}
