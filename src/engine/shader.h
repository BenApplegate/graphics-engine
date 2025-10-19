#pragma once

struct shader{
    unsigned int programID;
};

struct shader load_test_shader_program();

struct shader load_shader(const char* vertex_source, const char* fragment_source);