/// Author: Benjamin Applegate

#include "mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static float triangle_verts[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

struct mesh setup_test_triangle(){
    struct mesh mesh_data;

    glGenVertexArrays(1, &mesh_data.vao);
    
    glBindVertexArray(mesh_data.vao);

    glGenBuffers(1, &mesh_data.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_data.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
}