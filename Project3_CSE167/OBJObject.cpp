#include "OBJObject.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;
//float xVal, yVal, zVal, degVal = 0.0f;
float xVal, yVal, degVal = 0.0f;
float sVal = 1.0f;
// min and max values of object's vertices
float OBJObject::minX = numeric_limits<float>::max();
float OBJObject::minY = numeric_limits<float>::max();
float OBJObject::minZ = numeric_limits<float>::max();

float OBJObject::maxX = numeric_limits<float>::min();
float OBJObject::maxY = numeric_limits<float>::min();
float OBJObject::maxZ = numeric_limits<float>::min();

OBJObject::OBJObject(const char *filepath)
{
    toWorld = glm::mat4(1.0f);
//    toWorld = glm::rotate(glm::mat4(1.0f), (glm::mediump_float)90, glm::vec3(0.0f, 0.0f, 1.0f));
    minX = numeric_limits<float>::max();
    minY = numeric_limits<float>::max();
    minZ = numeric_limits<float>::max();
    //
    maxX = numeric_limits<float>::min();
    maxY = numeric_limits<float>::min();
    maxZ = numeric_limits<float>::min();
    
    parse(filepath);
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //
    glGenBuffers(1, &VBO2); // for normal coloring
    
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW); // 3rd param changed
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // 6 * sizeof(float)
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1); // Enable the usage of layout location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // when layout=1
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW); // 3rd param changed
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


OBJObject::~OBJObject()
{
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath)
{
//    cout << "Parsing file: " << filepath << endl;// 
    
    ifstream ifs;
    float x,y,z; // vertex coordinates
    float r = 0.0;
    float g = 0.0;
    float b = 0.0; // vertex color
    char c1, c2;
    char buf[255]; // buffer for reading line for filepath
    string strbuf;
    
    unsigned int x1, y1, z1, x2, y2, z2;
    
    ifs.open(filepath);
    if (!ifs.is_open()) {
        cerr << "error loading the file" << endl;
        exit(-1);
    } // in case the file can't be found or is corrupt
    
    while (!ifs.eof()) {
        ifs.get(c1);
        ifs.get(c2);
        getline(ifs, strbuf);
        strbuf.copy(buf, strbuf.length());
        
        if ((c1 == 'v') && (c2 == ' ')) {
            sscanf(buf, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
            
            vertices.push_back(glm::vec3(x, y, z)); // do not store the rgb values here
            
        } else if ((c1 == 'v') && (c2 == 'n')) {
            //cout << "vn line read" << endl;
            sscanf(buf, " %f %f %f", &x, &y, &z);
            
            // map the vertex normals in the range of 0 to 1
            // and store as rgb value
            //            glm::vec3 normalized = glm::normalize(glm::vec3(x, y, z)); // normalize the vertex normals
            //            normals.push_back(glm::vec3((normalized.x+1.0f)/2.0f, (normalized.y+1.0f)/2.0f, (normalized.z+1.0f)/2.0f));
            normals.push_back(glm::vec3((x+1.0f)/2.0f, (y+1.0f)/2.0f, (z+1.0f)/2.0f));
            
        } else if ((c1 == 'f') && (c2 == ' ')) {
            // 3 sets of indices to vertices and normals
            // define the 3 corners of a triangle
            
            sscanf(buf, "%u//%u %u//%u %u//%u", &x1, &x2, &y1, &y2, &z1, &z2);
            // make sure that the number you parsed is 3 * num_faces
            // offset all indices by -1
            indices.push_back(x1 - 1);
            indices.push_back(y1 - 1);
            indices.push_back(z1 - 1);
        }
    }
    ifs.close(); // close the file when done parsing
}

void OBJObject::spin(float deg)
{
    // If you haven't figured it out from the last project, this is how you fix spin's behavior
    toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::update()
{
    spin(1.0f);
//    if (count < 30) {
//        spin(1.0f);
//        count++;
//    }
//    else {
//        spin(-1.0f);
//        count++;
//        if (count == 60) count = 0;
//    }
}

void OBJObject::draw(GLuint shaderProgram)
{
    
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;

    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");

    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    //toWorld
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_POINTS, (GLsizei)indices.size()*3, GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram, glm::mat4 C)
{
//    cout << "drawing obj" << endl;
    
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * C; // before it was C
    
//    modelview = modelview * glm::mat4(1.0f);

    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");

    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    //toWorld
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::toCenter() {
//    this->toWorld[3][0] = 0.0f;
//    this->toWorld[3][1] = 0.0f;
//    this->toWorld[3][2] = 0.0f;
    float midX = (maxX + minX)/2.0f;
    float midY = (maxY + minY)/2.0f;
    float midZ = (maxZ + minZ)/2.0f;
    
    translateMat = glm::translate(glm::mat4(1.0f), glm::vec3((-1.0f)*midX, (-1.0f)*midY, (-1.0f)*midZ));
    toWorld = toWorld * translateMat;
}

void OBJObject::scale() {
    float diffX = maxX - minX;
    float diffY = maxY - minY;
    float diffZ = maxZ - minZ;
    
    float maxRange = fmax(diffX, fmax(diffY, diffZ));
    float factor = 1.0f/(maxRange/2.0f);
    
    scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(factor));
    // toWorld = toWorld * scaleMat;
}

void OBJObject::reset() { //
    toCenter();
    scale();
    // set the toWorld= T*S*glm::mat4(1.0f)
    //toWorld = translateMat * scaleMat * glm::mat4(1.0f);
    toWorld = scaleMat * glm::mat4(1.0f);
}

void OBJObject::orbit(float angle, glm::vec3 axis) {
//    this->toWorld = glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), axis) * this->toWorld; // can change value to less or greater than 50
     this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), axis);
}

void OBJObject::translateXY(float xVal, float yVal) {
    translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(xVal*(0.008f), yVal*(0.008f), 0.0f)); //
    toWorld = translateMat * toWorld;
}

void OBJObject::translateZ(float zVal) {
    this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zVal)) * this->toWorld;
}

void OBJObject::scale(float val) {
    this->toWorld = this->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(val));
}

// point rendering
void OBJObject::drawLines(GLuint shaderProgram, glm::mat4 C)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * C; // before it was C
    
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    //toWorld
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glDrawElements(GL_LINES, (GLsizei)indices.size()*3, GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}



