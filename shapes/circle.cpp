#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../utils/shaders.hpp"
#include "circle.hpp"

#include <cmath>

CIRCLE::CIRCLE(float x_center, float y_center, float radius, int n_sides) {
    n_elements = 3 * n_sides;
    float vertices[n_elements];
    float colors[n_elements];
    unsigned int indices[n_elements];
    float angle = 360.0 / float(n_sides) * M_PI / 180.0;

    for (int i = 0; i < n_sides; i++) {
        // Calculate vertices
        float x = radius * cos(i * angle);
        float y = radius * sin(i * angle);
        vertices[(i * 3)] = x;
        vertices[(i * 3) + 1] = y;
        vertices[(i * 3) + 2] = 0.0f;

        // Set colors
        colors[(i * 3)] = 0.0f;
        colors[(i * 3) + 1] = 0.5f;
        colors[(i * 3) + 2] = 1.0f;

        // Set the indices
        indices[(i * 3)] = 0;
        indices[(i * 3) + 1] = i + 1;
        indices[(i * 3) + 2] = i + 2;
    }

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
    glBufferData(GL_ARRAY_BUFFER, n_elements * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, n_elements * sizeof(float), colors, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    shader_programme = create_shaders_from_files("test_vs.glsl", "test_fs.glsl");
};

void CIRCLE::draw() {
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // Draw points 0-6 from the currently bound VAO with current in-use shader
    glDrawElements(GL_TRIANGLE_FAN, n_elements, GL_UNSIGNED_INT, 0);
    // Retrieve the matrix uniform location and set the matrix
    unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void CIRCLE::delete_buffers() {
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vao);
}
