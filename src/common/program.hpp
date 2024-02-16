#ifndef SRC_COMMON_PROGRAM
#define SRC_COMMON_PROGRAM

#include <string>

#include <glm/glm.hpp>

class Program
{
  public:
    Program();
    void attachShader(const unsigned int shaderId) const;
    void linkProgram() const;
    void useProgram() const;
    void deleteProgram();
    int getUniformLocation(const std::string &uniformName) const;
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setUniform4f(const std::string &name, float f0, float f1, float f2, float f3);
    void setMatrix4fv(const std::string &name, const glm::mat4 &matrix);

  private:
    unsigned int programId_;
};

#endif /* SRC_COMMON_PROGRAM */
