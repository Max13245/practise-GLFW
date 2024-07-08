#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include "log.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
using namespace std;

// Constants
int window_width = 800;
int window_height = 800;

const char* title = "Hello World!";
const char* vs_path = "test_vs.glsl";
const char* fs_path = "test_fs.glsl";

GLuint create_compiled_shader(string shader_str, int SHADER_TYPE) {
    // Convert string to const char *
    const char * shader_char = shader_str.c_str();

    // Load the shaders into GL shaders
    GLuint shader = glCreateShader(SHADER_TYPE);
    glShaderSource(shader, 1, &shader_char, NULL);
    glCompileShader(shader);

    // Check for compile errors
    int compile_params = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_params);
    if (GL_TRUE != compile_params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shader);
        _print_shader_info_log(shader);
        exit(1);
    }

    return shader;
}

GLuint create_shader_program_from_strings(string vs_string, string fs_string){
    GLuint vs = create_compiled_shader(vs_string, GL_VERTEX_SHADER);
    GLuint fs = create_compiled_shader(fs_string, GL_FRAGMENT_SHADER);

    // Create an empty program which will serve as the complete shader program (when compiled shaders are added)
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);

    // check if link was successful
    int link_params = -1;
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &link_params);
    if (GL_TRUE != link_params) {
        fprintf(stderr,
            "ERROR: could not link shader programme GL index %u\n",
            shader_programme);
        _print_programme_info_log(shader_programme);
        exit(1);
    }

    return shader_programme;
}

string get_shaders(const char* path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

GLuint create_shaders_from_files(
    const char* vertex_shader_filename, 
    const char* fragment_shader_filename) {
        string vs_string = get_shaders(vertex_shader_filename);
        string fs_string = get_shaders(fragment_shader_filename);
        return create_shader_program_from_strings(vs_string, fs_string);
}

void reload_shader_from_files(GLuint *program, 
    const char* vertex_shader_filename, 
    const char* fragment_shader_filename){
        assert( program && vertex_shader_filename && fragment_shader_filename );

        GLuint reloaded_program = create_shaders_from_files(vertex_shader_filename, fragment_shader_filename);

        if (reloaded_program) {
            glDeleteProgram(*program);
            *program = reloaded_program;
        }
}

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

    // get version info
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

    // Store the points in a GLbuffer
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint color_vbo = 0;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

    // Create a vertex array object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint shader_programme = create_shaders_from_files(vs_path, fs_path);

    // Set a background color
    glClearColor(0.8f, 0.8f, 1.0f, 1);

    // game loop
    while(!glfwWindowShouldClose(window)) {
        _update_fps_counter(window);
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MACDIFF? When i update the viewport on mac everything breaks (traingles are weirdly positioned on window)
        // Some things not updated during dragging
        //glViewport(0, 0, window_width, window_height);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

        // update other events like input handling 
        glfwPollEvents();

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        }
    }

    // Destory/Terminate GLFW components
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}