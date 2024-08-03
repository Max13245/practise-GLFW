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

    // Use copies for linked creation, so the root pointers are still available
    linked *current_convex = new linked;
    linked *current_reflex = new linked;
    linked *current_ear = new linked;
    linked *convex = current_convex;
    linked *reflex = current_reflex;
    linked *ear = current_ear;
    dlinked* poly_vertices = array_to_dlinked(vertices);

    dlinked* current_node = poly_vertices;
    for (int i = 0; i < n_vertices; i++) {
        if (is_convex(current_node->parent_node, current_node, current_node->child_node)) {
            // Add to convex
            current_convex->node_data = current_node;
            current_convex->child_node = new linked;
            current_convex = current_convex->child_node;

            if (is_ear(poly_vertices, current_node)) {
                // Add to ears
                current_ear->node_data = current_node;
                current_ear->child_node = new linked;
                current_ear = current_ear->child_node;
            }
        } else {
            // Add to reflex
            current_reflex->node_data = current_node;
            current_reflex->child_node = new linked;
            current_reflex = current_reflex->child_node;
        } 

        current_node = current_node->child_node;
    };

    print_linked(convex);
    cout << "-----------------" << endl;
    print_linked(reflex);
    cout << "-----------------" << endl;
    print_linked(ear);
};

void SPOLY::print_linked(linked* node) {
    cout << node->node_data->x << endl;
    cout << node->node_data->y << endl;
    if (node->child_node->node_data) {
        print_linked(node->child_node);
    }
}

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
    // Derive vector AB and AC
    float vector_AB_x = A->x - B->x;
    float vector_AB_y = A->y - B->y;
    float vector_AC_x = A->x - C->x;
    float vector_AC_y = A->y - C->y;

    float cross_product = vector_AB_x * vector_AC_y - vector_AC_x * vector_AB_y;
    if (cross_product >= 0) {
        return false;
    }
    return true;
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
