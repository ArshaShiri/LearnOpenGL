#ifndef SRC_COMMON_SHADER
#define SRC_COMMON_SHADER

#include <filesystem>
#include <string>

#include <glad/glad.h>

enum class ShaderType
{
    Vertex,
    Fragment
};

/**
 * @brief A simple shader wrapper class
 *
 */
class Shader
{
  public:
    /**
     * @brief Construct a new Shader object from the give source
     *
     * @param shaderType Type of the shader
     * @param shaderSource The source code of the shader
     * @param shaderFilePath The file path to a shader source (it is used if shaderSource is an empty string)
     */
    Shader(const ShaderType shaderType, std::string shaderSource, const std::string &shaderFilePath = "");

    /**
     * @brief Get the id of the shader
     *
     * @return The id of the shader
     */
    unsigned int getShaderId() const;

    /**
     * @brief Delete the underlying shader
     *
     */
    void deleteShader() const;

  private:
    void createShader(const ShaderType shaderType);
    void compileShader();
    std::string getSourceFromFile(const std::string &shaderFilePath) const;

    unsigned int shaderId_;
    std::string shaderSource_;
};

#endif /* SRC_COMMON_SHADER */
