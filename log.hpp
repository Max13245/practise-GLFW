#ifndef LOG_H
#define LOG_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>

void _print_shader_info_log(GLuint shader_index);
void _print_programme_info_log(GLuint programme);
const char* GL_type_to_string(GLenum type);
void print_all(GLuint programme);
bool is_valid(GLuint programme);
bool restart_gl_log();
bool gl_log(const char *message, ...);
void log_gl_params();

#endif