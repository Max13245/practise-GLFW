#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils/shaders.hpp"
#include "triangle.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

TRIANGLE::TRIANGLE(float vertices[9], float colors[9]) {
    // Store the points in a GLbuffer
    points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Store the color in a buffer aswell
    color_vbo = 0;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

    // Create a vertex array object
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    shader_programme = create_shaders_from_files("test_vs.glsl", "test_fs.glsl");

    xOffset = 0.0f;
    yOffset = 0.0f;
    this->update_transform_matrix();
};

GLuint TRIANGLE::get_vao() {
    return vao;
}

GLuint TRIANGLE::get_shader_programme() {
    return shader_programme;
}

void TRIANGLE::draw() {
    // ALWAYS draw after moving, since then the transform matrix is updated and previous time is not yet updated
    // Retrieve the matrix uniform location and set the matrix
    unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // Update previous time so delta time can be calculated if necessary
    previous_time = glfwGetTime();
}

void TRIANGLE::move_up(float delta_offset) {
    yOffset += delta_offset * get_delta_time();
    update_transform_matrix();
}

void TRIANGLE::move_right(float delta_offset) {
    xOffset += delta_offset * get_delta_time();
    update_transform_matrix();
}

void TRIANGLE::move_down(float delta_offset) {
    yOffset -= delta_offset * get_delta_time();
    update_transform_matrix();
}

void TRIANGLE::move_left(float delta_offset) {
    xOffset -= delta_offset * get_delta_time();
    update_transform_matrix();
}

void TRIANGLE::update_transform_matrix() {
    // Update the transformation matrix
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));
}

void TRIANGLE::delete_buffers() {
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &vao);
}

double TRIANGLE::get_delta_time() {
    cout << glfwGetTime() - previous_time << endl;
    return glfwGetTime() - previous_time;
}