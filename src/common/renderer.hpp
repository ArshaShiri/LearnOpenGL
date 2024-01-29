#ifndef SRC_COMMON_RENDERER
#define SRC_COMMON_RENDERER

#include <glad/glad.h>

// clang-format off
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// clang-format on

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

#endif /* SRC_COMMON_RENDERER */
