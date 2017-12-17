#include "Skybox.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

unsigned int cubemapTexture;

Skybox::Skybox()
{
    toWorld = glm::mat4(1.0f);
    
    faces = {
        "right.ppm",
        "left.ppm",
        "top.ppm",
        "bottom.ppm",
        "back.ppm",
        "front.ppm"
    };
    
    cubemapTexture = loadCubemap(faces);
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

//unsigned char* Skybox::loadPPM(const char* filename, int& width, int& height)
//{
//    const int BUFSIZE = 128;
//    FILE* fp;
//    unsigned int read;
//    unsigned char* rawData;
//    char buf[3][BUFSIZE];
//    char* retval_fgets;
//    size_t retval_sscanf;
//
//    if ( (fp=fopen(filename, "rb")) == NULL)
//    {
//        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
//        width = 0;
//        height = 0;
//        return NULL;
//    }
//
//    // Read magic number:
//    retval_fgets = fgets(buf[0], BUFSIZE, fp);
//
//    // Read width and height:
//    do
//    {
//        retval_fgets=fgets(buf[0], BUFSIZE, fp);
//    } while (buf[0][0] == '#');
//    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
//    width  = atoi(buf[1]);
//    height = atoi(buf[2]);
//
//    // Read maxval:
//    do
//    {
//        retval_fgets=fgets(buf[0], BUFSIZE, fp);
//    } while (buf[0][0] == '#');
//
//    // Read image data:
//    rawData = new unsigned char[width * height * 3];
//    read = fread(rawData, width * height * 3, 1, fp);
//    fclose(fp);
//    if (read != 1)
//    {
//        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
//        delete[] rawData;
//        width = 0;
//        height = 0;
//        return NULL;
//    }
//
//    return rawData;
//}

unsigned int Skybox::loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//        unsigned char *data = loadPPM(faces[i].c_str(), width, height);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data); 
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // seamless edge

    return textureID;
}


void Skybox::draw(GLuint shaderProgram)
{
    
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
//    glCullFace(GL_BACK);
//    glDepthMask(GL_FALSE);
    
    // We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");// light.colorvec3
//    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    uModelview = glGetUniformLocation(shaderProgram, "view");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
   
//    glDepthMask(GL_TRUE);

    glDepthFunc(GL_LESS); // set depth function back to default
}





