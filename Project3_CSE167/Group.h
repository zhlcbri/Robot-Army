#ifndef Group_h
#define Group_h

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
#include "Node.h"

class Group: public Node {
    virtual void draw(GLuint shaderProgram, glm::mat4 M) = 0;
    
    virtual void update() = 0;
//    virtual void spin(float deg) = 0; //
    virtual void scale(float val) = 0;
};

#endif
