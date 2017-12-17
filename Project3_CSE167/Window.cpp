#include "Window.h"
#include <math.h>
#include "shader.h"
#include <time.h> //

float zVal = 0.0f;
const char* window_title = "GLFW Starter Project";

GLint shaderProgram_robot;
GLint shaderProgram_skybox;

// On some systems you need to change this to the absolute path
#define ROBOT_VERTEX_SHADER_PATH "robotShader.vert"
#define ROBOT_FRAGMENT_SHADER_PATH "robotShader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "skyboxShader.frag"

// Default camera parameters
glm::vec3 cam_pos(-300.0f, 300.0f, 0.0f);        // e  | Position of camera
//glm::vec3 cam_pos(0.0f, 0.0f, 10.0f);        // e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

double Window::xpos;
double Window::ypos;

double Window::OLD_XPOS;
double Window::OLD_YPOS;

double Window::scrollY = 0.0f;

glm::vec3 Window::A;
glm::vec3 Window::B;
float Window::angle;
glm::vec3 Window::axis;

//glm::mat4 newWorld;
float angle = 25.0f;

bool Window::CAMERA_ROTATE;
bool Window::OBJ_ROTATE;
bool Window::SPHERE_SHOW = false;
bool Window::CULL = false;

//Cube * cube;
Skybox * skybox;
Geometry * head;
Geometry * body;
Geometry * l_arm;
Geometry * r_arm;
Geometry * l_leg;
Geometry * r_leg;
Geometry * l_eyeball;
Geometry * r_eyeball;
//Geometry * l_antenna;
//Geometry * r_antenna; // change to Geometry

Transform * hmtx; // head
Transform * larm_mtx; // l_arm
Transform * rarm_mtx; // r_arm
Transform * lleg_mtx; // l_leg
Transform * rleg_mtx; // r_leg;
Transform * bmtx; // body
Transform * leye_mtx;
Transform * reye_mtx;

Transform * modelMtx;
Transform * temp;
Transform * tempmtx;
Transform * army;

OBJObject * bunny;// debug
OBJObject * sphere;
Geometry * sphere_obj;
//Transform * sphere_mtx;

//glm::mat4 walk = glm::mat4(1.0f);// test spinning animation
//int Window::count = 0;

int Window::army_length = 10; // not correct!
//int army_length = 10;

clock_t previous = 0;
clock_t now = 0;
clock_t elapsed_time = 0;

void Window::initialize_objects()
{
    bunny = new OBJObject("bunny.obj"); // debug
    sphere = new OBJObject("sphere.obj"); //
    sphere_obj = new Geometry("sphere.obj");
    
    skybox = new Skybox();
    head = new Geometry("head.obj");
    body = new Geometry("body.obj");
    l_arm = new Geometry("limb.obj");
    r_arm = new Geometry("limb.obj");
    l_leg = new Geometry("limb.obj");
    r_leg = new Geometry("limb.obj");
    l_eyeball = new Geometry("eyeball.obj");
    r_eyeball = new Geometry("eyeball.obj");
//    l_antenna = new Geometry("antenna.obj");
//    r_antenna = new Geometry("antenna.obj");
    
    hmtx = new Transform(glm::mat4(1.0f)); // rotate 90 degrees
    hmtx->addChild(head);
    larm_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f))); // enable keyboard control to change these values
    larm_mtx->addChild(l_arm);
    larm_mtx->LEFT = true;
    
    rarm_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, 0.0f)));
    rarm_mtx->addChild(r_arm);
    rarm_mtx->RIGHT = true;
    
    bmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)));
    bmtx->addChild(body);
    
    lleg_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -40.0f, 0.0f)));
    lleg_mtx->addChild(l_leg);
    lleg_mtx->LEFT = true;
    
    rleg_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, -40.0f, 0.0f)));
    rleg_mtx->addChild(r_leg);
    rleg_mtx->RIGHT = true;
    
//    sphere_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f))
//                               * glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(55.0f)));//
//    sphere_mtx->addChild(sphere_obj);//
    
//    leye_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 5.0f)));
//    leye_mtx->addChild(l_eyeball);
    
    modelMtx = new Transform(glm::mat4(1.0f));
    modelMtx->addChild(hmtx);
    modelMtx->addChild(larm_mtx);
    modelMtx->addChild(rarm_mtx);
    modelMtx->addChild(bmtx);
    modelMtx->addChild(lleg_mtx);
    modelMtx->addChild(rleg_mtx);
//    modelmtx->addChild(leye_mtx);
    //
    modelMtx->addChild(sphere_obj);//
    
    temp = new Transform(glm::mat4(1.0f));
    temp->addChild(hmtx);
    temp->addChild(larm_mtx);
    temp->addChild(rarm_mtx);
    temp->addChild(bmtx);
    temp->addChild(lleg_mtx);
    temp->addChild(rleg_mtx);
    
    army = new Transform(glm::mat4(1.0f)); //
    float space = 80.0f; // draws 100 robots
    for (int x = 0; x < army_length; x++) {
        for (int z = 0; z < army_length; z++) {
//            Transform * tempmtx = modelMtx;
//            tempmtx->M = glm::translate(glm::mat4(1.0f), glm::vec3((float)x*space, 0.0f, (float)z*space));
            tempmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3((float)x*space, 0.0f, (float)z*space)));
//            tempmtx->origin = glm::vec3((float)x*space, 0.0f, (float)z*space);
//            cout << "tempmtx has origin: " << tempmtx->origin.x << ", " << tempmtx->origin.y << ", " << tempmtx->origin.z << endl;
//            sphere->drawLines(shaderProgram_robot, glm::mat4(1.0f)); // here it won't draw. don't try here
            
            tempmtx->children = modelMtx->children;
            army->addChild(tempmtx);
        }
    }
    
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram_robot = LoadShaders(ROBOT_VERTEX_SHADER_PATH, ROBOT_FRAGMENT_SHADER_PATH);
    shaderProgram_skybox = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
}

void Window::display_callback(GLFWwindow* window)
{
    
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram_skybox);
    skybox->draw(shaderProgram_skybox);

    // Use the shader of programID
    glUseProgram(shaderProgram_robot);
    // Render the object

//    army->radius = 900.0f; //test culling
    cout << "CULL: " << CULL << endl;
//    now = clock();
//    elapsed_time = now - previous;
    float c1 = clock();
    
    if (CULL) {
        for (Node* child: army->children) {
//            glm::vec3 origin = army->origin_in_CVV(army->origin); // x'
            if (child->in_six_planes(child->origin) == 6) {
                child->draw(shaderProgram_robot, glm::mat4(1.0f));
            }
        }
        
//        for (Node* child: army->children) {
//            int count = 0;
//            //            glm::vec3 origin = army->origin_in_CVV(army->origin); // x'
//            //            if (child->in_six_planes(child->origin) == 6) {
//            glm::vec4 origin_NDC = Window::P * Window::V * child->M * glm::vec4(child->origin, 1.0f);
//            origin_NDC = origin_NDC/origin_NDC.w;
//            if ( (origin_NDC.x <= -1.0f || origin_NDC.x >= 1.0f)
//                || (origin_NDC.y <= -1.0f || origin_NDC.y >= 1.0f)
//                || (origin_NDC.z <= -1.0f || origin_NDC.z >= 1.0f) )
//            {count++;}
//
//            if ( (origin_NDC.y <= -1.0f || origin_NDC.y >= 1.0f)
//                || (origin_NDC.x <= -1.0f || origin_NDC.x >= 1.0f)
//                || (origin_NDC.z <= -1.0f || origin_NDC.z >= 1.0f) )
//            {count++;}
//
//            if ( (origin_NDC.z <= -1.0f || origin_NDC.z >= 1.0f)
//                || (origin_NDC.y <= -1.0f || origin_NDC.y >= 1.0f)
//                || (origin_NDC.x <= -1.0f || origin_NDC.x >= 1.0f) )
//            {count++;}
//            cout << "count: " << count << endl;
//            if (count == 0) {child->draw(shaderProgram_robot, glm::mat4(1.0f));}
//
//        }
    } else {
        army->draw(shaderProgram_robot, glm::mat4(1.0f));
    }
    
    float c2 = clock();
    float elapsed_time = (c2 - c1)/CLOCKS_PER_SEC;
//    previous = now;
//    cout << "time elapsed per frame: " << elapsed_time << endl;
    cout << "time elapsed per frame: " << elapsed_time << endl;

    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);

}

void Window::idle_callback()
{    
    skybox->toWorld = glm::translate(glm::mat4(1.0f), cam_pos); // scene won't move when we zoom in/out robots
//    larm_mtx->update();
//    modelMtx->update();
//    army->update();
    temp->update();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(skybox);
    // delete robots
	glDeleteProgram(shaderProgram_robot);
    glDeleteProgram(shaderProgram_skybox);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);
	// Set swap interval to 1
	glfwSwapInterval(1);
	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
//        P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
        P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 3000.0f);
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
        }
        
        switch (key) {
            case GLFW_KEY_S:
                if (mods == GLFW_MOD_SHIFT) {
                    army->scale(1.001f); // was 1.5f
//                    army->M = army->M * glm::scale(glm::mat4(1.0f), glm::vec3(1.001));
                } else {
                    army->scale(0.999f); // make sure it doesn't go out of the window
//                    army->M = army->M * glm::scale(glm::mat4(1.0f), glm::vec3(0.999f));
                }
                break;
                
            case GLFW_KEY_X:
                if (mods == GLFW_MOD_SHIFT) {
                    army->translateX(4.0f);
                } else {
                    army->translateX(-4.0f);
                }
                break;
                
            case GLFW_KEY_B:
                if (!SPHERE_SHOW) {
                    SPHERE_SHOW = true;
                } else {
                    SPHERE_SHOW = false;
                }
                break;
                
            case GLFW_KEY_C:
                if (!CULL) {
                    CULL = true;
                } else {
                    CULL = false;
                }
                break;
                
            case GLFW_KEY_UP:
                army_length++;
                break;
                
            case GLFW_KEY_DOWN:
                army_length--;
                break;
                
            default:
                break;
        }
	}
}

// Map 2D cursor position to 3D; do not change this method
glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xpos, double ypos) {
    glm::vec3 v;
    float d;
    int w = Window::width;
    int h = Window::height;
    
    v.x = (2.0 * xpos - w)/w;
    v.y = (h - 2.0 * ypos)/h;
    v.z = 0.0;
    d = glm::length(v);
    d = (d < 1) ? d : 1.0; // if d is smaller than 1, set d to 1
    v.z = sqrtf(1.0001 - d*d); // or pow(d, 2.0)
    v = glm::normalize(v);
    return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (action == GLFW_PRESS) {
        
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: 
                CAMERA_ROTATE = true;
                break;
                
            case GLFW_MOUSE_BUTTON_RIGHT:
                OBJ_ROTATE = true;
                break;
                
            default:
                break;
        }
    }
    if (action == GLFW_RELEASE) {
        CAMERA_ROTATE = false;
        OBJ_ROTATE = false;
    }
//    scrollY = 0.0;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    A = trackBallMapping(window, OLD_XPOS, OLD_YPOS);
    B = trackBallMapping(window, xpos, ypos);

    if (A == B) {
        return;
    }
    
    float velocity = length(B-A);
    if (velocity < 0.0001) return;
    
    angle = velocity;
    axis = glm::cross(A, B);

    if (CAMERA_ROTATE) {
        glm::mat3 R = glm::mat3(glm::rotate(glm::mat4(1.0f), angle*50/180 * glm::pi<float>(), axis));
        cam_pos = R * cam_pos;
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    }
    
    if (OBJ_ROTATE) {
        if (axis.y > 0) {
            army->M = army->M * glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f));
        } else {
            army->M = army->M * glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
    
//    if (ROTATE) skybox->orbit(angle, axis);
    
    OLD_XPOS = xpos;
    OLD_YPOS = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollY += yoffset;
    float zoomSensitivity = -0.018f; // change
//    obj->translateZ(zoomSensitivity * (float)scrollY);
    
//    army->M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zoomSensitivity * (float)scrollY)) * army->M;
//    cam_pos = glm::mat3(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)scrollY))) * cam_pos;
//    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zoomSensitivity*(float)yoffset));
    glm::vec4 cam_pos_homo = T * glm::vec4(cam_pos, 1.0f);
    cam_pos = cam_pos_homo/cam_pos_homo.w;
//    glm::normalize(cam_pos_homo);
//    cam_pos, = cam_pos_homo[0];
//    cam_pos[1] = cam_pos_homo[1];
//    cam_pos[2] = cam_pos_homo[2];

    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    scrollY = 0.0;
}


