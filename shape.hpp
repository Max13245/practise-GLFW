#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class SHAPE {
    public:
        SHAPE();
        void move_up(float delta_offset);
        void move_right(float delta_offset);
        void move_down(float delta_offset);
        void move_left(float delta_offset);
        void set_delta_time(double new_delta_time);

    protected:
        double delta_time;
        float xOffset;
        float yOffset;
        glm::mat4 transform;
        void update_transform_matrix();
};

#endif