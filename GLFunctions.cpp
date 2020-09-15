//
// Created by jon on 06/09/2020.
//

#include "GLFunctions.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>
#include <stdexcept>


GLuint loadShader(GLenum shader_type, const std::string &file_path) {
    const GLuint shader_id = glCreateShader(shader_type);

    std::ifstream filestream(file_path);
    if(filestream.fail()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    std::vector<std::string> lines;
    {
        std::string buffer;
        while (getline(filestream, buffer)) {
            buffer.push_back('\n');
            lines.push_back(std::move(buffer));
        }
    }


    std::vector<const GLchar *> lines_c;
    lines_c.reserve(lines.size());
    for(std::string& line: lines) {
        lines_c.push_back(line.c_str());
    }

    glShaderSource(shader_id, lines_c.size(), lines_c.data(), nullptr);
    glCompileShader(shader_id);

    int success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    return shader_id;
}


GLuint makeProgram(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();
    for(GLuint shader: shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        throw std::runtime_error("ERROR Shader program 2 linking failed:\n" + std::string(infoLog));
    }
    return program;
}