#ifndef SRC_COMMON_RENDERER
#define SRC_COMMON_RENDERER

#include <glad/glad.h>
#include <signal.h>

// clang-format off
#define ASSERT(x) if (!(x)) raise(SIGTRAP);

#ifdef DEBUG_BUILD
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#  define GLCall(x) x
#endif
// clang-format on

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class VertexArray;
class IndexBuffer;
class Program;

class Renderer
{
  public:
    void draw(const VertexArray &vertexArray, const IndexBuffer &indexBuffer, const Program &shaderProgram) const;
    void clear() const;

  private:
};

#endif /* SRC_COMMON_RENDERER */
