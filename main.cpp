#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils/log.hpp"
#include "shapes/triangle.hpp"
#include "shapes/quad.hpp"
#include "shapes/circle.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
using namespace std;

// For fps control
#include <unistd.h>


// Constants
int window_width = 800;
int window_height = 800;

const char* title = "Hello World!";

void glfw_error_callback(int error, const char* description) {
  gl_log("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
}

void _update_fps_counter(GLFWwindow* window) {
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);

        // For now use the window title to display the frame rate
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

int main() {
    assert(restart_gl_log());
    gl_log("starting GLFW\n%s\n", glfwGetVersionString());

    // Init a callback function for GLFW internal errors
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        cout << "GLFW initialization failed" << endl;
        glfwTerminate();
        return 1;
    }

    // For apple machines
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Init a GLFW window
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
    if (!window) {
        cout << "Window Creation failed" << endl;
        glfwDestroyWindow(window);
        return 1;
    }

    // Create the current context and load glad extension
    glfwMakeContextCurrent(window);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return 0;
    }

    // Print version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // Log openGL paramters after glad is loaded
    log_gl_params();

    // Only draw a pixel when it is positioned closer to the screen (in front of other pixels)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Define traingle points, x, y, z
    float points[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    };

    // Define colors RGB
    float colors[] = {
        0.0f, 0.5f, 1.0f,
        0.3f, 0.5f, 1.0f,
        0.0f, 0.8f, 1.0f,
    };

    // Define traingle points, x, y, z
    float quad_points[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0
    };

    // Define colors RGB
    float quad_colors[] = {
        0.0f, 0.5f, 1.0f,
        0.3f, 0.5f, 1.0f,
        0.0f, 0.8f, 1.0f,
        0.0f, 0.8f, 1.0f,
    };

    //TRIANGLE random_triangle(points, colors);
    QUAD random_quad(quad_points, quad_colors);
    //CIRCLE random_circle(0.0f, 0.0f, 1.0f, 36);
    float speed = 1.0f; // Speed in f/s (distance / time)

    // Set a background color
    glClearColor(0.8f, 0.8f, 1.0f, 1);

    double previous_time = glfwGetTime();

    // game loop
    while(!glfwWindowShouldClose(window)) {
        // Calculate delta time before handling input events
        double delta_time = glfwGetTime() - previous_time;
        //random_triangle.set_delta_time(delta_time);
        random_quad.set_delta_time(delta_time);
        //random_circle.set_delta_time(delta_time);
        previous_time = glfwGetTime();

        // Update fps
        _update_fps_counter(window);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //random_triangle.draw();
        random_quad.draw();
        //random_circle.draw();

        // Put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

        // Update other events like input handling 
        glfwPollEvents();

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        } 
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
            //random_triangle.move_up(speed);
            random_quad.move_up(speed);
            //random_circle.move_up(speed);
        } 
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
            //random_triangle.move_right(speed);
            random_quad.move_right(speed);
            //random_circle.move_right(speed);
        } 
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
            //random_triangle.move_down(speed);
            random_quad.move_down(speed);
            //random_circle.move_down(speed);
        } 
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
            //random_triangle.move_left(speed);
            random_quad.move_left(speed);
            //random_circle.move_left(speed);
        }
    }

    //random_triangle.delete_buffers();
    random_quad.delete_buffers();
    //random_circle.delete_buffers();

    // TODO: Delete shaders aswell

    // Destory/Terminate GLFW components
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}