#include <glad/glad.h> // Include before GLFW
#include <GLFW/glfw3.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../utils/shaders.hpp"
#include "../utils/dlinked_list.hpp"
#include "../utils/linked_list.hpp"
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

bool SPOLY::is_ear(dlinked* poly_vertices, dlinked* triangle_node) {
    dlinked* current_node = poly_vertices;
    for (int i = 0; i < n_vertices; i++) {
        // Check if vertex is one of the triangle vertices
        if (current_node == triangle_node->parent_node ||
            current_node == triangle_node ||
            current_node == triangle_node->child_node
        ) {
            current_node = current_node->child_node;
            continue;
        }

        if (point_in_triangle(triangle_node->parent_node, triangle_node, triangle_node->child_node, current_node)) {
            return false;
        }

        // Move to the next node
        current_node = current_node->child_node;
    }
    return true;
}

bool SPOLY::is_convex(dlinked* A, dlinked* B, dlinked* C) {
    // TODO: Probably make this more efficient
    // Derive vector AB
    float vector_AB_x = A->x - B->x;
    float vector_AB_y = A->y - B->y;

    float slope_AB = vector_AB_y / vector_AB_x;
    float y_intercept_AB = A->y - (slope_AB * A->x);
    
    float slope_SC = -1.0 / slope_AB; 
    float y_intercept_SC = C->y - (slope_SC * C->x);

    float S_x = (y_intercept_SC - y_intercept_AB) / (slope_AB - slope_SC);
    float vector_SC_x = S_x - C->x;

    // Check if the vector AB can be turned right so it forms SC
    if ((-vector_AB_x < 0 && vector_SC_x > 0) || (-vector_AB_x > 0 && vector_SC_x < 0)) {
        return true;
    }
    return false;
}

float SPOLY::get_triangle_area(dlinked* A, dlinked* B, dlinked* C) {
    // Derive vectors from the three vertices
    float vector_AB_x = A->x - B->x;
    float vector_AB_y = A->y - B->y;
    float vector_BC_x =  C->x - B->x;
    float vector_BC_y = C->y - B->y;
    float cross_product = vector_AB_x * vector_BC_y - vector_BC_x * vector_AB_y;
    return abs(cross_product) / 2;
}

bool SPOLY::point_in_triangle(dlinked* A, dlinked* B, dlinked* C, dlinked* P) {
    float triangle_area = get_triangle_area(A, B, C);
    float area_sum = 0.0;
    area_sum += get_triangle_area(A, B, P);
    area_sum += get_triangle_area(A, C, P);
    area_sum += get_triangle_area(B, C, P);

    if (triangle_area == area_sum) {
        return true;
    }
    return false;
}

dlinked* SPOLY::array_to_dlinked(float vertices[]) {
    // Create a circular doubly linked list
    dlinked* first = new dlinked;
    dlinked* current_node = first;
    dlinked* previous = NULL;
    for (int i = 0; i < n_vertices; i++) {
        current_node->x = vertices[3 * i];
        current_node->y = vertices[3 * i + 1];
        current_node->z = vertices[3 * i + 2];

        current_node->parent_node = previous;
        current_node->child_node = new dlinked;
        previous = current_node;
        current_node = current_node->child_node;
    }
    previous->child_node = first;
    first->parent_node = previous;

    return first;
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
