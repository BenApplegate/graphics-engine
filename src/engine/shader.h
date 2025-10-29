/// Author: Benjamin Applegate
/// Loads, compiles, and links shaders into a shader program
/// Needs to be replaced with a more robust solution
/// Won't be well documented or fully featured until the more robust solution is created

#pragma once

struct shader{
    unsigned int programID;
};

struct shader load_test_shader_program();

struct shader load_shader(const char* vertex_source, const char* fragment_source);