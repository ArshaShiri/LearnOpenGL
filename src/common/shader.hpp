#ifndef SRC_COMMON_SHADER_HELPER
#define SRC_COMMON_SHADER_HELPER

#include <string>

#include <glad/glad.h>

enum class ShaderType
{
    Vertex,
    Fragment
};

class Shader
{
  public:
    Shader(const ShaderType shaderType, const std::string &shaderSource);
    const unsigned int getShaderId() const;
    void deleteShader() const;

  private:
    void createShader(const ShaderType shaderType);
    void compileShader();

    unsigned int shaderId_;
    const char *shaderSource_;
};

#endif /* SRC_COMMON_SHADER_HELPER */
