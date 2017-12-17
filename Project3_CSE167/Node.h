#ifndef Node_h
#define Node_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "stdio.h"
#include "OBJObject.h"
using namespace std;

class Node {
public:
    glm::mat4 M; // 4x4 transformation matrix
    bool GEODE = false;
    glm::vec3 origin;
    
    virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;  // Matrix4
    // separates bounding sphere updates from rendering
    virtual void update() = 0;
//    virtual void spin(float deg) = 0;//
    virtual void scale(float val) = 0;
    
    virtual void drawSphere(GLuint shaderProgram, OBJObject * sphere) = 0;
    
    
    //    glm::vec3 origin_in_CVV(glm::vec3 x);
    //    float dist_to_plane(glm::vec3 x_prime, glm::vec3 normal);
    virtual float dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal) = 0;
    virtual int in_six_planes(glm::vec3 origin) = 0;
    virtual float radius_in_NDC(float radius, glm::vec3 normal) = 0;

};

#endif
