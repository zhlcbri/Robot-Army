#include "Geometry.h"
#include <fstream>
#include <iostream>
#include "Window.h"
//#include "Transform.h"
//OBJObject * obj; //
//glm::mat4 walker = glm::mat4(1.0f);


glm::mat4 walker = glm::rotate(glm::mat4(1.0f), -90.0f/180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
int Geometry::counter = 0;

Geometry::Geometry(const char *filepath) {
    DRAW_ALL = true;
    obj = new OBJObject(filepath);
    //
    if (strcmp(filepath, "sphere.obj") == 0) {
        IS_SPHERE = true;
        DRAW_ALL = false;
    } else {
        IS_SPHERE = false;;
    }
    if (strcmp(filepath, "limb.obj") == 0) {
        IS_LIMB = true;
    } else {
        IS_LIMB = false;
    }
    scaleMat = glm::mat4(1.0f);
}

Geometry::~Geometry() {
    // delete obj?
//    delete(obj);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C) {
    C = C * scaleMat;
    if (IS_SPHERE && Window::SPHERE_SHOW) {
//        obj->drawLines(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f))
//                       * C * glm::scale(glm::mat4(1.0f), glm::vec3(55.0f)));
        float midY = (obj->toWorld[3][1] - obj->toWorld[0][1]);
        obj->drawLines(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, midY+50.0f, 0.0f))
                       * C * glm::scale(glm::mat4(1.0f), glm::vec3(55.0f)));
    } else if (IS_LIMB) {
//        C = C * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 60.0f, 0.0f));
        C = C * glm::rotate(glm::mat4(1.0f), -90.0f/180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
//        C = C * glm::rotate(glm::mat4(1.0f), -180.0f/180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
//        C = C * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 60.0f, 0.0f));
        obj->draw(shaderProgram, C);
    }
    else if (DRAW_ALL) {
        obj->draw(shaderProgram, C * glm::rotate(glm::mat4(1.0f), -90.0f/180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
    }
}

void Geometry::update() {
//    if (L_ARM) {
//    if (counter < 30) {
//        walker = walker * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
//        counter++;
//    } else {
//        walker = walker * glm::rotate(glm::mat4(1.0f), -1.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
//        counter++;
//    }
//    if (counter == 60) counter = 0;
//    }
}

void Geometry::scale(float val) {
    scaleMat = scaleMat* glm::scale(glm::mat4(1.0f), glm::vec3(val));
}

void Geometry::drawSphere(GLuint shaderProgram, OBJObject * sphere) {
    
}

float Geometry:: dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal) {
    return 0.0f;
}
int Geometry::in_six_planes(glm::vec3 origin) {
    return 0;
}
float Geometry::radius_in_NDC(float radius, glm::vec3 normal) {
    return 0.0f;
}


