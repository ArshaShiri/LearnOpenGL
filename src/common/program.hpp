#ifndef SRC_COMMON_PROGRAM
#define SRC_COMMON_PROGRAM

class Program
{
  public:
    Program();
    void attachShader(const unsigned int shaderId) const;
    void linkProgram() const;
    void useProgram() const;

  private:
    unsigned int programId_;
};

#endif /* SRC_COMMON_PROGRAM */
