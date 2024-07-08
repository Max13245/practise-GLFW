#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>

class TRIANGLE {
    public:
        TRIANGLE(float vertices[9], float colors[9]);
        GLuint get_vao();
        GLuint get_shader_programme();


    private:
        GLuint vao;
        GLuint shader_programme;
};

#endif