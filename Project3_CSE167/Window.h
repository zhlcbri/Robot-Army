#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
//#include "Cube.h"
#include "shader.h"
#include "Skybox.h"
#include "Node.h"
#include "Transform.h"
#include "Geometry.h"
using namespace std;

class Window
{
public:
	static int width;
	static int height;
    
    static double xpos;
    static double ypos;
    static double OLD_XPOS;
    static double OLD_YPOS;
    static double scrollY;

    static int count;
    static int army_length;
    
    static glm::vec3 A;
    static glm::vec3 B;
    static float angle;
    static glm::vec3 axis;
    
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
    static bool CAMERA_ROTATE;
    static bool OBJ_ROTATE;
    static bool SPHERE_SHOW;
    static bool CULL;
    
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //
    static glm::vec3 trackBallMapping(GLFWwindow* window, double xpos, double ypos);
    
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
