#ifndef CIRCLE_H
#define CIRCLE_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "shape.hpp"

class CIRCLE: public SHAPE {
    public:
        CIRCLE(float x_center, float y_center, float radius, int n_sides);
        void draw();
        void delete_buffers();

    private:
        GLuint points_vbo;
        GLuint color_vbo;
        GLuint ebo;
        GLuint vao;
        GLuint shader_programme;
        unsigned int n_elements;
};

#endif