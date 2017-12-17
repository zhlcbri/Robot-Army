#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#include <OpenGL/gl3.h> //
#include <OpenGL/glext.h> //
#include <OpenGL/gl.h> // Remove this line in future projects
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "Window.h"

using namespace std;
class OBJObject
{
private:
    std::vector<GLuint> indices;
    
public:
    OBJObject(const char* filepath);
    ~OBJObject();
    
    int count = 0;

    static float minX;
    static float minY;
    static float minZ;
    
    static float maxX;
    static float maxY;
    static float maxZ;
    
    static float midX;
    static float midY;
    static float midZ;
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    GLuint VBO2;
    
    GLuint uProjection, uModelview;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    
    glm::mat4 toWorld;
    glm::mat4 translateMat;
    glm::mat4 scaleMat;
    
    void parse(const char* filepath);
    void draw(GLuint shaderProgram); //
    void draw(GLuint shaderProgram, glm::mat4 C);
    void drawLines(GLuint shaderProgram, glm::mat4 C); // point rendering
    
    void update();
    void toCenter();
    void scale();
    void scale(float val);
    void spin(float deg); //
    
    void reset();
    void orbit(float angle, glm::vec3 axis);
    void translateXY(float xVal, float yVal);
    void translateZ(float zVal);
    
};

#endif





