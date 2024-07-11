#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils/shaders.hpp"
#include "shape.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

SHAPE::SHAPE() {
    xOffset = 0.0f;
    yOffset = 0.0f;
    this->update_transform_matrix();
};

void SHAPE::move_up(float delta_offset) {
    yOffset += delta_offset * delta_time;
    update_transform_matrix();
}

void SHAPE::move_right(float delta_offset) {
    xOffset += delta_offset * delta_time;
    update_transform_matrix();
}

void SHAPE::move_down(float delta_offset) {
    yOffset -= delta_offset * delta_time;
    update_transform_matrix();
}

void SHAPE::move_left(float delta_offset) {
    xOffset -= delta_offset * delta_time;
    update_transform_matrix();
}

void SHAPE::update_transform_matrix() {
    // Update the transformation matrix
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));
}

void SHAPE::set_delta_time(double new_delta_time) {
    delta_time = new_delta_time;
}