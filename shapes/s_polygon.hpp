#ifndef SPOLY_H
#define SPOLY_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"

#include "shape.hpp"
#include "../utils/dlinked_list.hpp"

class SPOLY: public SHAPE {
    public:
        SPOLY(float vertices[], int n_vertices);
        dlinked* array_to_dlinked(float vertices[]);
        int n_vertices;
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