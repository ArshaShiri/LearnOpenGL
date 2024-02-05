#include <stdexcept>

#include <glad/glad.h>

#include "program.hpp"

Program::Program() : programId_{ glCreateProgram() } {}

void Program::attachShader(const unsigned int shaderId) const { glAttachShader(programId_, shaderId); }

void Program::linkProgram() const
{
    glLinkProgram(programId_);
    glValidateProgram(programId_);

    int result;
    glGetProgramiv(programId_, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        int logLength = 0;
        glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &logLength);
        char *message = static_cast<char *>(alloca(static_cast<long unsigned int>(logLength) * sizeof(char)));
        glGetProgramInfoLog(programId_, logLength, &logLength, message);
        throw std::runtime_error(message);
    }
}

void Program::useProgram() const { glUseProgram(programId_); }

void Program::deleteProgram() { glDeleteProgram(programId_); }

int Program::getUniformLocation(const std::string &uniformName) const
{
    const auto location = glGetUniformLocation(programId_, uniformName.c_str());

    if (location == -1)
        throw std::runtime_error("Invalid location for the uniform!");

    return location;
}

void Program::setBool(const std::string &name, bool value)
{
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Program::setInt(const std::string &name, int value) { glUniform1i(getUniformLocation(name), value); }

void Program::setFloat(const std::string &name, float value) { glUniform1f(getUniformLocation(name), value); }

void Program::setUniform4f(const std::string &name, float f0, float f1, float f2, float f3)
{
    glUniform4f(getUniformLocation(name), f0, f1, f2, f3);
}
