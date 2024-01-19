#include <iostream>
#include <stdexcept>
#include <string>

#include "shader.hpp"

Shader::Shader(const ShaderType shaderType, const std::string &shaderSource)
  : shaderId_{ 0 }, shaderSource_{ shaderSource.c_str() }
{
    try
    {
        createShader(shaderType);
        compileShader();
    }
    catch (const std::exception &e)
    {
        std::cout << "Shader helper exception with message: '" << e.what() << "'\n";
    }
}

const unsigned int Shader::getShaderId() const { return shaderId_; }

void Shader::deleteShader() const { glDeleteShader(shaderId_); }

void Shader::createShader(const ShaderType shaderType)
{
    switch (shaderType)
    {
    case ShaderType::Vertex: {
        shaderId_ = glCreateShader(GL_VERTEX_SHADER);
        break;
    }
    case ShaderType::Fragment: {
        shaderId_ = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    default:
        throw std::runtime_error("Shader type is not supported!");
        break;
    }

    glShaderSource(shaderId_, 1, &shaderSource_, nullptr);
}

void Shader::compileShader()
{
    glCompileShader(shaderId_);

    // check for shader compile errors
    int success;
    char infoLog[512];

    glGetShaderiv(shaderId_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId_, 512, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }
}
