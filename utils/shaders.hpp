#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include "log.hpp"

#include <string>
#include <assert.h>
using namespace std;


GLuint create_shaders_from_files(
    const char* vertex_shader_filename, 
    const char* fragment_shader_filename);

void reload_shader_from_files(GLuint *program, 
    const char* vertex_shader_filename, 
    const char* fragment_shader_filename);

#endif