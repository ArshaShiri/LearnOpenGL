#include "vertex_array.hpp"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    const auto numberOfVertexArrays = 1;
    glGenVertexArrays(numberOfVertexArrays, &vertexArrayId_);
}

void VertexArray::bind() { glBindVertexArray(vertexArrayId_); }

void VertexArray::unbind() { glBindVertexArray(0); }

void VertexArray::deleteArrayObject() { glDeleteVertexArrays(1, &vertexArrayId_); }
