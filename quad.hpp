#ifndef QUAD_H
#define QUAD_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class QUAD {
    public:
        QUAD(float vertices[12], float colors[12]);
        GLuint get_vao();
        GLuint get_shader_programme();
        void draw();
        void move_up(float delta_offset);
        void move_right(float delta_offset);
        void move_down(float delta_offset);
        void move_left(float delta_offset);
        void delete_buffers();
        void set_delta_time(double new_delta_time);

    private:
        GLuint points_vbo;
        GLuint color_vbo;
        GLuint ebo;
        GLuint vao;
        GLuint shader_programme;
        double delta_time;
        float xOffset;
        float yOffset;
        glm::mat4 transform;
        void update_transform_matrix();
};

#endif