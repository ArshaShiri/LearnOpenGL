#include <iostream>

#include "index_buffer.hpp"
#include "program.hpp"
#include "renderer.hpp"
#include "vertex_array.hpp"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ": " << line << std::endl;
    }

    return true;
}

void Renderer::draw(const VertexArray &vertexArray, const IndexBuffer &indexBuffer, const Program &shaderProgram) const
{
    shaderProgram.useProgram();

    vertexArray.bind();
    indexBuffer.bind();

    GLCall(glDrawElements(GL_TRIANGLES, static_cast<int>(indexBuffer.getCount()), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const { glClear(GL_COLOR_BUFFER_BIT); }
