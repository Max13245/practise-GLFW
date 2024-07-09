#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils/shaders.hpp"
#include "quad.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

QUAD::QUAD(float vertices[12], float colors[12]) {
    // Vertices must be passed ass Top left, Top Right, Bottom Right, Bottom left
    unsigned int indices[6] = {
        0, 1, 3,
        1, 2, 3
    };

    // Store the indices in ebo
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    // Store the points in a GLbuffer
    points_vbo = 0;
    glGenBuffers(1, &points_vbo);

    // Store the color in a buffer aswell
    color_vbo = 0;
    glGenBuffers(1, &color_vbo);

    // Create a vertex array object
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    shader_programme = create_shaders_from_files("test_vs.glsl", "test_fs.glsl");

    xOffset = 0.0f;
    yOffset = 0.0f;
    this->update_transform_matrix();
};

GLuint QUAD::get_vao() {
    return vao;
}

GLuint QUAD::get_shader_programme() {
    return shader_programme;
}

void QUAD::draw() {
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // Draw points 0-6 from the currently bound VAO with current in-use shader
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Retrieve the matrix uniform location and set the matrix
    unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void QUAD::move_up(float delta_offset) {
    yOffset += delta_offset * delta_time;
    update_transform_matrix();
}

void QUAD::move_right(float delta_offset) {
    xOffset += delta_offset * delta_time;
    update_transform_matrix();
}

void QUAD::move_down(float delta_offset) {
    yOffset -= delta_offset * delta_time;
    update_transform_matrix();
}

void QUAD::move_left(float delta_offset) {
    xOffset -= delta_offset * delta_time;
    update_transform_matrix();
}

void QUAD::update_transform_matrix() {
    // Update the transformation matrix
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));
}

void QUAD::delete_buffers() {
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vao);
}

void QUAD::set_delta_time(double new_delta_time) {
    delta_time = new_delta_time;
}