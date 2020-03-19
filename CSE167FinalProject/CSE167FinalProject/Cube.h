#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
public:
	Cube(float size);
	~Cube();
    
    glm::vec3 location;

	void draw();
	void update();
    void scale(glm::vec3 dir);
    void translate(glm::vec3 dir);
    void setColor(glm::vec3 c);

	void spin(float deg);
};

#endif

