#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "shape.hpp"

class TRIANGLE: public SHAPE {
    public:
        TRIANGLE(float vertices[9], float colors[9]);
        void draw();
        void delete_buffers();

    private:
        GLuint points_vbo;
        GLuint color_vbo;
        GLuint vao;
        GLuint shader_programme;
};

#endif