//
//  Skybox.hpp
//  Project3_CSE167
//
//  Created by Brittany Zhang on 11/5/17.
//  Copyright © 2017 ManianaBonita. All rights reserved.
//

#ifndef Skybox_h
#define Skybox_h

#include <iostream>
#include <stdio.h>
#include <vector>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Cube.h"

using namespace std;

class Skybox {
public:
    Skybox();
    ~Skybox();
    
    glm::mat4 toWorld;
    //unsigned int cubemapTexture;
    
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uModelview;
    
//    unsigned char* loadPPM(const char* filename, int& width, int& height);
    unsigned int loadCubemap(vector<string> faces);
    
    void draw(GLuint);
    
    vector<string> faces;
    
    // Define the coordinates and indices needed to draw the cube. Note that it is not necessary
    // to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
    // This just looks nicer since it's easy to tell what coordinates/indices belong where.
    const GLfloat vertices[8][3] = {
        // "Front" vertices
        {-700.0, -700.0,  700.0}, {700.0, -700.0,  700.0}, {700.0,  700.0,  700.0}, {-700.0,  700.0,  700.0},
//        {-900.0, -900.0,  900.0}, {900.0, -900.0,  900.0}, {900.0,  900.0,  900.0}, {-900.0,  900.0,  900.0},
        // "Back" vertices
        {-700.0, -700.0, -700.0}, {700.0, -700.0, -700.0}, {700.0,  700.0, -700.0}, {-700.0,  700.0, -700.0}
//        {-900.0, -900.0, -900.0}, {900.0, -900.0, -900.0}, {900.0,  900.0, -900.0}, {-900.0,  900.0, -900.0}
    };
    
    // Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
    // This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
    const GLuint indices[6][6] = {
        // Front face
        {0, 1, 2, 2, 3, 0},
//        {2, 1, 0, 0, 3, 2},
        // Top face
        {1, 5, 6, 6, 2, 1},
//        {6, 5, 1, 1, 2, 6},
        // Back face
        {7, 6, 5, 5, 4, 7},
//        {5, 6, 7, 7, 4, 5},
        // Bottom face
        {4, 0, 3, 3, 7, 4},
//        {3, 0, 4, 4, 7, 3},
        // Left face
        {4, 5, 1, 1, 0, 4},
//        {1, 5, 4, 4, 0, 1},
        // Right face
        {3, 2, 6, 6, 7, 3}
//        {6, 2, 3, 3, 7, 6}
    };
};

#endif
