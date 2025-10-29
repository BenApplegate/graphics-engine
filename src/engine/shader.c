/// Author: Benjamin Applegate

#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

static const struct shader blank_shader = {0};


static const char* testVertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char* testFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = vec4(0.0f, 0.7f, 0.8f, 1.0f);\n"
    "}\0";

struct shader load_test_shader_program(){
    struct shader test_shader;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &testVertexShaderSource, 0);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &testFragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    test_shader.programID = glCreateProgram();
    glAttachShader(test_shader.programID, vertexShader);
    glAttachShader(test_shader.programID, fragmentShader);
    glLinkProgram(test_shader.programID);
    glUseProgram(test_shader.programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return test_shader;
}

struct shader load_shader(const char* vertex_source, const char* fragment_source){
    //Open and map vertex shader source
    int vertexFD = open(vertex_source, O_RDONLY, 0);
    if(vertexFD == -1){
        fprintf(stderr, "Could not open the specified vertex shader");
        return blank_shader;
    }
    struct stat vertexStats;
    fstat(vertexFD, &vertexStats);
    const char* vertex_shader_source = mmap(nullptr, vertexStats.st_size, PROT_READ, MAP_PRIVATE, vertexFD, 0);
    //Compile vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        fprintf(stderr, "Failed to compile vertex shader: %s\n", infoLog);
    }
    munmap((void*)vertex_shader_source, vertexStats.st_size);
    close(vertexFD);

    //Open and map fragment shader source
    int fragmentFD = open(fragment_source, O_RDONLY, 0);
    if(fragmentFD == -1){
        fprintf(stderr, "Could not open the specified vertex shader");
        return blank_shader;
    }
    fstat(vertexFD, &vertexStats);
    const char* fragment_shader_source = mmap(nullptr, vertexStats.st_size, PROT_READ, MAP_PRIVATE, fragmentFD, 0);

    //Compile fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        fprintf(stderr, "Failed to compile fragment shader: %s\n", infoLog);
    }

    munmap((void*) fragment_shader_source, vertexStats.st_size);
    close(fragmentFD);

    struct shader program;
    program.programID = glCreateProgram();
    glAttachShader(program.programID, vertexShader);
    glAttachShader(program.programID, fragmentShader);
    glLinkProgram(program.programID);
    glGetProgramiv(program.programID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program.programID, 512, nullptr, infoLog);
        fprintf(stderr, "Failed to link shader program: %s\n", infoLog);
    }
    glUseProgram(program.programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}