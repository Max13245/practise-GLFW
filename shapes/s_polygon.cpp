#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../utils/shaders.hpp"
#include "../utils/dlinked_list.hpp"
#include "s_polygon.hpp"

#include <cmath>
#include <iostream>
using namespace std;

SPOLY::SPOLY(float vertices[], int n_poly_vertices) {
    n_vertices = n_poly_vertices;
    dlinked* poly_vertices = array_to_dlinked(vertices);

    cout << ":::::::::::" << endl;
    dlinked* current_node = poly_vertices;
    while (true) {
        cout << current_node->x << endl;
        if (current_node->child_node) {
            current_node = current_node->child_node;
        } else {
            break;
        }
    };
};

float SPOLY::get_triangle_area(dlinked* first, dlinked* second, dlinked* third) {
    // Derive vectors from the three vertices
    float vector_1x = first->x - second->x; // -1
    float vector_1y = first->y - second->y; // -1
    float vector_2x =  third->x - second->x; // 1
    float vector_2y = third->y - second->y; // -1
    float cross_product = vector_1x * vector_2y - vector_2x * vector_1y;
    return abs(cross_product) / 2;
}

bool SPOLY::point_in_triangle(dlinked* first, dlinked* second, dlinked* third, dlinked* point) {
    float triangle_area = get_triangle_area(first, second, third);
    float area_sum = 0.0;
    area_sum += get_triangle_area(first, second, point);
    area_sum += get_triangle_area(first, third, point);
    area_sum += get_triangle_area(second, third, point);

    if (triangle_area == area_sum) {
        return true;
    }
    return false;
}

float SPOLY::get_angle(dlinked* first, dlinked* second, dlinked* third) {
    // Derive vectors from the three vertices
    float vector_1x = first->x - second->x;
    float vector_1y = first->y - second->y;
    float vector_2x =  third->x - second->x;
    float vector_2y = third->y - second->y;

    // Calculate the angle between the 2 vectors (in degrees)
    float angle = acos(
        (vector_1x * vector_2x + vector_1y * vector_2y) / 
        (sqrt(pow(vector_1x, 2) + pow(vector_1y, 2)) * 
        sqrt(pow(vector_2x, 2) + pow(vector_2y, 2)))) *
        180 / M_PI;
    cout << angle << endl;
    return angle;
}

dlinked* SPOLY::array_to_dlinked(float vertices[]) {
    // Loop through all vertices in reverse so the previous pointer points to the node with the first vertex
    dlinked* previous = NULL;
    for (int i = n_vertices - 1; i >= 0; i--) {
        dlinked *current_node = new dlinked;
        current_node->x = vertices[3 * i];
        current_node->y = vertices[3 * i + 1];
        current_node->z = vertices[3 * i + 2];
        current_node->child_node = previous;
        if (previous) {
            previous->parent_node = current_node;
        }
        previous = current_node;
    }
    return previous;
}

void SPOLY::draw() {
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // Draw points 0-n from the currently bound VAO with current in-use shader
    glDrawElements(GL_TRIANGLE_STRIP, n_vertices, GL_UNSIGNED_INT, 0);
    // Retrieve the matrix uniform location and set the matrix
    unsigned int transformLoc = glGetUniformLocation(shader_programme, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void SPOLY::delete_buffers() {
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vao);
}
