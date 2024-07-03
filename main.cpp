#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
using namespace std;

#define GL_LOG_FILE "gl.log"

// Constants
int window_width = 800;
int window_height = 800;

const char* title = "Hello World!";
const string vs_path = "test_vs.glsl";
const string fs_path = "test_fs.glsl";
const string fs_path2 = "test_fs2.glsl";


string get_shaders(string path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

/* Some copy pasted logging code */
bool restart_gl_log() {
    FILE* file = fopen(GL_LOG_FILE, "w");
    if(!file) {
        fprintf(stderr,
        "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
        GL_LOG_FILE);
        return false;
    }
    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
    fclose(file);
    return true;
}

bool gl_log(const char *message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file) {
        fprintf(stderr, 
        "ERROR: could not open GL_LOG_FILE %s file for appending\n", 
        GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

void log_gl_params() {
    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO,
    };
    const char* names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO",
    };
    gl_log("GL Context Params:\n");
    // integers - only works if the order is 0-10 integer return types
    for (int i = 0; i < 10; i++) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        gl_log("%s %i\n", names[i], v);
    }
    // others
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[10], v);
    gl_log("%s %i %i\n", names[10], v[0], v[1]);
    unsigned char s = 0;
    glGetBooleanv(params[11], &s);
    gl_log("%s %u\n", names[11], (unsigned int)s);
    gl_log("-----------------------------\n");
}

void glfw_error_callback(int error, const char* description) {
  gl_log("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;
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

    // Store the points in a GLbuffer
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    // Create a vertex array object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Define traingle points, x, y, z
    float triangle_points[] = {
    -1.0f,  0.1f,  0.0f,
    0.6f, -0.4f,  0.0f,
    -0.4f, -0.4f,  0.0f
    };

    // Store the points in a GLbuffer
    GLuint vbo2 = 0;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triangle_points, GL_STATIC_DRAW);

    // Create a vertex array object
    GLuint vao2 = 0;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // This is the vertex shader, decides where 3d points should end up on the screen
    string vertex_shader_f = get_shaders(vs_path);
    const char * vertex_shader = vertex_shader_f.c_str();

    // This is the fragment shader, decides the color of one pixels sized fragment (r, g, b, a)
    string fragment_shader_f = get_shaders(fs_path);
    const char * fragment_shader = fragment_shader_f.c_str();

    string fragment_shader_f2 = get_shaders(fs_path2);
    const char * fragment_shader2 = fragment_shader_f2.c_str();
    

    // Load the shaders into GL shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs2, 1, &fragment_shader2, NULL);
    glCompileShader(fs2);

    // Create an empty program which will serve as the complete shader program (when compiled shaders are added)
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);

    GLuint shader_programme2 = glCreateProgram();
    glAttachShader(shader_programme2, vs);
    glAttachShader(shader_programme2, fs2);
    glLinkProgram(shader_programme2);

    // Set a background color
    glClearColor(0.8f, 0.8f, 1.0f, 1);

    // game loop
    while(!glfwWindowShouldClose(window)) {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MACDIFF? When i update the viewport on mac everything breaks (traingles are weirdly positioned on window)
        //glViewport(0, 0, window_width, window_height);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shader_programme2);
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update other events like input handling 
        glfwPollEvents();

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        }
    }

    // Destory/Terminate GLFW components
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}