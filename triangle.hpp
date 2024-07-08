#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class TRIANGLE {
    public:
        TRIANGLE(float vertices[9], float colors[9]);
        GLuint get_vao();
        GLuint get_shader_programme();
        void draw();
        void move_up(float delta_offset);
        void move_right(float delta_offset);
        void move_down(float delta_offset);
        void move_left(float delta_offset);

    private:
        GLuint vao;
        GLuint shader_programme;
        float xOffset;
        float yOffset;
        glm::mat4 transform;
        void update_transform_matrix();
};

#endif