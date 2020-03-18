#include "PointCloud.h"
#include <glm/gtx/string_cast.hpp>


PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read points from an obj file.
	 * Don't forget to load in the object normals for normal coloring as well
	 */
    
    FILE* fp;
    float x,y,z;
    float r,g,b;
    int j,k,l,m,n,o;
    maxes = glm::vec3(0,0,0);
    mins = glm::vec3(0,0,0);
    
    
    
    if (objFilename.compare("bunny.obj") == 0)
        spinDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    else
        spinDirection = glm::vec3(1.0f, 0.0f, 0.0f);

    fp = fopen(strcpy(new char[objFilename.length() + 1], objFilename.c_str()), "rb");
    if (fp==NULL) {
        cerr << "ERRORROROR" << endl;
        exit(-1);
    }
    char buffer[256];
    
    while(fgets(buffer, 256, fp)) {
        if ((buffer[0] =='v') && (buffer[1]==' ')) {
            sscanf(buffer, "v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            maxes = glm::vec3(max(maxes[0], x), max(maxes[1], y), max(maxes[2], z));
            mins = glm::vec3(min(mins[0], x), min(mins[1], y), min(mins[2], z));
            points.push_back(glm::vec3(x,y,z));
        }
        else if ((buffer[0]=='v') && (buffer[1]=='n')) {
            sscanf(buffer, "vn %f %f %f", &x, &y, &z);
            pointsNormal.push_back(glm::normalize(glm::vec3(x,y,z)*0.5f + 0.5f));
        }
        else if ((buffer[0] == 'f') && (buffer[1] == ' ')) {
            sscanf(buffer, "f %d//%d %d//%d %d//%d", &j,&k,&l,&m,&n,&o);
            triangles.push_back(j-1);
            triangles.push_back(l-1);
            triangles.push_back(n-1);
        }
    }
    fclose(fp);

    cout << points.size() << endl;
    cout << pointsNormal.size() << endl;
    cout << triangles.size() << endl;
    
    
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */
    glm::vec3 difference = maxes - mins;
    float maxDiff = max(difference[0], max(difference[1], difference[2]));
    
    
    average = glm::vec3((maxes[0] + mins[0])/2.0f, (maxes[1] + mins[1])/2.0f, (maxes[2] + mins[2])/2.0f);
    average = (glm::vec3(average[0] / maxDiff * 2.0f, average[1] / maxDiff* 2.0f, average[2] / maxDiff* 2.0f));

//    cout << glm::to_string(average) << endl;
//    cout << glm::to_string(maxes) << endl;
//    cout << glm::to_string(mins) << endl;
    


    
//    cout << glm::to_string(difference) << endl;
    
    points = translateToRange(points, maxDiff);
	 // Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 1, 1);
    distanceFromCenter = glm::vec3(0,0,0);
    location = glm::vec3(0,0,0);
    degrees = 0;
    originScale = glm::vec3(1,1,1);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/*
	 * TODO: Section 2 and 3. 	 
	 * Following the above example but for vertex normals, 
	 * 1) Generate a new vertex bufferbuffer,
	 * 2) Bind it as a GL_ARRAY_BUFFER type, 
	 * 3) Pass in the data 
	 * 4) Enable the next attribute array (which is 1)
	 * 5) Tell it how to process each vertex using glVertexAttribPointer
	 */
    glGenBuffers(1, &vbon);
    glBindBuffer(GL_ARRAY_BUFFER, vbon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pointsNormal.size(), pointsNormal.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), 0);
    
    
    glGenBuffers(1, &ebof);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebof);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangles.size(), triangles.data(), GL_STATIC_DRAW);
    
    
    
	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
    
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
    //spin(0.1f);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
    
    
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
    pointSize *= size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
    degrees += deg;
	model = glm::rotate(model, glm::radians(deg), spinDirection);
}



void PointCloud::translate(glm::vec3 dir) {
    distanceFromCenter += dir;
    PointCloud::translateHelper(dir);
}

void PointCloud::translateHelper(glm::vec3 dir) {
//    cout << "Direction:" + glm::to_string(dir) << endl;
//    cout << "Distance From Center: " + glm::to_string(distanceFromCenter) << endl;
    location += dir;
    model = glm::translate(glm::mat4(1.0f), dir) * model;
}

glm::vec3 PointCloud::getLocation() {
    return locationOfObj;
}

void PointCloud::translateTo(glm::vec3 dir) {
    glm::vec3 diff = dir - distanceFromCenter;
    translate(glm::vec3(diff[0], diff[1], 0));
}

void PointCloud::scale(glm::vec3 dir) {
    glm::vec3 temp = location;
    temp*=-1;
    originScale *= dir;
    model = glm::translate(glm::mat4(1.0f), temp) * model;
    model = glm::scale(dir) * model;
    model = glm::translate(glm::mat4(1.0f), location) * model;
}

void PointCloud::backToCenter() {
    distanceFromCenter *= -1;
    translateHelper(distanceFromCenter);
    distanceFromCenter = glm::vec3(0,0,0);
//    cout << "Distance From Center: " + glm::to_string(distanceFromCenter) << endl;
}

void PointCloud::trackballRotate(float angle, glm::vec3 dir) {
    model = glm::rotate(model, glm::radians(angle), dir);
}

void PointCloud::trackBallRotateWorld(float angle, glm::vec3 dir) {
    glm::mat4 trans = glm::mat4(1.0f);
    model = glm::rotate(trans, glm::radians(angle), dir) * model;
    locationOfObj = model * glm::vec4(location, 1.0f);
//    cout << glm::to_string(origin) << endl;
}

void PointCloud::resetOrientationScale() {
    degrees *= -1;
    model = glm::rotate(model, glm::radians(degrees), spinDirection);
    degrees = 0;
    

    originScale = glm::vec3(1.0f/originScale.x, 1.0f/originScale.y, 1.0f/originScale.z);
    glm::vec3 temp = location;
    temp*=-1;
    
    model = glm::translate(glm::mat4(1.0f), temp) * model;
    model = glm::scale(originScale) * model;
    model = glm::translate(glm::mat4(1.0f), location) * model;
    
    originScale = glm::vec3(1,1,1);
}

std::vector<glm::vec3> PointCloud::getPoints() {
    return points;
}

std::vector<glm::vec3> PointCloud::getPointsNormal() {
    return pointsNormal;
}

void PointCloud::initialTranslate() {
    model = glm::translate(glm::mat4(1.0f), -1.0f * average) * model;
}

std::vector<glm::vec3> PointCloud::translateToRange(std::vector<glm::vec3> points, float diff) {
    std::vector<glm::vec3> scaledPoints;
    for(int i = 0; i != points.size(); i++) {

        scaledPoints.push_back(glm::vec3(points[i][0] / diff * 2.0f, points[i][1] / diff* 2.0f, points[i][2] / diff* 2.0f));
    }
    
    return scaledPoints;
    
}

void PointCloud::setColor(glm::vec3 c) {
    color = c;
}

