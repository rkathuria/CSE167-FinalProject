#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Object.h"
 class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> pointsNormal;
    glm::vec3 spinDirection;
	GLuint vao, vbo, vbon, ebof;
	GLfloat pointSize;
    glm::vec3 distanceFromCenter;

    float degrees;
    glm::vec3 originScale;
    std::vector<unsigned int> triangles;
    glm::vec3 maxes;
    glm::vec3 mins = glm::vec3(100,100,100);
    glm::vec3 average;
    glm::vec3 locationOfObj;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
    
    void setColor(glm::vec3 c);


	void draw();
	void update();
    glm::vec3 location;
	void updatePointSize(GLfloat size);
	void spin(float deg);
    void translate(glm::vec3 dir);
    void translateHelper(glm::vec3 dir);
    void backToCenter();
    void resetOrientationScale();
    void scale(glm::vec3 dir);
    std::vector<glm::vec3> getPoints();
    std::vector<glm::vec3> getPointsNormal();
    void translateTo(glm::vec3 dir);
    void initialTranslate();
    glm::vec3 getLocation();
    
    void trackballRotate(float angle, glm::vec3 dir);
    std::vector<glm::vec3> translateToRange(std::vector<glm::vec3> points, float diff);
    void trackBallRotateWorld(float angle, glm::vec3 dir);
    
    
};

#endif

