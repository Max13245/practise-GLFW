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
        bool is_ear(dlinked* poly_vertices, dlinked* triangle_node);
        bool is_convex(dlinked* A, dlinked* B, dlinked* C);
        bool point_in_triangle(dlinked* first, dlinked* second, dlinked* third, dlinked* point);
        float get_triangle_area(dlinked* first, dlinked* second, dlinked* third);
};

#endif