#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "shader.hpp"

namespace fs = std::filesystem;

Shader::Shader(const ShaderType shaderType, std::string shaderSource, const std::string &shaderFilePath)
  : shaderId_{ 0 }, shaderSource_{ std::move(shaderSource) }
{
    try
    {
        if (shaderSource_.empty())
            shaderSource_ = getSourceFromFile(shaderFilePath);

        createShader(shaderType);
        compileShader();
    }
    catch (const std::exception &e)
    {
        std::cout << "Shader helper exception with message: '" << e.what() << "'\n";
    }
}

unsigned int Shader::getShaderId() const { return shaderId_; }

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

    const char *shaderSourcePtr = shaderSource_.c_str();
    glShaderSource(shaderId_, 1, &shaderSourcePtr, nullptr);
}

void Shader::compileShader()
{
    glCompileShader(shaderId_);

    // check for shader compile errors
    int result;
    glGetShaderiv(shaderId_, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int logLength = 0;
        glGetShaderiv(shaderId_, GL_INFO_LOG_LENGTH, &logLength);
        char *message = static_cast<char *>(alloca(static_cast<long unsigned int>(logLength) * sizeof(char)));
        glGetShaderInfoLog(shaderId_, logLength, &logLength, message);

        throw std::runtime_error(message);
    }
}

std::string Shader::getSourceFromFile(const std::string &shaderFilePath) const
{
    if (!fs::exists(shaderFilePath))
    {
        std::stringstream ss;
        ss << "Shader file cannot be found!\n";
        ss << "Current path: " << fs::current_path() << "\n";
        ss << "Shader path: " << shaderFilePath << "\n";

        throw std::runtime_error(ss.str());
    }

    std::ifstream shaderFile;
    shaderFile.open(shaderFilePath);
    std::stringstream shaderSourceBuffer;
    shaderSourceBuffer << shaderFile.rdbuf();

    return shaderSourceBuffer.str();
}
