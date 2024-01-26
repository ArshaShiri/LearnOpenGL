#ifndef SRC_COMMON_PROGRAM
#define SRC_COMMON_PROGRAM

#include <string>

class Program
{
  public:
    Program();
    void attachShader(const unsigned int shaderId) const;
    void linkProgram() const;
    void useProgram() const;
    void deleteProgram();
    int getUniformLocation(const std::string &uniformName) const;

  private:
    unsigned int programId_;
};

#endif /* SRC_COMMON_PROGRAM */
