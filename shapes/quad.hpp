#ifndef QUAD_H
#define QUAD_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "shape.hpp"

class QUAD: public SHAPE {
    public:
        QUAD(float vertices[12], float colors[12]);
        void draw();
        void delete_buffers();

    private:
        GLuint points_vbo;
        GLuint color_vbo;
        GLuint ebo;
        GLuint vao;
        GLuint shader_programme;
};

#endif