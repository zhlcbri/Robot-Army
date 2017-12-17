#ifndef Transform_h
#define Transform_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <list>
#include "Node.h"
#include "Group.h"

// Derived from Node
class Transform: public Node {
public:
    
    glm::mat4 M; // 4x4 transformation matrix
    glm::vec3 origin; // vec3 or 4?
    float radius; // hard-coded as 55
//    static float midY;
    
    bool LEFT = false;
    bool RIGHT = false;
    
    bool GEODE = false;

    Transform(glm::mat4 transMat);
    ~Transform();
    
    void setM(glm::mat4 newM);
    
    void init();
    
    list<Node*> children; // list of pointers to child nodes
    list<Node*>::iterator it;
    
    void addChild(Node* node);
    void removeChild(Node* node);
    
    // draw() needs to traverse the list of children and call
    // each child nodes from the list
    // when draw(C) is called, multiply matrix M with C
    void draw(GLuint shaderProgram, glm::mat4 C);
    
    void update();
    void walk(float deg);
//    void spin(float deg);
    void scale(float val);
    void drawSphere(GLuint shaderProgram, OBJObject * sphere);
    
    void translateX(float xVal);
    
//    glm::vec3 origin_in_CVV(glm::vec3 x);
//    float dist_to_plane(glm::vec3 x_prime, glm::vec3 normal);
    float dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal);
    int in_six_planes(glm::vec3 origin);
    float radius_in_NDC(float radius, glm::vec3 normal);
    
    bool check_origin_NDC(glm::vec3 origin, glm::vec3 normal);
};

#endif /* Transform_h */
