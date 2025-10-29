/// Author: Benjamin Applegate
/// Loads a test triangle into a VBO and VAO
/// Needs to be replaced with a more robust solution
/// Won't be well documented or fully featured until the more robust solution is created

#pragma once

struct mesh {
    unsigned int vao;
    int vbo;
    int ebo;
};

struct mesh setup_test_triangle();