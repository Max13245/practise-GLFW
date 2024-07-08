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

    // Delete shaders after link, since not necessary anymore
    glDeleteShader(vs);
    glDeleteShader(fs);

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