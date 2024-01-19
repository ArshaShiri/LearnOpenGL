#include <stdexcept>

#include <glad/glad.h>

#include "program.hpp"

Program::Program() : programId_{ glCreateProgram() } {}

void Program::attachShader(const unsigned int shaderId) const { glAttachShader(programId_, shaderId); }

void Program::linkProgram() const
{
    glLinkProgram(programId_);

    int success;
    char infoLog[512];

    glGetProgramiv(programId_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId_, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }
}

void Program::useProgram() const { glUseProgram(programId_); }
